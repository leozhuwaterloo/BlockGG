#include "./includes/CommandManager.hpp"
#include "./includes/LogManager.hpp"
#include "./includes/ResourceManager.hpp"
#include "./includes/GameManager.hpp"
#include "../objects/includes/Enemies.hpp"
#include "../utils/includes/CommonUtils.hpp"

static LogManager *logger = LogManager::getInstance();
static ResourceManager *resource_manager = ResourceManager::getInstance();

static void quit() {
  glfwSetWindowShouldClose(resource_manager->game->m_window, GL_TRUE);
}

static void help() {
  for(const auto &it : resource_manager->game->command_manager->command_info_map){
    logger->log(LogLevel::Info, it.first + ": " + it.second);
  }
}

static void toggleTexture() {
  if (resource_manager->game->command_manager->render_texture) {
    resource_manager->game->command_manager->render_texture = false;
    logger->log(LogLevel::Info, "Toggled texture mapping off");
  } else {
    resource_manager->game->command_manager->render_texture = true;
    logger->log(LogLevel::Info, "Toggled texture mapping on");
  }
}

static void toggleShadow() {
  if (resource_manager->game->command_manager->render_shadow) {
    resource_manager->game->command_manager->render_shadow = false;
    logger->log(LogLevel::Info, "Toggled shadow off");
  } else {
    resource_manager->game->command_manager->render_shadow = true;
    logger->log(LogLevel::Info, "Toggled shadow on");
  }
}

static void toggleShowShadowMap() {
  if (resource_manager->game->command_manager->showing_shadow_map) {
    resource_manager->game->command_manager->showing_shadow_map = false;
    resource_manager->game->remove(resource_manager->game->command_manager->shadow_map.get(), false);
    logger->log(LogLevel::Info, "Hided shadow map");
  } else {
    resource_manager->game->command_manager->showing_shadow_map = true;
    resource_manager->game->add(resource_manager->game->command_manager->shadow_map.get());
    logger->log(LogLevel::Info, "Showing shadow map");
  }
}

static void respawn() {
  resource_manager->game->player->respawn();
  logger->log(LogLevel::Info, "Respawned player");
}

static void toggleStaticCollision() {
  if (resource_manager->game->command_manager->static_collision) {
    resource_manager->game->command_manager->static_collision = false;
    logger->log(LogLevel::Info, "Toggled static collision off");
  } else {
    resource_manager->game->command_manager->static_collision = true;
    logger->log(LogLevel::Info, "Toggled static collision on");
  }
}

static void toggleDynamicCollision() {
  if (resource_manager->game->command_manager->dynamic_collision) {
    resource_manager->game->command_manager->dynamic_collision = false;
    logger->log(LogLevel::Info, "Toggled dynamic collision off");
  } else {
    resource_manager->game->command_manager->dynamic_collision = true;
    logger->log(LogLevel::Info, "Toggled dynamic collision on");
  }
}

static void killAll() {
  resource_manager->game->killAll();
  logger->log(LogLevel::Info, "Killed all enemies");
}

static void toggleParticle() {
  if (resource_manager->game->command_manager->render_particle) {
    resource_manager->game->command_manager->render_particle = false;
    logger->log(LogLevel::Info, "Toggled particle off");
  } else {
    resource_manager->game->command_manager->render_particle = true;
    logger->log(LogLevel::Info, "Toggled particle on");
  }
}

static void setGravity(const float &gravity) {
  resource_manager->game->player->gravity = gravity *
    resource_manager->game->command_manager->init_gravity;
  logger->log(LogLevel::Info, CommonUtils::format("Set gravity to %.2f", gravity));
}

static void setSpeed(const float &speed) {
  resource_manager->game->player->speed = speed *
    resource_manager->game->command_manager->init_speed;
  logger->log(LogLevel::Info, CommonUtils::format("Set player speed to %.2f", speed));
}

static void setShadowWidth(const float &width) {
  GLuint depth_map = resource_manager->game->render_manager->s_renderer->depth_map;
  resource_manager->game->render_manager->s_renderer->shadow_width = width;
  glBindTexture(GL_TEXTURE_2D, depth_map);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
    width, resource_manager->game->render_manager->s_renderer->shadow_height,
    0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  logger->log(LogLevel::Info, CommonUtils::format("Set shadow map width to %.2f", width));
}

static void setShadowHeight(const float &height) {
  GLuint depth_map = resource_manager->game->render_manager->s_renderer->depth_map;
  resource_manager->game->render_manager->s_renderer->shadow_height = height;
  glBindTexture(GL_TEXTURE_2D, depth_map);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
    resource_manager->game->render_manager->s_renderer->shadow_width, height,
    0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  logger->log(LogLevel::Info, CommonUtils::format("Set shadow map height to %.2f", height));
}

static void setFriction(const float &friction) {
  resource_manager->game->command_manager->friction = friction;
  logger->log(LogLevel::Info, CommonUtils::format("Set player friction to %.2f", friction));
}

static void toggleBump() {
  if (resource_manager->game->command_manager->render_bump) {
    resource_manager->game->command_manager->render_bump = false;
    logger->log(LogLevel::Info, "Toggled bump mapping off");
  } else {
    resource_manager->game->command_manager->render_bump = true;
    logger->log(LogLevel::Info, "Toggled bump mapping on");
  }
}

static void setSoundBG(const float &sound_bg) {
  alSourcef(resource_manager->background_music_source, AL_GAIN,
    resource_manager->bg_volume * sound_bg);
  logger->log(LogLevel::Info, CommonUtils::format("Set background sound volume to %.2f", sound_bg));
}

static void setSoundEffect(const float &sound_effect) {
  resource_manager->game->command_manager->sound_effect = sound_effect;
  for (GameObject *object : resource_manager->game->objects) {
    if (object->type == ObjectType::Slime) {
      Slime *slime = static_cast<Slime*>(object);
      alSourcef(slime->sound_source, AL_GAIN, slime->effect_sound * sound_effect);
    }
  }

  logger->log(LogLevel::Info, CommonUtils::format("Set sound effect volume to %.2f", sound_effect));
}


static void toggleGodMode() {
  if (resource_manager->game->command_manager->god_mode) {
    resource_manager->game->command_manager->god_mode = false;
    logger->log(LogLevel::Info, "Toggled god mod off");
    setSpeed(1.0f);
  } else {
    resource_manager->game->command_manager->god_mode = true;
    logger->log(LogLevel::Info, "Toggled god mod on");
    setSpeed(50.0f);
  }
}

static void toggleBumpTest() {
  if (resource_manager->game->command_manager->bump_test) {
    resource_manager->game->command_manager->bump_test = false;
    logger->log(LogLevel::Info, "Hided bump test crates");
  } else {
    resource_manager->game->command_manager->bump_test = true;
    logger->log(LogLevel::Info, "Showing bump test crates");
  }
  toggleGodMode();
  resource_manager->game->player->pos = glm::vec3(-14.0f, 122.0f, 1.0f);
}

static void toggleGui() {
  if (resource_manager->game->command_manager->render_gui) {
    resource_manager->game->command_manager->render_gui = false;
    logger->log(LogLevel::Info, "Toggled gui off");
  } else {
    resource_manager->game->command_manager->render_gui = true;
    logger->log(LogLevel::Info, "Toggled gui on");
  }
}

static void toggleSkybox() {
  if (resource_manager->game->command_manager->render_skybox) {
    resource_manager->game->command_manager->render_skybox = false;
    logger->log(LogLevel::Info, "Toggled skybox off");
  } else {
    resource_manager->game->command_manager->render_skybox = true;
    logger->log(LogLevel::Info, "Toggled skybox on");
  }
}

static void setRenderDistance(const float &render_distance) {
  resource_manager->game->terrain->setRenderDistance(render_distance);
  logger->log(LogLevel::Info, CommonUtils::format("Set render distance to %.2f", render_distance));
}


CommandManager::CommandManager()
  : render_texture(true),
    render_shadow(true),
    showing_shadow_map(false),
    dynamic_collision(true),
    static_collision(true),
    render_particle(true),
    friction(1.0f),
    render_bump(true),
    sound_effect(1.0f),
    god_mode(false),
    render_gui(true),
    bump_test(false),
    render_skybox(true)
{}
CommandManager::~CommandManager() {}

void CommandManager::init() {
  registerCommand("/quit", quit, "Quit the game");
  registerCommand("/help", help, "List of commands");
  registerCommand("/toggle texture", toggleTexture, "Toggle texture mapping on/off");
  registerCommand("/toggle shadow", toggleShadow, "Toggle shadow on/off");
  registerCommand("/toggle bump", toggleBump, "Toggle bump mapping on/off");
  registerCommand("/toggle s_col", toggleStaticCollision, "Toggle static collision on/off");
  registerCommand("/toggle d_col", toggleDynamicCollision, "Toggle dynamic collision on/off");
  registerCommand("/toggle particle", toggleParticle, "Toggle Particle on/off");
  registerCommand("/toggle shadow_map", toggleShowShadowMap, "Show shadow map");
  registerCommand("/toggle bump_test", toggleBumpTest, "Show bump mapping crates");
  registerCommand("/toggle gui", toggleGui, "Toggle gui on/off");
  registerCommand("/toggle skybox", toggleSkybox, "Toggle skybox on/off");

  registerCommand("/respawn", respawn, "Respawn");
  registerCommand("/killall", killAll, "Kill all enemies");
  registerCommand("/set gravity", setGravity, "Set gravity multiplier");
  registerCommand("/set friction", setFriction, "Set player friction multiplier");
  registerCommand("/set speed", setSpeed, "Set player speed multiplier");
  registerCommand("/set s_width", setShadowWidth, "Set shadow map width");
  registerCommand("/set s_height", setShadowHeight, "Set shadow map height");
  registerCommand("/set sound_bg", setSoundBG, "Set background sound volume");
  registerCommand("/set sound_effect", setSoundEffect, "Set sound effect volume");
  registerCommand("/set render", setRenderDistance, "Set render distance");
  registerCommand("/god", toggleGodMode, "Toggle god mode on/off");
  shadow_map = std::unique_ptr<StaticTexturedGui>(new StaticTexturedGui(
    glm::vec2(0.5f, 0.5f), glm::vec2(0.5f, 0.5f),
    resource_manager->game->render_manager->s_renderer->depth_map));
  init_gravity = resource_manager->game->player->gravity;
  init_speed = resource_manager->game->player->speed;
}


void CommandManager::executeCommand(const char* command) {
  std::string c(command);
  if (c == "/") return;
  if (c.substr(0, 4) == "/set") {
    float num = 1.0f;
    char type[20];
    sscanf(c.c_str(), "/set %s %f", type, &num);
    std::string n_c = "/set " + std::string(type);
    if (command_with_arg_map.find(n_c) != command_with_arg_map.end()) {
      command_with_arg_map.at(n_c)(num);
    } else {
      logger->log(LogLevel::Warn, "Unknown command: " + c + " (/help for a list of commands)");
    }
  } else if (command_map.find(c) != command_map.end()) {
    command_map.at(c)();
  } else {
    logger->log(LogLevel::Warn, "Unknown command: " + c + " (/help for a list of commands)");
  }
}

void CommandManager::registerCommand(const char* command,
  const std::function<void()> &function, const char* info)
{
  std::string c(command);
  command_map[c] = function;
  command_info_map[c] = info;
}

void CommandManager::registerCommand(const char* command,
  const std::function<void(float)> &function, const char* info)
{
  std::string c(command);
  command_with_arg_map[c] = function;
  command_info_map[c] = info;
}
