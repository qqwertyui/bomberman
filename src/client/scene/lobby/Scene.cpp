#include "Scene.hpp"
#include "GlobalConfig.hpp"
#include "common/ConnectionManager.hpp"
#include "common/Log.hpp"
#include "common/itf/core.pb.h"
#include <SFML/Graphics.hpp>

namespace bm::scene::lobby {
Scene::Scene(SceneManager &sceneMgr) : SceneBase(sceneMgr) {}

void Scene::handleEvents() {
  auto &window{getWindow()};
  while (const auto &e = window.pollEvent()) {
    if (e->is<sf::Event::Closed>()) {
      window.close();
    } else if (const auto *keyPressed = e->getIf<sf::Event::KeyPressed>()) {
      handleKeyEvent(keyPressed->scancode);
    } else if (const auto *mouseButton =
                   e->getIf<sf::Event::MouseButtonPressed>()) {
      handleMouseEvent(mouseButton->button);
    }
  }
}

void Scene::onEntry() {
  connMgr.connect(GlobalConfig::get().serverIp(),
                  GlobalConfig::get().serverPort());
  common::itf::C2SMessage req;
  auto *query = req.mutable_query();
  query->set_lobbies(true);
  if (not connMgr.send(req)) {
    LOG_ERR("Couldn't connect to server");
    connMgr.disconnect();
    return;
  }

  auto resp = connMgr.receive<common::itf::S2CMessage>();
  if (not resp.has_value()) {
    LOG_ERR("Couldn't connect to server");
    connMgr.disconnect();
    return;
  }

  if (not resp->has_query()) {
    LOG_INF("No active lobbies");
    return;
  }

  lobbyData.clear();
  for (const auto &lobby : resp->query().lobbies()) {
    lobbyData.emplace_back(lobby.connectedplayers(), lobby.maxplayers());
  }
  createLobbyButton(lobbyData);
}

void Scene::handleMouseEvent(const sf::Mouse::Button &button) {
  keyboardActive = false;
  auto &window{getWindow()};
  auto localPos = sf::Mouse::getPosition(window);
  sf::Vector2f mousePos(static_cast<float>(localPos.x),
                        static_cast<float>(localPos.y));
  if (button == sf::Mouse::Button::Left) {
    for (const auto &[index, lobbyButton] : lobbyButtons) {
      if (lobbyButton.getButtonBounds().contains(mousePos)) {
        change(SceneId::Game);
      }
    }
  }
}

void Scene::handleKeyEvent(const sf::Keyboard::Scancode &scancode) {
  if (lobbyButtons.empty()) {
    return;
  }
  keyboardActive = true;
  int totalLobbiesButtons = static_cast<int>(lobbyButtons.size());

  lobbyButtons.at(activeLobbyButton).setActive(false);
  if (scancode == sf::Keyboard::Scancode::Up) {
    if (activeLobbyButton % buttonInColumn > 0) {
      activeLobbyButton--;
    }
  } else if (scancode == sf::Keyboard::Scancode::Down) {
    if (activeLobbyButton % buttonInColumn < buttonInColumn - 1 &&
        activeLobbyButton + 1 < totalLobbiesButtons) {
      activeLobbyButton++;
    }
  } else if (scancode == sf::Keyboard::Scancode::Left) {
    if (activeLobbyButton >= buttonInColumn) {
      activeLobbyButton -= buttonInColumn;
    }
  } else if (scancode == sf::Keyboard::Scancode::Right) {
    if (activeLobbyButton + buttonInColumn < totalLobbiesButtons) {
      activeLobbyButton += buttonInColumn;
    }
  } else if (scancode == sf::Keyboard::Scancode::Enter) {
    change(SceneId::Game);
  } else if (scancode == sf::Keyboard::Scancode::Escape) {
    change(SceneId::Menu);
  } else {
    LOG_DBG("Running key pressed!");
  }
  lobbyButtons.at(activeLobbyButton).setActive(true);
}

void Scene::createLobbyButton(
    const std::vector<std::pair<int, int>> &lobbyData) {
  auto &window{getWindow()};
  float buttonSpacing{20.0f};
  sf::Vector2f buttonSize{190.0f, 49.0f};
  float centerY = window.getSize().y / 2.0f;
  float totalColumnHeight =
      buttonInColumn * buttonSize.y + (buttonInColumn - 1) * buttonSpacing;
  float startX = buttonSpacing;
  float startY = centerY - totalColumnHeight / 2.0f;
  lobbyButtons.clear();
  for (unsigned int i = 0; i < lobbyData.size(); i++) {
    const auto &[connected, max] = lobbyData[i];
    std::string label =
        std::to_string(connected) + "/" + std::to_string(max) + " players";
    int column = i / buttonInColumn;
    int row = i % buttonInColumn;
    sf::Vector2f buttonPos(startX + column * (buttonSize.x + buttonSpacing),
                           startY + row * (buttonSize.y + buttonSpacing));
    lobbyButtons.emplace(i, gui::Button(buttonPos, label));
  }
  if (!lobbyButtons.empty()) {
    activeLobbyButton = 0;
    lobbyButtons.at(activeLobbyButton).setActive(true);
  }
}

void Scene::onLeave() { connMgr.disconnect(); }

void Scene::update() {
  if (keyboardActive) {
    return;
  }
  auto &window = getWindow();
  auto localPos = sf::Mouse::getPosition(window);
  sf::Vector2f mousePos(static_cast<float>(localPos.x),
                        static_cast<float>(localPos.y));
  for (auto &[index, lobbyButton] : lobbyButtons) {
    if (lobbyButton.getButtonBounds().contains(mousePos)) {
      lobbyButton.setActive(true);
    } else {
      lobbyButton.setActive(false);
    }
  }
}

void Scene::draw() {
  auto &window{getWindow()};
  window.clear(sf::Color::Blue);
  for (const auto &[index, lobbyButton] : lobbyButtons) {
    window.draw(lobbyButton);
  }
}
} // namespace bm::scene::lobby
