#ifndef GAME_HPP
#define GAME_HPP

class Game {
public:
  int run();

private:
  void handleEvents();
  void update();
  void draw();

  bool shouldExit{false};
};

#endif