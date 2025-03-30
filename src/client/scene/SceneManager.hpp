#pragma once

#include "SceneBase.hpp"
#include <list>
#include <memory>

namespace bm::scene {
class SceneManager {
public:
  SceneManager() = default;
  SceneManager(std::list<std::unique_ptr<SceneBase>> &&scenes);
  void add(std::unique_ptr<SceneBase> &&scene);
  void remove(const SceneId &sceneId);
  void change(const SceneId &sceneId);
  SceneBase &getActive();

private:
  SceneBase *getSceneById(const SceneId &sceneId);
  std::list<std::unique_ptr<SceneBase>> scenes{};
  SceneBase *activeScene{nullptr};
};

} // namespace bm::scene
