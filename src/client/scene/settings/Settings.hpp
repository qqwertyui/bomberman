#pragma once

#include "scene/Scene.hpp"

namespace bm::scene {
class Settings : public Scene {
public:
  Settings(SceneManager &sceneMgr, sf::RenderWindow &window,
           const SceneId &sceneId);

  void handleEvents() override;
  void update() override;
  void draw() override;
};

} // namespace bm::scene
