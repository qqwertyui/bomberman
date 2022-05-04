#pragma once
#include "GameContext.hpp"
#include "Window.hpp"
#include <memory>

namespace SimpleSnake {

class Game {
public:
  int run();

private:
  bool initialize();
  void handleEvents();
  void update();
  void draw();

  GameContext context;
  std::unique_ptr<Window> m_window;
};

} // namespace SimpleSnake