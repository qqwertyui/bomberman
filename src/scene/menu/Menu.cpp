#include "Menu.hpp"
#include "rsrcManagement/TextureManager.hpp"
#include <iostream>

namespace SimpleSnake::scene {

Menu::Menu(SceneManager &sceneMgr, sf::RenderWindow &window,
           const SceneId &sceneId)
    : Scene(sceneMgr, window, sceneId) {
  buttons.emplace(ButtonId::Start, interface::Button("Start"));
  buttons.emplace(ButtonId::Settings, interface::Button("Settings"));
  buttons.emplace(ButtonId::Exit, interface::Button("Exit"));

  buttons[m_activeButton].setActive(true);

  sf::Vector2f buttonPosition{};
  buttons[ButtonId::Start].setPosition(buttonPosition);
  buttonPosition.y += 60;
  buttons[ButtonId::Settings].setPosition(buttonPosition);
  buttonPosition.y += 60;
  buttons[ButtonId::Exit].setPosition(buttonPosition);
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
  for (auto &button : buttons) {
    m_window.draw(button.second);
  }
  m_window.display();
}

void Menu::handleKeyEvent(const sf::Event::KeyEvent &keyEvent) {
  if (keyEvent.code == sf::Keyboard::Escape) {
    m_window.close();
  } else if (keyEvent.code == sf::Keyboard::Up) {
    if (m_activeButton < ButtonId::Start) {
      buttons[m_activeButton].setActive(false);
      m_activeButton =
          static_cast<ButtonId>(static_cast<unsigned int>(m_activeButton) + 1);
      buttons[m_activeButton].setActive(true);
    }
  } else if (keyEvent.code == sf::Keyboard::Down) {
    if (m_activeButton > ButtonId::Exit) {
      buttons[m_activeButton].setActive(false);
      m_activeButton =
          static_cast<ButtonId>(static_cast<unsigned int>(m_activeButton) - 1);
      buttons[m_activeButton].setActive(true);
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