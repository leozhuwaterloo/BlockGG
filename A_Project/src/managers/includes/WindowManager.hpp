#pragma once
#include "ResourceManager.hpp"
#include <GLFW/glfw3.h>
#include <list>
#include <imgui/imgui.h>

class WindowManager {
public:
	virtual ~WindowManager();
  static WindowManager *getInstance();
	void init();
  void draw();
	bool show_gui;
  bool first_run;
  bool expand_view;
	bool new_expand_view;
  bool show_debug_window;
	bool show_message_window;
  void toggleExpandView(const bool &expand_view);
	std::list<std::string> messages;
	void addMessage(const std::string &msg);
	char buf[200];
protected:
	int msg_size;
	ResourceManager *resource_manager;
	WindowManager();
  WindowManager(WindowManager const&);
  WindowManager& operator=(WindowManager const&);
  static WindowManager* instance;
  ImVec2 getPos(const float &x, const float &y, const float & r_x, const float &r_y);
};
