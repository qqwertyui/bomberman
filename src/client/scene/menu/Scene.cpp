#include "Scene.hpp"
#include "resource/TextureManager.hpp"
#include <SFML/Graphics.hpp>

namespace bm::scene::menu {
Scene::Scene(SceneManager &sceneMgr) : SceneBase(sceneMgr) {
  buttons.emplace(ButtonId::Start,
                  interface::Button(sf::Vector2f(100, 200), "Start"));
  buttons.emplace(ButtonId::Settings,
                  interface::Button(sf::Vector2f(0, 60), "Settings"));
  buttons.emplace(ButtonId::Exit,
                  interface::Button(sf::Vector2f(0, 120), "Exit"));

  buttons.at(ButtonId::Start).setActive(true);
}

void Scene::handleEvents() {
  auto &window{getWindow()};

  while (const auto &e = window.pollEvent()) {
    if (e->is<sf::Event::Closed>()) {
      window.close();
    } else if (const auto *keyPressed = e->getIf<sf::Event::KeyPressed>()) {
      handleKeyEvent(keyPressed->scancode);
    }
  }
}

void Scene::update() {}

void Scene::draw() {
  auto &window{getWindow()};

  window.clear(sf::Color::Red);
  for (auto &button : buttons) {
    window.draw(button.second);
  }
  window.display();
}

void Scene::handleKeyEvent(const sf::Keyboard::Scancode &scancode) {
  auto &window{getWindow()};

  if (scancode == sf::Keyboard::Scancode::Escape) {
    window.close();
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
      window.close();
    } else if (m_activeButton == ButtonId::Settings) {
      change(SceneId::Settings);
    } else if (m_activeButton == ButtonId::Start) {
      change(SceneId::Lobby);
    }
  }
}

} // namespace bm::scene::menu