#include "Game.hpp"
#include "GlobalConfig.hpp"
#include "common/Log.hpp"
#include "common/Networking.hpp"
#include "resource/FontManager.hpp"
#include "resource/TextureManager.hpp"

namespace bm {
std::unique_ptr<scene::SceneManager>
Game::createSceneManager(sf::RenderWindow &window) {
  auto sceneMgr = std::make_unique<scene::SceneManager>(window);
  return sceneMgr;
}

bool Game::initialize(int argc, char **argv) {
  if (not GlobalConfig::load(argc, argv)) {
    return false;
  }
  if (not initNetworking()) {
    LOG_ERR("Networking initialization failed");
    return false;
  }
  auto &config{GlobalConfig::get()};
  m_window = std::make_unique<sf::RenderWindow>(
      sf::VideoMode(sf::Vector2u(config.windowWidth(), config.windowHeight())),
      "Bomberman");
  if (not m_window) {
    LOG_ERR("Couldn't create window");
    return false;
  }

  auto &txtManager = resource::TextureManager::get();
  txtManager.load(resource::TextureId::BUTTON_ACTIVE,
                  config.assetsDir() + "buttonActive.png");
  txtManager.load(resource::TextureId::BUTTON_INACTIVE,
                  config.assetsDir() + "buttonInactive.png");

  auto &fontManager = resource::FontManager::get();
  fontManager.load(resource::FontId::MENU, config.assetsDir() + "Inkfree.ttf");

  m_sceneMgr = createSceneManager(*m_window);
  return true;
}

int Game::run(int argc, char **argv) {
  if (not initialize(argc, argv)) {
    LOG_ERR("Initalization failed");
    return 1;
  }

  const auto frameTimeMs{
      static_cast<int>(1.f / GlobalConfig::get().maxFps() * 1000)};
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

void Game::handleEvents() { m_sceneMgr->handleEvents(); }

void Game::update() { m_sceneMgr->update(); }

void Game::draw() { m_sceneMgr->draw(); }

} // namespace bm
