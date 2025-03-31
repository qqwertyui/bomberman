#include "Scene.hpp"
#include "common/Log.hpp"

namespace bm::scene::settings {
Scene::Scene(SceneManager &sceneMgr, sf::RenderWindow &window)
    : SceneBase(sceneMgr, window) {}

void Scene::handleEvents() {
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

void Scene::update() {}

void Scene::draw() {
  m_window.clear(sf::Color::Green);
  m_window.display();
}

} // namespace bm::scene::settings
