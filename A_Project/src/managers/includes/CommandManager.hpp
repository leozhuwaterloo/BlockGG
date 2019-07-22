#pragma once
#include <functional>
#include <unordered_map>
#include "../../guis/includes/StaticTexturedGui.hpp"
#include <memory>

class CommandManager {
public:
  CommandManager();
  virtual ~CommandManager();
  void init();
  void executeCommand(const char* command);
  void registerCommand(const char* command, const std::function<void()> &function, const char *info);
  void registerCommand(const char* command, const std::function<void(float)> &function, const char *info);
  std::unordered_map<std::string, std::string> command_info_map;
  bool render_texture, render_shadow, render_particle, render_bump, render_gui;
  bool render_skybox;
  bool showing_shadow_map;
  bool static_collision, dynamic_collision;
  std::unique_ptr<StaticTexturedGui> shadow_map;
  glm::vec3 init_gravity;
  float init_speed;
  float friction;
  float sound_effect, sound_bg;
  bool god_mode;
  bool bump_test;
protected:
  std::unordered_map<std::string, std::function<void(float)>> command_with_arg_map;
  std::unordered_map<std::string, std::function<void()>> command_map;
};
