#include "Lobby.hpp"
#include "GlobalConfig.hpp"

namespace bm {
Lobby::Lobby(unsigned int id)
    : id(id), maxSize(GlobalConfig::get().maxLobbySize()) {}

unsigned int Lobby::getId() const { return id; }

const std::vector<Client *> &Lobby::getMembers() const { return players; }

unsigned int Lobby::getMaxSize() const { return maxSize; }
} // namespace bm
