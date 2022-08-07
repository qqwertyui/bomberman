#pragma once

#include "common/ConnectionManager.hpp"
#include "scene/Scene.hpp"

namespace bomberman::scene {
class Running : public Scene {
public:
  Running(SceneManager &sceneMgr, sf::RenderWindow &window,
          const SceneId &sceneId);

  void handleEvents() override;
  void update() override;
  void draw() override;
  void onEntry() override;
  void onLeave() override;

private:
  common::ConnectionManager connMgr;
  int x, y;
};
} // namespace bomberman::scene