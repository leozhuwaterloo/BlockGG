#pragma once
#include "../../shaders/includes/MasterShader.hpp"
#include "../../shaders/includes/ShadowShader.hpp"
#include "../../shaders/includes/GuiShader.hpp"
#include "../../shaders/includes/ParticleShader.hpp"
#include "../../shaders/includes/NormalShader.hpp"
#include "../../shaders/includes/SkyboxShader.hpp"
#include "../../models/includes/MeshSource.hpp"
#include "../../models/includes/LightSource.hpp"
#include "../../models/includes/SceneNode.hpp"
#include "cs488-framework/MeshConsolidator.hpp"
#include <memory>
#include <AL/al.h>

class GameManager;

class ResourceManager {
public:
  virtual ~ResourceManager();
  static ResourceManager *getInstance();

  void init();
  void initProjectionMatrix();
  void initViewMatrix();
  void initLight();
  void initLightProjectionMatrix();
  void initLightVPMatrix();
  void initTexture();
  void initSound();
  void initShader();
  void initMeshSource();
  void initModel();

  void uploadCommonSceneUniforms();

  std::unique_ptr<MasterShader> m_shader;
  std::unique_ptr<ShadowShader> s_shader;
  std::unique_ptr<GuiShader> g_shader;
  std::unique_ptr<ParticleShader> p_shader;
  std::unique_ptr<NormalShader> n_shader;
  std::unique_ptr<SkyboxShader> b_shader;

  std::unique_ptr<LightSource> m_light;
  std::unique_ptr<MeshSource> m_mesh;
  std::unique_ptr<MeshSource2D> m_mesh_2d;
  BatchInfoMap batch_info_map;

  glm::mat4 m_projection, m_view;
  glm::mat4 l_projection, l_view, l_vp_matrix;

  GameManager *game;

  std::unordered_map<std::string, GLuint> texture_map;
  std::unordered_map<std::string, std::unique_ptr<SceneNode>> model_map;
  std::unordered_map<std::string, ALuint> sound_map;
  glm::vec3 ambient_intensity;
  float bg_volume;
  ALuint background_music_source;
protected:
  std::string base_dir;
  std::string getAssetFilePath(const std::string &base);
  void loadTexture(const std::string &name, const std::string &path);
  void loadSound(const std::string &name, const std::string &path);
private:
	ResourceManager(const std::string &exec_dir);
  ResourceManager(ResourceManager const&);
  ResourceManager& operator=(ResourceManager const&);
  static ResourceManager* instance;
};
