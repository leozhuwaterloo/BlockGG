#include "./includes/GameManager.hpp"
#include "./includes/LogManager.hpp"
#include "../utils/includes/CommonUtils.hpp"
#include "../objects/includes/EnemySpawner.hpp"
#include "../guis/includes/StaticTexturedGui.hpp"
#include "../objects/includes/Enemies.hpp"

static LogManager *logger = LogManager::getInstance();

//----------------------------------------------------------------------------------------
// Constructor
GameManager::GameManager()
  : resource_manager(ResourceManager::getInstance()),
    window_manager(WindowManager::getInstance()),
    event_manager(std::unique_ptr<EventManager>(new EventManager())),
    command_manager(std::unique_ptr<CommandManager>(new CommandManager())),
    render_manager(std::unique_ptr<RenderManager>(new RenderManager())),
    collision_manager(std::unique_ptr<CollisionManager>(new CollisionManager())),
    last_frame_time(std::chrono::high_resolution_clock::now())
{

}

//----------------------------------------------------------------------------------------
// Destructor
GameManager::~GameManager() {}


//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void GameManager::init()
{
  logger->log(LogLevel::Info, "Initializing resource manager");
  player = new Player(glm::vec3(0.0f, 5.0f, 0.0f));
  add(new ColoredGui(glm::vec2(0.50f, -0.84f), glm::vec2(0.135f, 0.028f),
    glm::vec4(0.549f, 0.114f, 0.251f, 1.0f)));
  resource_manager->game = this;
  resource_manager->init();
  logger->log(LogLevel::Info, "Initializing window manager");
  window_manager->init();
  logger->log(LogLevel::Info, "Initializing event manager");
  event_manager->init();
  logger->log(LogLevel::Info, "Initializing render manager");
  render_manager->init();
  logger->log(LogLevel::Info, "Initializing command manager");
  command_manager->init();
  logger->log(LogLevel::Info, "Initializing objects");
  terrain = new Terrain(glm::vec3(0.0f));
  add(terrain);
  add(player);
  add(new EnemySpawner());
  add(new StaticTexturedGui(glm::vec2(0.83f, -0.8f), glm::vec2(0.2f, 0.2f),
    resource_manager->texture_map.at("block_gg_trans")));
  add(new StaticTexturedGui(glm::vec2(0.35f, -0.84f), glm::vec2(0.038f, 0.06f),
    resource_manager->texture_map.at("heart")));
  logger->log(LogLevel::Info, "Initializing gui elements");
  logger->log(LogLevel::Info, "Initialization Completed");
}


//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void GameManager::appLogic()
{
  std::chrono::high_resolution_clock::time_point current_frame_time = std::chrono::high_resolution_clock::now();
  for (GameObject *object : objects) {
		object->tick();
	}
  collision_manager->tick();
  for (GuiElement *element : elements) {
		element->tick();
	}
  time_delta = (current_frame_time - last_frame_time).count() / 10000000.0f;
  last_frame_time = current_frame_time;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void GameManager::guiLogic()
{
  window_manager->draw();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void GameManager::draw() {
  render_manager->render();
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void GameManager::cleanup()
{
  delete resource_manager;
  delete window_manager;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool GameManager::cursorEnterWindowEvent (
		int entered
) {
	return event_manager->cursorEnterWindowEvent(entered);
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool GameManager::mouseMoveEvent (
		double xPos,
		double yPos
) {
	return event_manager->mouseMoveEvent(xPos, yPos);
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool GameManager::mouseButtonInputEvent (
		int button,
		int actions,
		int mods
) {
	return event_manager->mouseButtonInputEvent(button, actions, mods);
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool GameManager::mouseScrollEvent (
		double xOffSet,
		double yOffSet
) {
	return event_manager->mouseScrollEvent(xOffSet, yOffSet);
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool GameManager::windowResizeEvent (
		int width,
		int height
) {
  return event_manager->windowResizeEvent(width, height);
}

void GameManager::killAll() {
  std::vector<Enemy*> need_kill;
  for (GameObject *object : objects) {
    Enemy *enemy = dynamic_cast<Enemy*>(object);
    if (enemy != nullptr) {
      need_kill.push_back(enemy);
    }
  }
  for (const auto &it : need_kill) {
    it->kill(false);
  }
}

void GameManager::restart() {
  window_manager->show_message_window = true;
  m_paused = true;

  killAll();
  player->respawn();
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool GameManager::keyInputEvent (
		int key,
		int action,
		int mods
) {
	return event_manager->keyInputEvent(key, action, mods);
}


void GameManager::add(GameObject *object) {
  logger->log(LogLevel::Debug, CommonUtils::format(
    "Adding object %d - %s; Have: %d",
    object->id, OBJECT_TYPE_STR[(int)object->type].c_str(), objects.size));
  objects.add(object);
}

void GameManager::remove(GameObject *object) {
  remove(object, true);
}
void GameManager::remove(GameObject *object, const bool &be_free) {
  logger->log(LogLevel::Debug, CommonUtils::format(
    "Removing object %d - %s; Have: %d",
    object->id, OBJECT_TYPE_STR[(int)object->type].c_str(), objects.size));
  objects.remove(object, be_free);
}

void GameManager::add(GuiElement *element) {
  logger->log(LogLevel::Debug, CommonUtils::format(
    "Adding gui element %d - %s", element->id, ELEMENT_TYPE_STR[(int)element->type].c_str()));
  elements.add(element);
}

void GameManager::remove(GuiElement *element) {
  remove(element, true);
}
void GameManager::remove(GuiElement *element, const bool &be_free) {
  logger->log(LogLevel::Debug, CommonUtils::format(
    "Removing gui element %d - %s", element->id, ELEMENT_TYPE_STR[(int)element->type].c_str()));
  elements.remove(element, be_free);
}
