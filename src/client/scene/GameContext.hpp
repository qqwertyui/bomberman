#pragma once

#include <map>
#include <memory>
#include <optional>

namespace bm::scene {
namespace game {
class PlayerModel;
}

struct LobbyInfo {
  unsigned int id{};
  unsigned int size{};
};

struct Player {
  unsigned int id{};
  game::PlayerModel *model{};
};

struct GameContext {
  std::optional<LobbyInfo> lobby;
  std::optional<Player> player;
  std::map<int, game::PlayerModel *> enemies;

  inline void reset() {
    lobby.reset();
    player.reset();
    enemies.clear();
  }
};
} // namespace bm::scene
