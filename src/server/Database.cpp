#include "Database.hpp"

#include "Client.hpp"
#include "Lobby.hpp"
#include "common/ConnectionInfo.hpp"
#include "common/Log.hpp"
#include "common/Utils.hpp"
#include <cassert>
#include <mutex>

namespace bm {
bool Database::create(unsigned int maxLobbies, unsigned int maxPlayers) {
  instance = new Database(maxLobbies, maxPlayers);
  return true;
}

Database &Database::get() {
  assert(instance != nullptr);
  return *instance;
}

Database::Database(unsigned int maxLobbies, unsigned int maxPlayers)
    : maxPlayers{maxPlayers}, maxLobbies{maxLobbies} {
  lobbies.reserve(maxLobbies);
  for (unsigned int i = 0; i < maxLobbies; i++) {
    lobbies.emplace_back(i);
  }

  players.reserve(maxPlayers);
  for (unsigned int i = 0; i < maxPlayers; i++) {
    players.emplace_back(std::make_pair(Client{{}, i}, false));
  }
}

Lobby &Database::getLobbyById(unsigned int id) {
  assert(common::inRange(id, 0u, maxLobbies - 1));
  return lobbies[id];
}

unsigned int Database::getNumberOfLobbies() const { return lobbies.size(); }

Client *Database::getPlayerById(unsigned int id) {
  std::lock_guard<std::mutex> guard(mtx);
  assert(common::inRange(id, 0u, maxPlayers - 1));

  auto &player = players[id];
  const auto isAllocated{player.second};
  if (not isAllocated) {
    return nullptr;
  }
  return &players[id].first;
}

std::optional<unsigned int> Database::findFreePlayerEntryId() {
  for (unsigned int i = 0; i < maxPlayers; i++) {
    auto &player = players[i];
    const auto isAllocated{player.second};
    if (not isAllocated) {
      return i;
    }
  }
  return std::nullopt;
}

Client *Database::addPlayer(const common::ConnectionInfo &connection) {
  std::lock_guard<std::mutex> guard(mtx);
  if (connectedCounter >= maxPlayers) {
    return nullptr;
  }
  auto id = findFreePlayerEntryId();
  if (not id) {
    return nullptr;
  }
  auto &player = players[*id];
  player.second = true;
  player.first = {connection, *id};
  connectedCounter++;
  return &player.first;
}

void Database::removePlayer(unsigned int id) {
  std::lock_guard<std::mutex> guard(mtx);
  assert(common::inRange(id, 0u, maxPlayers - 1));

  auto &player = players[id];
  player.second = false;

  connectedCounter--;
  return;
}

bool Database::playerLimitReached() const {
  return (connectedCounter >= maxPlayers);
}

} // namespace bm
