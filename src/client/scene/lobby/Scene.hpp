#pragma once

#include "gui/WidgetManager.hpp"
#include "scene/SceneBase.hpp"
#include <vector>

namespace bm::scene::lobby {
class Scene : public SceneBase {
public:
  Scene(SceneManager &sceneMgr);

  void handleEvents(const sf::Event &e) override;
  void update() override;
  void draw() override;
  void onEntry() override;

private:
  using LobbyData = std::vector<std::pair<int, int>>;

  void createLobbyButton(const LobbyData &lobbyData);
  void handleKeyEvent(const sf::Keyboard::Scancode &scancode);
  bool joinLobby(unsigned int lobbyId);
  LobbyData fetchLobbyInfo();

  LobbyData lobbyData;
  gui::WidgetManager widgetManager;
};
} // namespace bm::scene::lobby
