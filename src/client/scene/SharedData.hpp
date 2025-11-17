#pragma once

#include "GameContext.hpp"
#include "common/ConnectionManager.hpp"
#include <SFML/Graphics.hpp>

namespace bm::scene {
class SharedData {
public:
  SharedData(sf::RenderWindow &window) : window(window) {}

  sf::RenderWindow &window;
  common::ConnectionManager connMgr;
  GameContext gameContext;
  bool isFpsCounterVisible{};
  bool isWindowFocused{};
};

} // namespace bm::scene
