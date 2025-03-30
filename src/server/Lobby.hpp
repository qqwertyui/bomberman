#pragma once

#include <vector>

namespace bm {
class Client;

class Lobby {
public:
  Lobby(unsigned int id);

  unsigned int getId() const;
  const std::vector<Client *> &getMembers() const;
  unsigned int getMaxSize() const;

private:
  const unsigned int id;
  const unsigned int maxSize;

  std::vector<Client *> players;
};
} // namespace bm
