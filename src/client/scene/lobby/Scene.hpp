#pragma once
#include "common/ConnectionManager.hpp"
#include "interface/Button.hpp"
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
  common::ConnectionManager connMgr;
  std::vector<std::pair<int, int>> lobbyData;
  std::map<int, interface::Button> lobbyButtons;
  int x, y;
};
} // namespace bm::scene::lobby
