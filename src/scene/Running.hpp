#pragma once

#include "Scene.hpp"

namespace SimpleSnake::scene {
class Running : public Scene {
public:
  Running(SceneManager &sceneMgr, sf::RenderWindow &window,
          const SceneId &sceneId);

  void handleEvents() override;
  void update() override;
  void draw() override;
};
} // namespace SimpleSnake::scene