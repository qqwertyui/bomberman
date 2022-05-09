#include "Menu.hpp"
#include <iostream>

namespace SimpleSnake::scene {

Menu::Menu(SceneManager &sceneMgr, sf::RenderWindow &window,
           const SceneId &sceneId)
    : Scene(sceneMgr, window, sceneId) {}

void Menu::handleEvents() {
  sf::Event e;
  while (m_window.pollEvent(e)) {
    if (e.type == sf::Event::Closed) {
      m_window.close();
    } else if (e.type == sf::Event::KeyPressed) {
      if (e.key.code == sf::Keyboard::Enter) {
        change(SceneId::Running);
      } else {
        std::cout << "Menu key pressed!\n";
      }
    }
  }
}

void Menu::update() {}

void Menu::draw() {
  m_window.clear(sf::Color::Red);
  m_window.display();
}

} // namespace SimpleSnake::scene