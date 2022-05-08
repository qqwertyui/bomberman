#pragma once
#include "GameContext.hpp"
#include "GlobalConfig.hpp"
#include "Window.hpp"
#include <memory>

namespace SimpleSnake {

class Game {
public:
  int run(int argc, char **argv);

private:
  bool initialize(int argc, char **argv);
  void handleEvents();
  void update();
  void draw();

  GameContext m_context;
  std::unique_ptr<GlobalConfig> m_config;
  std::unique_ptr<Window> m_window;
};

} // namespace SimpleSnake