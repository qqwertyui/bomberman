#include "Game.hpp"
#include "GlobalConfig.hpp"
#include "common/Networking.hpp"
#include "common/logging/Log.hpp"
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
  auto &config{GlobalConfig::get()};
  common::logging::setLevel(
      static_cast<common::logging::Level>(config.logLevel()));

  if (not initNetworking()) {
    LOG_ERR("Networking initialization failed");
    return false;
  }
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

  const auto frameTime = sf::seconds(1.f / GlobalConfig::get().maxFps());
  sf::Clock clock;
  sf::Time timeSinceLastFrame = sf::Time::Zero;

  while (m_window->isOpen()) {
    timeSinceLastFrame += clock.restart();

    handleEvents();
    update();

    if (timeSinceLastFrame >= frameTime) {
      draw();
      timeSinceLastFrame -= frameTime;
    }
  }
  return 0;
}

void Game::handleEvents() { m_sceneMgr->handleEvents(); }

void Game::update() { m_sceneMgr->update(); }

void Game::draw() { m_sceneMgr->draw(); }

} // namespace bm
