#include "Scene.hpp"
#include "common/ToUnderlying.hpp"
#include "resource/TextureManager.hpp"
#include "scene/SharedData.hpp"
#include <SFML/Graphics.hpp>

namespace bm::scene::menu {
Scene::Scene(SceneManager &sceneMgr)
    : SceneBase(sceneMgr),
      textBox({shared().window.getSize().x / 2.f - 200.f, 100.f}, {400.f, 50.f},
              "Enter text ...", 30, 20) {
  auto &window{shared().window};
  float buttonSpacing{60.f};
  float centerX = window.getSize().x / 2.f;
  float centerY = window.getSize().y / 2.f;
  sf::Vector2f ButtonSize(190.f, 49.f);
  sf::Vector2f startButton(centerX - ButtonSize.x / 2.f,
                           centerY - ButtonSize.y / 2.f - buttonSpacing);
  sf::Vector2f settingsButton(centerX - ButtonSize.x / 2.f,
                              centerY - ButtonSize.y / 2.f);
  sf::Vector2f exitButton(centerX - ButtonSize.x / 2.f,
                          centerY - ButtonSize.y / 2.f + buttonSpacing);
  buttons.emplace(ButtonId::Start, gui::Button(startButton, "Start"));
  buttons.emplace(ButtonId::Settings, gui::Button(settingsButton, "Settings"));
  buttons.emplace(ButtonId::Exit, gui::Button(exitButton, "Exit"));
  buttons.at(ButtonId::Start).setActive(true);
}

void Scene::handleEvents() {
  auto &window{shared().window};
  while (const auto &e = window.pollEvent()) {
    if (e->is<sf::Event::Closed>()) {
      window.close();
    } else if (const auto *keyPressed = e->getIf<sf::Event::KeyPressed>()) {
      handleKeyEvent(keyPressed->scancode);
    } else if (const auto *mouseButton =
                   e->getIf<sf::Event::MouseButtonPressed>()) {
      handleMouseClick(mouseButton->button);
    }
    textBox.handleEvent(*e);
  }
}

void Scene::handleKeyEvent(const sf::Keyboard::Scancode &scancode) {
  keyboardActive = true;
  auto &window{shared().window};
  if (scancode == sf::Keyboard::Scancode::Escape) {
    window.close();
  } else if (scancode == sf::Keyboard::Scancode::Up) {
    if (m_activeButton > ButtonId::Start) {
      buttons.at(m_activeButton).setActive(false);
      m_activeButton =
          static_cast<ButtonId>(common::toUnderlying(m_activeButton) - 1);
      buttons.at(m_activeButton).setActive(true);
      keyboardActive = false;
    }
  } else if (scancode == sf::Keyboard::Scancode::Down) {
    if (m_activeButton < ButtonId::Exit) {
      buttons.at(m_activeButton).setActive(false);
      m_activeButton =
          static_cast<ButtonId>(common::toUnderlying(m_activeButton) + 1);
      buttons.at(m_activeButton).setActive(true);
      keyboardActive = false;
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

void Scene::handleMouseClick(const sf::Mouse::Button &button) {
  keyboardActive = false;
  auto &window{shared().window};
  auto localpos{sf::Mouse::getPosition(window)};
  auto mousePosf{static_cast<sf::Vector2f>(localpos)};

  for (auto &button : buttons) {
    if (button.second.getButtonBounds().contains(mousePosf)) {
      buttons.at(m_activeButton).setActive(false);
      m_activeButton = button.first;
      button.second.setActive(true);
    } else {
      button.second.setActive(false);
    }
  }
  if (button == sf::Mouse::Button::Left) {
    for (auto &button : buttons) {
      if (button.second.getButtonBounds().contains(mousePosf)) {
        if (button.first == ButtonId::Exit) {
          window.close();
        } else if (button.first == ButtonId::Settings) {
          change(SceneId::Settings);
        } else if (button.first == ButtonId::Start) {
          change(SceneId::Lobby);
        }
      }
    }
  }
}

void Scene::update() {
  if (keyboardActive) {
    return;
  }
  auto &window{shared().window};
  auto localpos = sf::Mouse::getPosition(window);
  auto mousePosf{static_cast<sf::Vector2f>(localpos)};

  for (auto &button : buttons) {
    if (button.second.getButtonBounds().contains(mousePosf)) {
      if (m_activeButton != button.first) {
        buttons.at(m_activeButton).setActive(false);
        m_activeButton = button.first;
        button.second.setActive(true);
      }
      return;
    }
  }
}

void Scene::draw() {
  auto &window{shared().window};
  window.clear(sf::Color::Red);
  for (auto &button : buttons) {
    window.draw(button.second);
  }
  window.draw(textBox);
}
} // namespace bm::scene::menu
