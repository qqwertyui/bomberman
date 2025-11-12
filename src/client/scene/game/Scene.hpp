#pragma once

#include "scene/SceneBase.hpp"
#include <memory>

namespace bm::scene::game {
class Scene : public SceneBase {
public:
  Scene(SceneManager &sceneMgr);

  void onEntry() override;
  void onLeave() override;

  void handleEvents(const sf::Event &e) override;
  void update() override;
  void draw() override;

private:
  struct impl;
  std::unique_ptr<impl> pimpl;
};

} // namespace bm::scene::game
