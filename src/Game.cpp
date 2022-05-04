#include "Game.hpp"
#include "Event.hpp"
#include "Timer.hpp"
#include <iostream>

namespace SimpleSnake {

static constexpr int FPS = 2;

bool Game::initialize() {
  m_window = std::make_unique<Window>();
  if (not m_window->isOk()) {
    return false;
  }
  return true;
}

int Game::run() {
  if (not initialize()) {
    return false;
  }

  Timer timer{1.f / FPS * 1000};
  while (not context.shouldExit) {
    handleEvents();
    update();
    if (timer.nextTick()) {
      draw();
      timer.reset();
    }
  }
  return context.exitCode;
}

void Game::handleEvents() {
  Event e;
  while (m_window->pollEvent(e)) {
    switch (e.type) {
    case Event::Type::KeyPressed: {
      auto key{e.value.keyboard.code};
      if (key == Keyboard::Key::Escape) {
        context.shouldExit = true;
      }
      break;
    }
    default: {
      std::cout << "unhandled type, id = " << static_cast<int>(e.type) << "\n";
    }
    }
  }
}

void Game::update() {}

void Game::draw() { std::cout << "Hello World\n"; }

} // namespace SimpleSnake