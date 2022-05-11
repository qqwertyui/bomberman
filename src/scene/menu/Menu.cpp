#include "Menu.hpp"
#include "rsrcManagement/TextureManager.hpp"
#include <iostream>

namespace SimpleSnake::scene {

Menu::Menu(SceneManager &sceneMgr, sf::RenderWindow &window,
           const SceneId &sceneId)
    : Scene(sceneMgr, window, sceneId) {
  buttons.emplace_back(new interface::Button("Start"));
  buttons.emplace_back(new interface::Button("Settings"));
  buttons.emplace_back(new interface::Button("Exit"));
}

void Menu::handleEvents() {
  sf::Event e;
  while (m_window.pollEvent(e)) {
    if (e.type == sf::Event::Closed) {
      m_window.close();
    } else if (e.type == sf::Event::KeyPressed) {
      handleKeyEvent(e.key);
    }
  }
}

void Menu::update() {}

void Menu::draw() {
  m_window.clear(sf::Color::Red);
  m_window.draw(*buttons[0]); // temp
  m_window.display();
}

void Menu::handleKeyEvent(const sf::Event::KeyEvent &keyEvent) {
  if (keyEvent.code == sf::Keyboard::Escape) {
    m_window.close();
  } else if (keyEvent.code == sf::Keyboard::Up) {
    if (m_activeButton == ButtonId::Start) {
      m_activeButton = ButtonId::Exit;
    } else {
      m_activeButton =
          static_cast<ButtonId>(static_cast<unsigned int>(m_activeButton) + 1);
    }
  } else if (keyEvent.code == sf::Keyboard::Down) {
    if (m_activeButton == ButtonId::Exit) {
      m_activeButton = ButtonId::Start;
    } else {
      m_activeButton =
          static_cast<ButtonId>(static_cast<unsigned int>(m_activeButton) - 1);
    }
  } else if (keyEvent.code == sf::Keyboard::Enter) {
    if (m_activeButton == ButtonId::Exit) {
      m_window.close();
    } else if (m_activeButton == ButtonId::Settings) {
      change(SceneId::Settings);
    } else if (m_activeButton == ButtonId::Start) {
      change(SceneId::Running);
    }
  }
  std::cout << static_cast<unsigned int>(m_activeButton) << "\n";
}

} // namespace SimpleSnake::scene