#include "./includes/EventManager.hpp"
#include "./includes/GameManager.hpp"
#include "./includes/LogManager.hpp"
#include "./includes/WindowManager.hpp"
#include "../utils/includes/CommonUtils.hpp"

static LogManager *logger = LogManager::getInstance();
static WindowManager *window_manager =  WindowManager::getInstance();
//----------------------------------------------------------------------------------------
// Constructor
EventManager::EventManager()
  : resource_manager(ResourceManager::getInstance()),
    key_pressed{0},
    pre_x(0),
    pre_y(0),
    mouse_grabbed(false) {}

//----------------------------------------------------------------------------------------
// Destructor
EventManager::~EventManager() {}

void EventManager::init() {
  grabMouse(true);
  ImGuiIO& io = ImGui::GetIO();
  io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
  io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
  io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
  io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
  io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
  io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
  io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
  io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
  io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
  io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
  io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
  io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;

	glfwSetCharCallback(resource_manager->game->m_window, charInputEvent);
}

void EventManager::grabMouse(const bool &grab_mouse){
  if (grab_mouse) {
    logger->log(LogLevel::Info, "Grab mouse");
    glfwSetInputMode(resource_manager->game->m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    mouse_grabbed = true;
  } else {
    logger->log(LogLevel::Info, "Ungrab mouse");
    glfwSetInputMode(resource_manager->game->m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    mouse_grabbed = false;
  }
}


void EventManager::charInputEvent(GLFWwindow* window, unsigned int c) {
  if(window_manager->expand_view) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(c);
  }
}


bool EventManager::keyInputEvent(
  const int &key,
  const int &action,
  const int &mods
) {
	bool eventHandled(false);
  bool entered(false);
  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_ENTER) {
      eventHandled = true;
      window_manager->toggleExpandView(!window_manager->expand_view);
      entered = true;
    } else if (key == GLFW_KEY_F2) {
      eventHandled = true;
      grabMouse(!mouse_grabbed);
    } else if(key == GLFW_KEY_F1) {
      window_manager->show_debug_window = !window_manager->show_debug_window;
			eventHandled = true;
		}
  }
  if (window_manager->expand_view || !mouse_grabbed || entered) {
    ImGuiIO& io = ImGui::GetIO();
    if (action == GLFW_PRESS) {
      io.KeysDown[key] = true;
    } else if (action == GLFW_RELEASE) {
      io.KeysDown[key] = false;
    }
    return true;
  }
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_W) {
      eventHandled = true;
      key_pressed[0] = true;
    } else if (key == GLFW_KEY_D) {
      eventHandled = true;
      key_pressed[1] = true;
    } else if (key == GLFW_KEY_S) {
      eventHandled = true;
      key_pressed[2] = true;
    } else if (key == GLFW_KEY_A) {
      eventHandled = true;
      key_pressed[3] = true;
    } else if (key == GLFW_KEY_SPACE) {
      eventHandled = true;
      resource_manager->game->player->jump();
    } else if (key == GLFW_KEY_Q) {
      eventHandled = true;
      key_pressed[5] = true;
    } else if (key == GLFW_KEY_E) {
      eventHandled = true;
      key_pressed[6] = true;
    } else if (key == GLFW_KEY_R) {
      eventHandled = true;
      if (resource_manager->game->m_paused) {
        resource_manager->game->m_paused = false;
        window_manager->show_message_window = false;
      }
    } else if (key == GLFW_KEY_P) {
      eventHandled = true;
    }
	} else if (action == GLFW_RELEASE) {
    if (key == GLFW_KEY_W) {
      eventHandled = true;
      key_pressed[0] = false;
    } else if (key == GLFW_KEY_D) {
      eventHandled = true;
      key_pressed[1] = false;
    } else if (key == GLFW_KEY_S) {
      eventHandled = true;
      key_pressed[2] = false;
    } else if (key == GLFW_KEY_A) {
      eventHandled = true;
      key_pressed[3] = false;
    } else if (key == GLFW_KEY_Q) {
      eventHandled = true;
      key_pressed[5] = false;
    } else if (key == GLFW_KEY_E) {
      eventHandled = true;
      key_pressed[6] = false;
    }
  }
  return eventHandled;
}

bool EventManager::windowResizeEvent(const int &width, const int &height) {
	bool eventHandled(false);
	resource_manager->initProjectionMatrix();
	return eventHandled;
}


bool EventManager::cursorEnterWindowEvent(const int &entered) {
	bool eventHandled(false);
	// Fill in with event handling code...
	return eventHandled;
}

bool EventManager::mouseMoveEvent(const double &xPos, const double &yPos) {
	bool eventHandled(false);
  if (window_manager->expand_view || !mouse_grabbed) return eventHandled;
  resource_manager->game->player->onMouseMove(pre_x - xPos, pre_y - yPos);
  pre_x = xPos;
  pre_y = yPos;
	return eventHandled;
}


bool EventManager::mouseButtonInputEvent (
  const int &button,
  const int &actions,
  const int &mods
) {
	bool eventHandled(false);
  if (window_manager->expand_view || !mouse_grabbed) return eventHandled;
  if (actions == GLFW_PRESS) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      eventHandled = true;
      key_pressed[4] = true;
    }
  } else if (actions == GLFW_RELEASE) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      eventHandled = true;
      key_pressed[4] = false;
    }
  }
	return eventHandled;
}


bool EventManager::mouseScrollEvent(const double &xOffSet, const double &yOffSet) {
	bool eventHandled(false);
	// Fill in with event handling code...
	return eventHandled;
}
