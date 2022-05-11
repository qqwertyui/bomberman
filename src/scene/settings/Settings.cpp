#include "Settings.hpp"
#include <iostream>

namespace SimpleSnake::scene {

Settings::Settings(SceneManager &sceneMgr, sf::RenderWindow &window,
                   const SceneId &sceneId)
    : Scene(sceneMgr, window, sceneId) {}

void Settings::handleEvents() {
  sf::Event e;
  while (m_window.pollEvent(e)) {
    if (e.type == sf::Event::Closed) {
      m_window.close();
    } else if (e.type == sf::Event::KeyPressed) {
      if (e.key.code == sf::Keyboard::Escape) {
        change(SceneId::Menu);
      } else {
        std::cout << "Settings key pressed!\n";
      }
    }
  }
}

void Settings::update() {}

void Settings::draw() {
  m_window.clear(sf::Color::Green);
  m_window.display();
}

} // namespace SimpleSnake::scene