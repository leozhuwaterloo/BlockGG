// Spring 2019

#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "ResourceManager.hpp"
#include "EventManager.hpp"
#include "WindowManager.hpp"
#include "RenderManager.hpp"
#include "CollisionManager.hpp"
#include "CommandManager.hpp"
#include "../../objects/includes/Player.hpp"
#include "../../objects/includes/Terrain.hpp"
#include "../../utils/includes/LinkedList.hpp"
#include <memory>
#include <chrono>

class GameManager : public CS488Window {
public:
	GameManager();
	virtual ~GameManager();
  using CS488Window::m_window;
  using CS488Window::m_windowHeight;
  using CS488Window::m_windowWidth;
	using CS488Window::m_paused;
  Player *player;
	Terrain *terrain;
  std::unique_ptr<EventManager> event_manager;
	std::unique_ptr<CommandManager> command_manager;
	std::unique_ptr<RenderManager> render_manager;
	float time_delta;
	LinkedList<GameObject> objects;
	LinkedList<GuiElement> elements;
	void killAll();
	void restart();
	void add(GameObject *object);
	void add(GuiElement *element);
	void remove(GameObject *object);
	void remove(GuiElement *element);
	void remove(GameObject *object, const bool &be_free);
	void remove(GuiElement *element, const bool &be_free);
protected:
	WindowManager *window_manager;
	ResourceManager *resource_manager;
	std::unique_ptr<CollisionManager> collision_manager;
	std::chrono::high_resolution_clock::time_point last_frame_time;
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	//-- Events
	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;
};
