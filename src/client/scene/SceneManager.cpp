#include "SceneManager.hpp"
#include "common/Log.hpp"
#include <algorithm>

namespace bm::scene {
SceneManager::SceneManager(std::list<std::unique_ptr<SceneBase>> &&scenes)
    : scenes(std::move(scenes)) {}

void SceneManager::add(std::unique_ptr<SceneBase> &&scene) {
  scenes.emplace_back(std::move(scene));
}

void SceneManager::remove(const SceneId &sceneId) {
  auto it = std::find_if(scenes.begin(), scenes.end(),
                         [&sceneId](std::unique_ptr<SceneBase> &scene) {
                           return scene->getId() == sceneId;
                         });
  if (it != scenes.end()) {
    scenes.erase(it);
  }
}

void SceneManager::change(const SceneId &sceneId) {
  auto *scene = getSceneById(sceneId);
  if (scene == nullptr) {
    LOG_WRN("Failed to switch to SceneId=%u",
            static_cast<unsigned int>(sceneId));
    return;
  }
  if (scene != activeScene) {
    if (activeScene) {
      activeScene->onLeave();
    }
    scene->onEntry();
    activeScene = scene;
  }
}

SceneBase *SceneManager::getSceneById(const SceneId &sceneId) {
  auto it = std::find_if(scenes.begin(), scenes.end(),
                         [&sceneId](std::unique_ptr<SceneBase> &scene) {
                           return scene->getId() == sceneId;
                         });
  if (it != scenes.end()) {
    return it->get();
  }
  return nullptr;
}

SceneBase &SceneManager::getActive() { return *activeScene; }

} // namespace bm::scene
