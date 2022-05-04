#ifndef GAMECONTEXT_HPP
#define GAMECONTEXT_HPP

#include <cstdint>

namespace SimpleSnake {

struct GameContext {
  bool shouldExit;
  int32_t exitCode;
};

} // namespace SimpleSnake

#endif