#pragma once

#include "Scene.hpp"
#include <list>
#include <memory>

namespace bm::scene {
class SceneManager {
public:
  SceneManager() = default;
  SceneManager(std::list<std::unique_ptr<Scene>> &&scenes);
  void add(std::unique_ptr<Scene> &&scene);
  void remove(const SceneId &sceneId);
  void change(const SceneId &sceneId);
  Scene &getActive();

private:
  Scene *getSceneById(const SceneId &sceneId);
  std::list<std::unique_ptr<Scene>> scenes{};
  Scene *activeScene{nullptr};
};

} // namespace bm::scene
