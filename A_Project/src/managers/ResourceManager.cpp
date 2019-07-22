#include "./includes/ResourceManager.hpp"
#include "./includes/GameManager.hpp"
#include "../models/includes/scene_lua.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/Exception.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "./includes/LogManager.hpp"
#include "../utils/includes/CommonUtils.hpp"
#include <lodepng/lodepng.h>
#include <AL/alut.h>


static LogManager *logger = LogManager::getInstance();

ResourceManager *ResourceManager::instance = nullptr;

ResourceManager* ResourceManager::getInstance() {
  if (!instance) instance = new ResourceManager("./");
  return instance;
}

//----------------------------------------------------------------------------------------
// Constructor
ResourceManager::ResourceManager(const std::string &base_dir)
  : base_dir(base_dir),
    ambient_intensity(glm::vec3(0.2f)),
    m_shader(std::unique_ptr<MasterShader>(new MasterShader())),
    s_shader(std::unique_ptr<ShadowShader>(new ShadowShader())),
    g_shader(std::unique_ptr<GuiShader>(new GuiShader())),
    p_shader(std::unique_ptr<ParticleShader>(new ParticleShader())),
    n_shader(std::unique_ptr<NormalShader>(new NormalShader())),
    b_shader(std::unique_ptr<SkyboxShader>(new SkyboxShader())),
    bg_volume(0.05f) {}

//----------------------------------------------------------------------------------------
// Destructor
ResourceManager::~ResourceManager()
{
  glDeleteBuffers(1, &m_mesh_2d->vbo_position);
  glDeleteBuffers(1, &m_mesh->vbo_position);
  glDeleteBuffers(1, &m_mesh->vbo_normal);
  glDeleteBuffers(1, &m_mesh->vbo_uv_coord);
  glDeleteBuffers(1, &m_mesh->vbo_tangent);

  alutExit();
  for (const auto &it : sound_map) {
    alDeleteBuffers(1, &(it.second));
  }
  alDeleteSources(1, &background_music_source);
}

void ResourceManager::init(){
  logger->log(LogLevel::Info, "Initializing light");
  initLight();
  logger->log(LogLevel::Info, "Initializing projection matrix");
  initProjectionMatrix();
  logger->log(LogLevel::Info, "Initializing view matrix");
  initViewMatrix();
  logger->log(LogLevel::Info, "Loading textures");
  initTexture();
  logger->log(LogLevel::Info, "Loading sounds");
  initSound();
  logger->log(LogLevel::Info, "Loading shaders");
  initShader();
  logger->log(LogLevel::Info, "Loading meshes");
  initMeshSource();
  logger->log(LogLevel::Info, "Loading models");
  initModel();
  game->player->setResourceManager(this);
}

void ResourceManager::loadTexture(const std::string &name, const std::string &path) {
  glGenTextures(1, &texture_map[name]);
  glBindTexture(GL_TEXTURE_2D, texture_map[name]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  // Load texture
  unsigned width, height;
  std::vector<unsigned char> image;
  logger->log(LogLevel::Debug, "Loading texture: " + name);
  unsigned error = lodepng::decode(image, width, height, path);
  if (error == 0) {
    if (name != "block_gg_trans" && name != "heart") {
      /*flip texture*/
      unsigned char* top = nullptr;
    	unsigned char* bot = nullptr;
      for (int i = 0; i < height / 2; i++) {
      	top = &image[0] + i * width * 4;
      	bot = &image[0] + (height - i - 1) * width * 4;
      	for (int j = 0; j < width * 4; j++) {
          std::swap(*top, *bot);
      		++top;
      		++bot;
      	}
      }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
    CHECK_GL_ERRORS;
  } else {
    logger->log(LogLevel::Error, CommonUtils::format(
      "Failed to load texture %s", path.c_str()));
  }
}


void ResourceManager::initTexture() {
  std::vector<std::string> files;
  std::string texture_dir = getAssetFilePath("textures/");
  CommonUtils::getFiles(texture_dir.c_str(), files);
  for (const auto &s : files) {
    if (s == "skybox") continue;
    unsigned int s_length = s.length();
    if (s_length > 4 && std::string(&s[s_length - 4]) == ".png") {
      loadTexture(s.substr(0, s_length - 4), texture_dir + s);
    } else {
      throw Exception(s +  " is not a png file");
    }
  }
  glBindTexture(GL_TEXTURE_2D, 0);

  // Initialize skybox texture
  files.clear();
  files.push_back(getAssetFilePath("textures/skybox/right.png"));
  files.push_back(getAssetFilePath("textures/skybox/left.png"));
  files.push_back(getAssetFilePath("textures/skybox/up.png"));
  files.push_back(getAssetFilePath("textures/skybox/down.png"));
  files.push_back(getAssetFilePath("textures/skybox/back.png"));
  files.push_back(getAssetFilePath("textures/skybox/front.png"));

  glGenTextures(1, &texture_map["skybox"]);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_map["skybox"]);
  for(int i = 0; i < files.size(); ++i) {
    std::string path = files[i];
    unsigned width, height;
    std::vector<unsigned char> image;
    logger->log(LogLevel::Debug, "Loading texture: " + path);
    unsigned error = lodepng::decode(image, width, height, path);
    if (error == 0) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
        width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
      CHECK_GL_ERRORS;
    } else {
      logger->log(LogLevel::Error, CommonUtils::format(
        "Failed to load texture %s", path.c_str()));
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  CHECK_GL_ERRORS;
}

static bool check_al_errors() {
  ALCenum error;
  error = alGetError();
  if (error != AL_NO_ERROR) {
    logger->log(LogLevel::Error, alutGetErrorString(error));
    return true;
  }
  return false;
}

void ResourceManager::loadSound(const std::string &name, const std::string &path) {
  logger->log(LogLevel::Debug, "Loading sound: " + name);
  ALsizei size, freq;
  ALenum format;
  ALvoid *data;
  ALboolean loop = AL_FALSE;

  sound_map[name] = alutCreateBufferFromFile(path.c_str());
  check_al_errors();
}

void ResourceManager::initSound() {
  alutInit(0, NULL);
  if (check_al_errors()) return;

  std::vector<std::string> files;
  std::string sound_dir = getAssetFilePath("sounds/");
  CommonUtils::getFiles(sound_dir.c_str(), files);
  for (const auto &s : files) {
    unsigned int s_length = s.length();
    if (s_length > 4 && std::string(&s[s_length - 4]) == ".wav") {
      loadSound(s.substr(0, s_length - 4), sound_dir + s);
    } else {
      throw Exception(s +  " is not a wav file");
    }
  }

  alGenSources(1, &background_music_source);
  alSourcef(background_music_source, AL_PITCH, 1.0f);
  alSourcef(background_music_source, AL_GAIN, bg_volume);
  alSourcei(background_music_source, AL_LOOPING, AL_TRUE);
  alSourcei(background_music_source, AL_SOURCE_RELATIVE, AL_TRUE);
  alSourcef(background_music_source, AL_ROLLOFF_FACTOR, 0.0);
  alSourcei(background_music_source, AL_BUFFER, sound_map.at("music_stage_1"));
  alSourcePlay(background_music_source);
}

void ResourceManager::initShader() {
  m_shader->init(getAssetFilePath("shaders/MasterVertexShader.vs").c_str(),
    getAssetFilePath("shaders/MasterFragmentShader.fs").c_str());
  logger->log(LogLevel::Info, "Master shader loaded");
  s_shader->init(getAssetFilePath("shaders/ShadowVertexShader.vs").c_str(),
    getAssetFilePath("shaders/ShadowFragmentShader.fs").c_str());
  logger->log(LogLevel::Info, "Shadow shader loaded");
  g_shader->init(getAssetFilePath("shaders/GuiVertexShader.vs").c_str(),
    getAssetFilePath("shaders/GuiFragmentShader.fs").c_str());
  logger->log(LogLevel::Info, "Gui shader loaded");
  p_shader->init(getAssetFilePath("shaders/ParticleVertexShader.vs").c_str(),
    getAssetFilePath("shaders/ParticleFragmentShader.fs").c_str());
  logger->log(LogLevel::Info, "Particle shader loaded");
  n_shader->init(getAssetFilePath("shaders/NormalVertexShader.vs").c_str(),
    getAssetFilePath("shaders/NormalFragmentShader.fs").c_str());
  logger->log(LogLevel::Info, "Normal shader loaded");
  b_shader->init(getAssetFilePath("shaders/SkyboxVertexShader.vs").c_str(),
    getAssetFilePath("shaders/SkyboxFragmentShader.fs").c_str());
  logger->log(LogLevel::Info, "Skybox shader loaded");
  uploadCommonSceneUniforms();
}

void ResourceManager::initMeshSource() {
  // Load meshes
  m_mesh = std::unique_ptr<MeshSource>(new MeshSource());
  m_mesh_2d = std::unique_ptr<MeshSource2D>(new MeshSource2D());
  std::unique_ptr<MeshConsolidator> mesh_consolidator (new MeshConsolidator {
    getAssetFilePath("meshes/crate.obj"),
    getAssetFilePath("meshes/gun_smg.obj"),
    getAssetFilePath("meshes/cube.obj"),
    getAssetFilePath("meshes/slime.obj"),
    getAssetFilePath("meshes/grass.obj"),
    getAssetFilePath("meshes/tree1/tree1main.obj"),
    getAssetFilePath("meshes/tree1/tree1leaf1.obj"),
    getAssetFilePath("meshes/tree1/tree1leaf2.obj"),
    getAssetFilePath("meshes/tree1/tree1leaf3.obj"),
  });
  mesh_consolidator->getBatchInfoMap(batch_info_map);

  // For 2d objects
  {
    // Generate vao
    glGenVertexArrays(1, &(m_mesh_2d->vao));
    glBindVertexArray(m_mesh_2d->vao);

    // Generate vbo
    GLfloat positions[] = { -1, 1, -1, -1, 1, 1, 1, -1 };
		glGenBuffers(1, &m_mesh_2d->vbo_position);
		glBindBuffer(GL_ARRAY_BUFFER, m_mesh_2d->vbo_position);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
		CHECK_GL_ERRORS;

    // update batch_info_map
    batch_info_map["quad"].startIndex = 0;
    batch_info_map["quad"].numIndices = 4;
  }

  // For 3d objects
  {
    // Generate vao
    glGenVertexArrays(1, &(m_mesh->vao));
    glBindVertexArray(m_mesh->vao);
    CHECK_GL_ERRORS;

    // Generate vbo
		glGenBuffers(1, &m_mesh->vbo_position);
		glBindBuffer(GL_ARRAY_BUFFER, m_mesh->vbo_position);
		glBufferData(GL_ARRAY_BUFFER, mesh_consolidator->getNumVertexPositionBytes(),
				mesh_consolidator->getVertexPositionDataPtr(), GL_STATIC_DRAW);
		CHECK_GL_ERRORS;

		glGenBuffers(1, &m_mesh->vbo_normal);
		glBindBuffer(GL_ARRAY_BUFFER, m_mesh->vbo_normal);
		glBufferData(GL_ARRAY_BUFFER, mesh_consolidator->getNumVertexNormalBytes(),
				mesh_consolidator->getVertexNormalDataPtr(), GL_STATIC_DRAW);
		CHECK_GL_ERRORS;

		glGenBuffers(1, &m_mesh->vbo_uv_coord);
		glBindBuffer(GL_ARRAY_BUFFER, m_mesh->vbo_uv_coord);
		glBufferData(GL_ARRAY_BUFFER, mesh_consolidator->getNumVertexUVCoordBytes(),
				mesh_consolidator->getVertexUVCoordDataPtr(), GL_STATIC_DRAW);
		CHECK_GL_ERRORS;

    glGenBuffers(1, &m_mesh->vbo_tangent);
		glBindBuffer(GL_ARRAY_BUFFER, m_mesh->vbo_tangent);
		glBufferData(GL_ARRAY_BUFFER, mesh_consolidator->getNumVertexTangentBytes(),
				mesh_consolidator->getVertexTangentDataPtr(), GL_STATIC_DRAW);
		CHECK_GL_ERRORS;
	}
  //-- Unbind target, and restore default values
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ResourceManager::initModel() {
  std::vector<std::string> files;
  std::string model_dir = getAssetFilePath("models/");
  CommonUtils::getFiles(model_dir.c_str(), files);
  for (const auto &s : files) {
    unsigned int s_length = s.length();
    if (s_length > 4 && std::string(&s[s_length - 4]) == ".lua") {
      model_map[s.substr(0, s_length - 4)] = std::unique_ptr<SceneNode>(
        import_lua(model_dir + s));
      logger->log(LogLevel::Debug, "Loading model " + s);
    } else {
      throw Exception(s + " is not a lua file");
    }
  }
}

void ResourceManager::initProjectionMatrix() {
  float aspect = (float)game->m_windowWidth / game->m_windowHeight;
  m_projection = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);
}

void ResourceManager::initViewMatrix() {
  m_view = glm::lookAt(game->player->pos, game->player->pos + game->player->forward,
    game->player->up);
  initLightVPMatrix();
}

void ResourceManager::initLight() {
  m_light = std::unique_ptr<LightSource>(new LightSource(
    glm::vec3(20.0f, 30.0f, 20.0f) * 100000.0f, glm::vec3(1.0f)));
}

void ResourceManager::initLightVPMatrix() {
  glm::vec3 look_pos = game->player->pos;
  m_light->pos = m_light->init_pos + glm::vec3(look_pos.x, 0.0f, look_pos.z);
  float d_target = glm::length(m_light->init_pos);
  l_projection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, d_target - 100.0f, d_target + 50.0f);
  l_view = glm::lookAt(m_light->pos, look_pos,
    glm::vec3(0.0f, 1.0f, 0.0f));
  l_vp_matrix = l_projection * l_view;
}

void ResourceManager::uploadCommonSceneUniforms() {
  // Master Shader
  m_shader->enable();
  glUniformMatrix4fv(m_shader->projection_loc, 1, GL_FALSE, glm::value_ptr(m_projection));
  glUniform3fv(m_shader->ambient_intensity_loc, 1, glm::value_ptr(ambient_intensity));
  glUniform3fv(m_shader->light_rgb_intensity_loc, 1, glm::value_ptr(m_light->rgb_intensity));

  glUniform1i(m_shader->m_texture_loc, m_shader->m_texture_index);
  glUniform1i(m_shader->shadow_map_loc,  m_shader->shadow_map_index);

  CHECK_GL_ERRORS;
  m_shader->disable();

  // Normal Shader
  n_shader->enable();
  glUniformMatrix4fv(n_shader->projection_loc, 1, GL_FALSE, glm::value_ptr(m_projection));
  glUniform3fv(n_shader->ambient_intensity_loc, 1, glm::value_ptr(ambient_intensity));
  glUniform3fv(n_shader->light_rgb_intensity_loc, 1, glm::value_ptr(m_light->rgb_intensity));

  glUniform1i(n_shader->m_texture_loc, n_shader->m_texture_index);
  glUniform1i(n_shader->shadow_map_loc,  n_shader->shadow_map_index);
  glUniform1i(n_shader->normal_map_loc,  n_shader->normal_map_index);
  n_shader->disable();

  // Particle Shader
  p_shader->enable();
  glUniformMatrix4fv(p_shader->projection_loc, 1, GL_FALSE, glm::value_ptr(m_projection));
  p_shader->disable();

  // Skybox Shader
  b_shader->enable();
  glUniformMatrix4fv(b_shader->projection_loc, 1, GL_FALSE, glm::value_ptr(m_projection));
  b_shader->disable();
  CHECK_GL_ERRORS;
}


std::string ResourceManager::getAssetFilePath(const std::string &base) {
  return base_dir + "/assets/" + base;
}
