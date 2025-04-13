#include "Scene.hpp"
#include "common/Log.hpp"
#include <SFML/Graphics.hpp>

namespace bm::scene::settings {
Scene::Scene(SceneManager &sceneMgr) : SceneBase(sceneMgr) {}

void Scene::handleEvents() {
  auto &window{getWindow()};

  while (const auto &e = window.pollEvent()) {
    if (e->is<sf::Event::Closed>()) {
      window.close();
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
  auto &window{getWindow()};

  window.clear(sf::Color::Green);
}

} // namespace bm::scene::settings
