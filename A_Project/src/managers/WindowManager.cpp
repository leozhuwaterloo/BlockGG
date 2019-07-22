#include "./includes/WindowManager.hpp"
#include "./includes/GameManager.hpp"

WindowManager *WindowManager::instance = nullptr;
WindowManager* WindowManager::getInstance() {
  if (!instance) instance = new WindowManager();
  return instance;
}

//----------------------------------------------------------------------------------------
// Constructor
WindowManager::WindowManager()
  : show_gui(true),
    first_run(true),
    show_debug_window(true),
    show_message_window(false),
    expand_view(false),
    new_expand_view(0),
    msg_size(50),
    buf{0},
    resource_manager(ResourceManager::getInstance())
{
  messages.resize(msg_size);
}

//----------------------------------------------------------------------------------------
// Destructor
WindowManager::~WindowManager() {}


void WindowManager::init() {
  buf[0] = '/';
}

void WindowManager::addMessage(const std::string &msg) {
  messages.push_back(msg);
  while(messages.size() > msg_size) {
    messages.pop_front();
  }
}

void WindowManager::draw() {
  if( !show_gui ) return;

	if (first_run) {
		ImGui::SetNextWindowPos(getPos(50, (expand_view ? -500 : -200) - 50, 0, 1));
    ImGui::SetNextWindowSize(getPos(350, (expand_view ? 500 : 200), 0, 0));
		first_run = false;
	}

	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
    ImGuiWindowFlags_NoMove);
	float opacity(0.3f);

  if (show_debug_window) {
  	ImGui::Begin("Window", &show_debug_window, getPos(350,
      (expand_view ? 500 : 200), 0, 0), opacity, windowFlags);

    ImGui::PushTextWrapPos();
    for (const std::string &x : messages) {
      ImGui::TextUnformatted(x.c_str());
    }
    ImGui::PopTextWrapPos();

    if(expand_view) {
      if (resource_manager->game->event_manager->mouse_grabbed) {
        ImGui::SetKeyboardFocusHere();
      }
      if (new_expand_view) {
        ImGui::GetIO().KeysDown[GLFW_KEY_RIGHT] = true;
        new_expand_view = false;
      } else {
        ImGui::GetIO().KeysDown[GLFW_KEY_RIGHT] = false;
      }
    }
    if(ImGui::InputText("", buf, sizeof(buf), ImGuiInputTextFlags_EnterReturnsTrue)) {
      char* input_end = buf + strlen(buf);
      while (input_end > buf && input_end[-1] == ' ') { input_end--; } *input_end = 0;
      if (buf[0]) resource_manager->game->command_manager->executeCommand(buf);
      strcpy(buf, "/");
    }
    ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);
    ImGui::SameLine(240);
    ImGui::Text("Score: %.1f", resource_manager->game->player->score);
    glm::vec3 pos = resource_manager->game->player->pos;
    ImGui::Text("X: %.1f, Y: %.1f, Z: %.1f", pos.x, pos.y, pos.z);
    ImGui::SameLine(240);
    if(ImGui::SmallButton("Quit Game")) {
      glfwSetWindowShouldClose(resource_manager->game->m_window, GL_TRUE);
    }
    if (resource_manager->game->event_manager->mouse_grabbed) {
      ImGui::SetScrollHere(1.0f);
    }
  	ImGui::End();
  }

  if (show_message_window) {
    ImGui::SetNextWindowPosCenter();
    ImGui::SetNextWindowSize(getPos(0.0f, 0.0f, 0.5f, 0.5f));
    ImGui::Begin("Messsage", &show_message_window,
      getPos(0.0f, 0.0f, 0.5f, 0.5f), 0.1f, windowFlags);
    ImGui::Text("You died!");
    ImGui::Text("Score: %.2f", resource_manager->game->player->pre_score);
    ImGui::Text("Press R to restart...");
    ImGui::End();
  }
}

void WindowManager::toggleExpandView(const bool &expand_view) {
  this->expand_view = expand_view;
  first_run = true;
  new_expand_view = true;
}

ImVec2 WindowManager::getPos(const float &x, const float &y,
  const float & r_x, const float &r_y)
{
  return ImVec2(x + resource_manager->game->m_windowWidth * r_x,
    y + resource_manager->game->m_windowHeight * r_y);
}
