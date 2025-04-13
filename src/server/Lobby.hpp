#pragma once

#include <vector>

namespace bm {
class Lobby {
public:
  Lobby(unsigned int id);

  unsigned int getId() const;
  bool add(int playerId);
  bool remove(int playerId);
  std::vector<int> getMembersIds() const;

  bool isFull() const;
  bool isEmpty() const;
  unsigned int getMaxSize() const;

private:
  const unsigned int id;
  const unsigned int maxSize;

  std::vector<int> members;
};
} // namespace bm
