#pragma once
#include "ResourceManager.hpp"
#include <GLFW/glfw3.h>

class EventManager {
public:
	EventManager();
	virtual ~EventManager();
  void init();

  bool keyInputEvent(const int &key, const int &action, const int &mods);
  bool windowResizeEvent(const int &width, const int &height);
  bool cursorEnterWindowEvent(const int &entered);
  bool mouseMoveEvent(const double &xPos, const double &yPos);
  bool mouseButtonInputEvent(const int &button, const int &actions, const int &mods);
  bool mouseScrollEvent(const double &xOffSet, const double &yOffSet);
  bool key_pressed[7];
	void grabMouse(const bool &grab_mouse);
	bool mouse_grabbed;
	static void charInputEvent(GLFWwindow* window, unsigned int c);
private:
  ResourceManager *resource_manager;
  double pre_x, pre_y;
};
