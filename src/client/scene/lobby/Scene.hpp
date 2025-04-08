#pragma once
#include "common/ConnectionManager.hpp"
#include "gui/Button.hpp"
#include "scene/SceneBase.hpp"
#include <map>
#include <vector>

namespace bm::scene::lobby {
class Scene : public SceneBase {
public:
  Scene(SceneManager &sceneMgr);

  void handleEvents() override;
  void update() override;
  void draw() override;
  void onEntry() override;
  void onLeave() override;

private:
  void createLobbyButton(const std::vector<std::pair<int, int>> &lobbyData);
  void handleMouseEvent(const sf::Mouse::Button &button);
  void handleKeyEvent(const sf::Keyboard::Scancode &scancode);
  common::ConnectionManager connMgr;
  std::vector<std::pair<int, int>> lobbyData;
  std::map<int, gui::Button> lobbyButtons;
  bool keyboardActive{true};
  int activeLobbyButton{0};
  int buttonInColumn{4};
  int x, y;
};
} // namespace bm::scene::lobby
