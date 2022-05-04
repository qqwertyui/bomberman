#include "Game.hpp"
#include <chrono>
#include <iostream>
#include <thread>
#include <windows.h>

namespace SimpleSnake {

static constexpr int FPS = 2;

void eventLoopHandler() {
  while (1) {
    // Collect and save events (pressed keys, window events, etc.) in queue for
    // use in handleEvents
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void Game::startEventLoop() { std::thread{eventLoopHandler}.detach(); }

int Game::run() {
  startEventLoop();
  std::chrono::time_point<std::chrono::high_resolution_clock> start{
      std::chrono::high_resolution_clock::now()};
  constexpr auto fpsIntervalMs{
      static_cast<int>(1.f / static_cast<double>(FPS) * 1000.f)};

  while (not context.shouldExit) {
    handleEvents();
    update();
    auto now = std::chrono::high_resolution_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start)
            .count() > fpsIntervalMs) {
      draw();
      start = now;
    }
  }
  return context.exitCode;
}

void Game::handleEvents() {
  if (false) {
    context.shouldExit = true;
  }
}

void Game::update() {
  //
}

void Game::draw() { std::cout << "press ctrl+c to stop\n"; }

} // namespace SimpleSnake