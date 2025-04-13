#pragma once

#include "SceneId.hpp"

namespace bm::scene {
class SharedData;
class SceneManager;

class SceneBase {
public:
  SceneBase(SceneManager &sceneManager);
  virtual ~SceneBase();

  virtual void onEntry() {};
  virtual void onLeave() {};

  virtual void handleEvents() = 0;
  virtual void update() = 0;
  virtual void draw() = 0;

  SharedData &shared();
  void change(const SceneId &sceneId);

private:
  SceneManager &sceneManager;
};

} // namespace bm::scene
