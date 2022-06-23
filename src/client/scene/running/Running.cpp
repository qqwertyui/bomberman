#include "Running.hpp"
#include <iostream>

namespace bomberman::scene {

Running::Running(SceneManager &sceneMgr, sf::RenderWindow &window,
                 const SceneId &sceneId)
    : Scene(sceneMgr, window, sceneId) {}

void Running::handleEvents() {
  sf::Event e;
  while (m_window.pollEvent(e)) {
    if (e.type == sf::Event::Closed) {
      m_window.close();
    } else if (e.type == sf::Event::KeyPressed) {
      if (e.key.code == sf::Keyboard::Escape) {
        change(SceneId::Menu);
      } else {
        std::cout << "Running key pressed!\n";
      }
    }
  }
}

void Running::update() {}

void Running::draw() {
  m_window.clear(sf::Color::Blue);
  m_window.display();
}

} // namespace bomberman::scene