#pragma once

#include "SceneBase.hpp"
#include <map>

namespace sf {
class RenderWindow;
}

namespace bm::scene {
class SceneManager {
public:
  SceneManager(sf::RenderWindow &window);
  ~SceneManager();

  void add(SceneId sceneId, SceneBase *scene);
  void remove(const SceneId &sceneId);
  void change(const SceneId &sceneId);
  SceneBase &getActive();

private:
  SceneBase *getSceneById(const SceneId &sceneId);

  std::map<SceneId, SceneBase *> scenes{};
  SceneBase *active{nullptr};
};

} // namespace bm::scene
