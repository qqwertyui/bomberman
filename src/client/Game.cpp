#include "Game.hpp"
#include "ConfigLoader.hpp"
#include "common/Log.hpp"
#include "rsrcManagement/FontManager.hpp"
#include "rsrcManagement/TextureManager.hpp"
#include "scene/menu/Menu.hpp"
#include "scene/running/Running.hpp"
#include "scene/settings/Settings.hpp"

namespace bomberman {

std::unique_ptr<scene::SceneManager>
Game::createSceneManager(sf::RenderWindow &window) {
  auto sceneMgr = std::make_unique<scene::SceneManager>();
  auto menu =
      std::make_unique<scene::Menu>(*sceneMgr, window, scene::SceneId::Menu);
  auto running = std::make_unique<scene::Running>(*sceneMgr, window,
                                                  scene::SceneId::Running);
  auto settings = std::make_unique<scene::Settings>(*sceneMgr, window,
                                                    scene::SceneId::Settings);

  sceneMgr->add(std::move(menu));
  sceneMgr->add(std::move(running));
  sceneMgr->add(std::move(settings));
  sceneMgr->change(scene::SceneId::Menu);
  return sceneMgr;
}

bool Game::initialize(int argc, char **argv) {
  ConfigLoader::loadConfig(argc, argv);
  auto &globalConfig{GlobalConfig::get()};
  if (globalConfig.isHelp()) {
    LOG_INF("Possible parameters are [%s]",
            globalConfig.getRegisteredNames().c_str());
    return false;
  }
  LOG_INF("Loaded configuration: [%s]", globalConfig.asString().c_str());

  if (not initNetworking()) {
    LOG_ERR("Networking initialization failed");
    return false;
  }

  m_window = std::make_unique<sf::RenderWindow>(
      sf::VideoMode(sf::Vector2u(GlobalConfig::get().windowWidth(),
                                 GlobalConfig::get().windowHeight())),
      "Bomberman");
  if (not m_window) {
    LOG_ERR("Couldn't create window");
    return false;
  }

  auto &txtManager = rsrcManagement::TextureManager::instance();
  txtManager.load(rsrcManagement::TextureId::BUTTON_ACTIVE,
                  globalConfig.assetsDir() + "buttonActive.png");
  txtManager.load(rsrcManagement::TextureId::BUTTON_INACTIVE,
                  globalConfig.assetsDir() + "buttonInactive.png");

  auto &fontManager = rsrcManagement::FontManager::instance();
  fontManager.load(rsrcManagement::FontId::MENU,
                   globalConfig.assetsDir() + "Inkfree.ttf");

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

} // namespace bomberman