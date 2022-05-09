#include "Game.hpp"
#include "ConfigLoader.hpp"
#include "utils/Log.hpp"

namespace SimpleSnake {

bool Game::initialize(int argc, char **argv) {
  m_config = ConfigLoader::loadConfig(argc, argv);
  LOG_INF("Loaded configuration: [%s]", m_config->asString().c_str());

  m_window =
      std::make_unique<sf::RenderWindow>(sf::VideoMode(640, 480), "Snake");
  if (not m_window) {
    return false;
  }
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

void Game::handleEvents() {
  sf::Event e;
  while (m_window->pollEvent(e)) {
    if (e.type == sf::Event::Closed) {
      m_window->close();
    }
  }
}

void Game::update() {}

void Game::draw() {
  m_window->clear(sf::Color::Black);
  m_window->display();
}

} // namespace SimpleSnake