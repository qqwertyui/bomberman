#pragma once

#include "common/ConnectionManager.hpp"
#include "scene/SceneBase.hpp"

namespace bm::scene::lobby {
class Scene : public SceneBase {
public:
  Scene(SceneManager &sceneMgr, sf::RenderWindow &window);

  void handleEvents() override;
  void update() override;
  void draw() override;
  void onEntry() override;
  void onLeave() override;

private:
  common::ConnectionManager connMgr;
  int x, y;
};

} // namespace bm::scene::lobby
