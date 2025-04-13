#include "Lobby.hpp"
#include "GlobalConfig.hpp"
#include <algorithm>

namespace bm {
Lobby::Lobby(unsigned int id)
    : id(id), maxSize(GlobalConfig::get().maxLobbySize()) {
  members.reserve(maxSize);
}

unsigned int Lobby::getId() const { return id; }

std::vector<int> Lobby::getMembersIds() const { return members; }

bool Lobby::add(int playerId) {
  if (isFull()) {
    return false;
  }
  members.push_back(playerId);
  return true;
}

bool Lobby::remove(int playerId) {
  if (isEmpty()) {
    return false;
  }
  auto it = std::find(members.begin(), members.end(), playerId);
  if (it == members.end()) {
    return false;
  }
  members.erase(it);
  return true;
}

bool Lobby::isFull() const { return (members.size() >= maxSize); }

bool Lobby::isEmpty() const { return (members.size() == 0); };

unsigned int Lobby::getMaxSize() const { return maxSize; }
} // namespace bm
