#include "Game.hpp"
#include "ConfigLoader.hpp"
#include "Event.hpp"
#include "Timer.hpp"
#include "utils/Log.hpp"
#include <iostream>

namespace SimpleSnake {

bool Game::initialize(int argc, char **argv) {
  m_config = ConfigLoader::loadConfig(argc, argv);
  LOG("Loaded configuration: %s", m_config.asString().c_str());

  m_window = Window::create();
  if (not m_window) {
    return false;
  }
  return true;
}

int Game::run(int argc, char **argv) {
  if (not initialize(argc, argv)) {
    LOG("Initalization failed");
    return 1;
  }

  Timer timer{1.f / m_config.maxFps() * 1000};
  while (not m_context.shouldExit) {
    handleEvents();
    update();
    if (timer.nextTick()) {
      draw();
      timer.reset();
    }
  }
  return m_context.exitCode;
}

void Game::handleEvents() {
  Event e;
  while (m_window->pollEvent(e)) {
    switch (e.type) {
    case Event::Type::KeyPressed: {
      auto key{e.value.keyboard.key};
      if (key == Keyboard::Key::Escape) {
        m_context.shouldExit = true;
      } else if (key == Keyboard::Key::A) {
        std::cout << "A";
      }
      break;
    }
    case Event::Type::MouseMoved: {
      auto pos{e.value.mouse.pos};
      std::cout << "Move is at (x=" << pos.x << ", y=" << pos.y << ")\n";
      break;
    }
    default: {
      LOG("Unhandled event, id = %d", static_cast<int>(e.type));
    }
    }
  }
}

void Game::update() {}

void Game::draw() { std::cout << "Hello World\n"; }

} // namespace SimpleSnake