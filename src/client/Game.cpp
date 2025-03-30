#include "Game.hpp"
#include "GlobalConfig.hpp"
#include "common/Log.hpp"
#include "resource/FontManager.hpp"
#include "resource/TextureManager.hpp"
#include "scene/lobby/Scene.hpp"
#include "scene/menu/Scene.hpp"
#include "scene/settings/Scene.hpp"

namespace bm {
std::unique_ptr<scene::SceneManager>
Game::createSceneManager(sf::RenderWindow &window) {
  auto sceneMgr = std::make_unique<scene::SceneManager>();
  auto menu = std::make_unique<scene::menu::Scene>(*sceneMgr, window,
                                                   scene::SceneId::Menu);
  auto lobby = std::make_unique<scene::lobby::Scene>(*sceneMgr, window,
                                                     scene::SceneId::Lobby);
  auto settings = std::make_unique<scene::settings::Scene>(
      *sceneMgr, window, scene::SceneId::Settings);

  sceneMgr->add(std::move(menu));
  sceneMgr->add(std::move(lobby));
  sceneMgr->add(std::move(settings));
  sceneMgr->change(scene::SceneId::Menu);
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
      static_cast<unsigned int>(1.f / GlobalConfig::get().maxFps() * 1000)};
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

} // namespace bm
