#ifndef GAME_HPP
#define GAME_HPP

#include "GameContext.hpp"

namespace SimpleSnake {

class Game {
public:
  int run();

private:
  void startEventLoop();
  void handleEvents();
  void update();
  void draw();

  GameContext context;
};

} // namespace SimpleSnake

#endif