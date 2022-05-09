#include "Game.hpp"
#include "ConfigLoader.hpp"
#include "scene/Menu.hpp"
#include "scene/Running.hpp"
#include "utils/Log.hpp"

namespace SimpleSnake {

std::unique_ptr<scene::SceneManager>
Game::createSceneManager(sf::RenderWindow &window) {
  auto sceneMgr = std::make_unique<scene::SceneManager>();
  auto menu =
      std::make_unique<scene::Menu>(*sceneMgr, window, scene::SceneId::Menu);
  auto running = std::make_unique<scene::Running>(*sceneMgr, window,
                                                  scene::SceneId::Running);

  sceneMgr->add(std::move(menu));
  sceneMgr->add(std::move(running));
  sceneMgr->change(scene::SceneId::Menu);
  return sceneMgr;
}

bool Game::initialize(int argc, char **argv) {
  m_config = ConfigLoader::loadConfig(argc, argv);
  LOG_INF("Loaded configuration: [%s]", m_config->asString().c_str());

  m_window =
      std::make_unique<sf::RenderWindow>(sf::VideoMode(640, 480), "Snake");
  if (not m_window) {
    return false;
  }

  m_sceneMgr = createSceneManager(*m_window);
  return true;
}

int Game::run(int argc, char **argv) {
  LOG_DBG("Debug mode enabled");
  if (not initialize(argc, argv)) {
    LOG_ERR("Initalization failed");
    return 1;
  }

  const auto frameTimeMs{
      static_cast<unsigned int>(1.f / m_config->maxFps() * 1000)};
  sf::Clock clock;
  while (m_window->isOpen()) {
    handleEvents();
    update();
    if (clock.getElapsedTime().asMilliseconds() > frameTimeMs) {
      draw();
      clock.restart();
    }
  }
  return 0;
}

void Game::handleEvents() { m_sceneMgr->getActive().handleEvents(); }

void Game::update() { m_sceneMgr->getActive().update(); }

void Game::draw() { m_sceneMgr->getActive().draw(); }

} // namespace SimpleSnake