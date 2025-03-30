#include "Menu.hpp"
#include "rsrcManagement/TextureManager.hpp"
#include <iostream>

namespace bm::scene {

Menu::Menu(SceneManager &sceneMgr, sf::RenderWindow &window,
           const SceneId &sceneId)
    : Scene(sceneMgr, window, sceneId) {
  buttons.emplace(ButtonId::Start,
                  interface::Button(sf::Vector2f(100, 200), "Start"));
  buttons.emplace(ButtonId::Settings,
                  interface::Button(sf::Vector2f(0, 60), "Settings"));
  buttons.emplace(ButtonId::Exit,
                  interface::Button(sf::Vector2f(0, 120), "Exit"));

  buttons.at(ButtonId::Start).setActive(true);
}

void Menu::handleEvents() {
  while (const auto &e = m_window.pollEvent()) {
    if (e->is<sf::Event::Closed>()) {
      m_window.close();
    } else if (const auto *keyPressed = e->getIf<sf::Event::KeyPressed>()) {
      handleKeyEvent(keyPressed->scancode);
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

void Menu::handleKeyEvent(const sf::Keyboard::Scancode &scancode) {
  if (scancode == sf::Keyboard::Scancode::Escape) {
    m_window.close();
  } else if (scancode == sf::Keyboard::Scancode::Up) {
    if (m_activeButton < ButtonId::Start) {
      buttons.at(m_activeButton).setActive(false);
      m_activeButton =
          static_cast<ButtonId>(static_cast<unsigned int>(m_activeButton) + 1);
      buttons.at(m_activeButton).setActive(true);
    }
  } else if (scancode == sf::Keyboard::Scancode::Down) {
    if (m_activeButton > ButtonId::Exit) {
      buttons.at(m_activeButton).setActive(false);
      m_activeButton =
          static_cast<ButtonId>(static_cast<unsigned int>(m_activeButton) - 1);
      buttons.at(m_activeButton).setActive(true);
    }
  } else if (scancode == sf::Keyboard::Scancode::Enter) {
    if (m_activeButton == ButtonId::Exit) {
      m_window.close();
    } else if (m_activeButton == ButtonId::Settings) {
      change(SceneId::Settings);
    } else if (m_activeButton == ButtonId::Start) {
      change(SceneId::Running);
    }
  }
}

} // namespace bm::scene