#pragma once

#include "SceneId.hpp"

namespace sf {
class RenderWindow;
}

namespace bm::scene {
class SceneManager;

class SceneBase {
public:
  SceneBase(SceneManager &sceneManager);
  ~SceneBase();

  virtual void onEntry() {};
  virtual void onLeave() {};

  virtual void handleEvents() = 0;
  virtual void update() = 0;
  virtual void draw() = 0;

  sf::RenderWindow &getWindow();
  void change(const SceneId &sceneId);

private:
  SceneManager &sceneManager;
};

} // namespace bm::scene
