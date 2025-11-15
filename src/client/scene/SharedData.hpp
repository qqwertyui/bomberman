#pragma once

#include "common/ConnectionManager.hpp"
#include <SFML/Graphics.hpp>

namespace bm::scene {
class SharedData {
public:
  SharedData(sf::RenderWindow &window) : window(window) {}

  sf::RenderWindow &window;
  common::ConnectionManager connMgr;
  bool isFpsCounterVisible{};
};

} // namespace bm::scene
