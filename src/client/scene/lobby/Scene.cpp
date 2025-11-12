#include "Scene.hpp"
#include "GlobalConfig.hpp"
#include "common/itf/core.pb.h"
#include "common/logging/Log.hpp"
#include "scene/SharedData.hpp"
#include <SFML/Graphics.hpp>

namespace bm::scene::lobby {
Scene::Scene(SceneManager &sceneMgr) : SceneBase(sceneMgr) {}

void Scene::handleEvents(const sf::Event &e) {
  if (const auto *keyPressed = e.getIf<sf::Event::KeyPressed>()) {
    handleKeyEvent(keyPressed->scancode);
  } else if (const auto *mouseButton =
                 e.getIf<sf::Event::MouseButtonPressed>()) {
    handleMouseClick(mouseButton->button);
  }
}

void Scene::onEntry() {
  auto &connMgr{shared().connMgr};
  connMgr.connect(GlobalConfig::get().serverIp(),
                  GlobalConfig::get().serverPort());
  if (not connMgr.isConnected()) {
    LOG_ERR("Couldn't connect to server");
    return;
  }
  auto info = fetchLobbyInfo();
  if (info.empty()) {
    return;
  }
  createLobbyButton(info);
}

Scene::LobbyData Scene::fetchLobbyInfo() {
  common::itf::C2SMessage req;
  req.mutable_query()->set_lobbies(true);

  auto &connMgr{shared().connMgr};
  if (not connMgr.send(req)) {
    connMgr.disconnect();
    return {};
  }
  auto resp = connMgr.receive<common::itf::S2CMessage>();
  if (not resp.has_value()) {
    LOG_ERR("Couldn't connect to server");
    return {};
  }
  if (not resp->has_query()) {
    LOG_INF("No active lobbies");
    return {};
  }

  Scene::LobbyData lobbyData;
  for (const auto &lobby : resp->query().lobbies()) {
    lobbyData.emplace_back(lobby.connectedplayers(), lobby.maxplayers());
  }
  return lobbyData;
}

void Scene::handleMouseClick(const sf::Mouse::Button &button) {
  keyboardActive = false;
  auto &window{shared().window};
  auto localPos{sf::Mouse::getPosition(window)};
  auto mousePos{static_cast<sf::Vector2f>(localPos)};

  if (button == sf::Mouse::Button::Left) {
    auto buttonId = getSelectedButtonId(mousePos);
    if (not buttonId) {
      return;
    }
    auto lobbyId = buttonId.value();
    if (not joinLobby(lobbyId)) {
      return;
    }
  }
}

void Scene::handleKeyEvent(const sf::Keyboard::Scancode &scancode) {
  if (lobbyButtons.empty()) {
    return;
  }
  keyboardActive = true;
  auto totalLobbiesButtons = static_cast<int>(lobbyButtons.size());

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
    const auto lobbyId{static_cast<unsigned int>(activeLobbyButton)};
    auto success = joinLobby(lobbyId);
    if (not success) {
      LOG_ERR("Couldn't join the lobby %u", lobbyId);
    }
  } else if (scancode == sf::Keyboard::Scancode::Escape) {
    change(SceneId::Menu);
  } else {
    LOG_DBG("Running key pressed!");
  }
  lobbyButtons.at(activeLobbyButton).setActive(true);
}

std::optional<unsigned int>
Scene::getSelectedButtonId(const sf::Vector2f &mousePos) const {
  for (const auto &element : lobbyButtons) {
    auto &button{element.second};
    if (button.getButtonBounds().contains(mousePos)) {
      return element.first;
    }
  }
  return std::nullopt;
}

bool Scene::joinLobby(unsigned int lobbyId) {
  auto &connMgr{shared().connMgr};

  common::itf::C2SMessage req;
  auto *targetLobby = req.mutable_update()->mutable_lobby();
  targetLobby->set_id(lobbyId);
  targetLobby->set_action(common::itf::UpdateLobbyReq::ENTER);

  if (not connMgr.send(req)) {
    return false;
  }
  auto resp = connMgr.receive<common::itf::S2CMessage>();
  if (not resp.has_value()) {
    LOG_ERR("Couldn't connect to server");
    return false;
  }
  if (not resp->has_update() and resp->update().has_lobby()) {
    LOG_ERR("Received invalid response from the server");
    return false;
  }

  const auto &status{resp->update().lobby().status()};
  if (status != common::itf::UpdateLobbyResp::OK) {
    LOG_INF("Couldn't join lobby, cause: %s",
            common::itf::UpdateLobbyResp::Status_Name(status).c_str());
    return false;
  }

  change(SceneId::Game);
  return true;
}

void Scene::createLobbyButton(const Scene::LobbyData &lobbyData) {
  auto &window{shared().window};
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

void Scene::onLeave() {}

void Scene::update() {
  if (keyboardActive) {
    return;
  }
  auto &window = shared().window;
  auto localPos = sf::Mouse::getPosition(window);
  auto mousePos{static_cast<sf::Vector2f>(localPos)};

  for (auto &[index, lobbyButton] : lobbyButtons) {
    if (lobbyButton.getButtonBounds().contains(mousePos)) {
      lobbyButton.setActive(true);
    } else {
      lobbyButton.setActive(false);
    }
  }
}

void Scene::draw() {
  auto &window{shared().window};
  window.clear(sf::Color::Blue);
  for (const auto &[index, lobbyButton] : lobbyButtons) {
    window.draw(lobbyButton);
  }
}
} // namespace bm::scene::lobby
