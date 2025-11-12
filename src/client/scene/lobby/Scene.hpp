#pragma once

#include "gui/Button.hpp"
#include "scene/SceneBase.hpp"
#include <map>
#include <vector>

namespace bm::scene::lobby {
class Scene : public SceneBase {
public:
  Scene(SceneManager &sceneMgr);

  void handleEvents(const sf::Event &e) override;
  void update() override;
  void draw() override;
  void onEntry() override;
  void onLeave() override;

private:
  using LobbyData = std::vector<std::pair<int, int>>;
  using ButtonMap = std::map<int, gui::Button>;

  void createLobbyButton(const LobbyData &lobbyData);
  void handleMouseClick(const sf::Mouse::Button &button);
  void handleKeyEvent(const sf::Keyboard::Scancode &scancode);
  std::optional<unsigned int>
  getSelectedButtonId(const sf::Vector2f &mousePos) const;
  bool joinLobby(unsigned int lobbyId);
  LobbyData fetchLobbyInfo();

  LobbyData lobbyData;
  ButtonMap lobbyButtons;

  bool keyboardActive{true};
  int activeLobbyButton{0};
  const int buttonInColumn{4};
};
} // namespace bm::scene::lobby
