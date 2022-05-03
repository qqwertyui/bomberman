#include "Game.hpp"
#include <iostream>
#include <windows.h> // temp

int Game::run() {
  while (not shouldExit) {
    Sleep(500); // temp
    handleEvents();
    update();
    draw();
  }
  return 0;
}

void Game::handleEvents() {
  if (false) {
    shouldExit = true;
  }
}

void Game::update() {
  //
}

void Game::draw() { std::cout << "press ctrl+c to stop\n"; }