#pragma once

#include <optional>
#include <utility>
#include <vector>

namespace bm::common {
class ConnectionInfo;
}

namespace bm {
class Lobby;
class Client;

class Database {
public:
  static bool create(unsigned int maxLobbies, unsigned int maxPlayers);
  static Database &get();

  Lobby &getLobbyById(unsigned int id);
  unsigned int getNumberOfLobbies() const;

  Client *getPlayerById(unsigned int id);
  Client *addPlayer(const common::ConnectionInfo &connection);
  void removePlayer(unsigned int id);

  bool playerLimitReached() const;

private:
  Database(unsigned int maxLobbies, unsigned int maxPlayers);
  inline static Database *instance;

  std::optional<unsigned int> findFreePlayerEntryId();

  std::vector<Lobby> lobbies;
  std::vector<std::pair<Client, bool>> players;

  unsigned int connectedCounter{};
  const unsigned int maxPlayers;
  const unsigned int maxLobbies;
};

} // namespace bm
