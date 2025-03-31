#pragma once

#include "scene/SceneBase.hpp"

namespace bm::scene::settings {
class Scene : public SceneBase {
public:
  Scene(SceneManager &sceneMgr, sf::RenderWindow &window);

  void handleEvents() override;
  void update() override;
  void draw() override;
};

} // namespace bm::scene::settings
