#include "Scene.hpp"
#include "GlobalConfig.hpp"
#include "common/itf/core.pb.h"
#include "common/logging/Log.hpp"
#include "gui/Button.hpp"
#include "scene/SharedData.hpp"
#include <SFML/Graphics.hpp>

namespace bm::scene::lobby {
Scene::Scene(SceneManager &sceneMgr)
    : SceneBase(sceneMgr), widgetManager(shared().window) {}

void Scene::handleEvents(const sf::Event &e) {
  widgetManager.handleEvents(e);
  if (const auto *keyPressed = e.getIf<sf::Event::KeyPressed>()) {
    handleKeyEvent(keyPressed->scancode);
  }
}

void Scene::onEntry() {
  widgetManager.reset();

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

void Scene::handleKeyEvent(const sf::Keyboard::Scancode &scancode) {
  if (scancode == sf::Keyboard::Scancode::Escape) {
    change(SceneId::Menu);
  }
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

  constexpr int buttonInColumn{4};
  float totalColumnHeight =
      buttonInColumn * buttonSize.y + (buttonInColumn - 1) * buttonSpacing;
  float startX = buttonSpacing;
  float startY = centerY - totalColumnHeight / 2.0f;
  widgetManager.clear();

  for (unsigned int i = 0; i < lobbyData.size(); i++) {
    const auto &[connected, max] = lobbyData[i];
    std::string label =
        std::to_string(connected) + "/" + std::to_string(max) + " players";
    int column = i / buttonInColumn;
    int row = i % buttonInColumn;
    sf::Vector2f buttonPos(startX + column * (buttonSize.x + buttonSpacing),
                           startY + row * (buttonSize.y + buttonSpacing));

    widgetManager.add(new gui::Button(buttonPos, label, [this, i]() {
      auto success = joinLobby(i);
      if (not success) {
        LOG_ERR("Couldn't join the lobby %u", i);
      }
    }));
  }
}

void Scene::update() { widgetManager.update(); }

void Scene::draw() {
  auto &window{shared().window};
  window.clear(sf::Color::Blue);
  widgetManager.draw();
}
} // namespace bm::scene::lobby
