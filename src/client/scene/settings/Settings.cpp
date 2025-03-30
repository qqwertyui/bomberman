#include "Settings.hpp"
#include "common/Log.hpp"

namespace bm::scene {
Settings::Settings(SceneManager &sceneMgr, sf::RenderWindow &window,
                   const SceneId &sceneId)
    : Scene(sceneMgr, window, sceneId) {}

void Settings::handleEvents() {
  while (const auto &e = m_window.pollEvent()) {
    if (e->is<sf::Event::Closed>()) {
      m_window.close();
    } else if (const auto *keyPressed = e->getIf<sf::Event::KeyPressed>()) {
      if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
        change(SceneId::Menu);
      } else {
        LOG_DBG("Settings key pressed!");
      }
    }
  }
}

void Settings::update() {}

void Settings::draw() {
  m_window.clear(sf::Color::Green);
  m_window.display();
}

} // namespace bm::scene
