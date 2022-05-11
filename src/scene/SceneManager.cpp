#include "SceneManager.hpp"
#include "utils/Log.hpp"

namespace SimpleSnake::scene {

SceneManager::SceneManager(std::list<std::unique_ptr<Scene>> &&scenes)
    : scenes(std::move(scenes)) {}

void SceneManager::add(std::unique_ptr<Scene> &&scene) {
  scenes.emplace_back(std::move(scene));
}

void SceneManager::remove(const SceneId &sceneId) {
  auto it = std::find_if(scenes.begin(), scenes.end(),
                         [&sceneId](std::unique_ptr<Scene> &scene) {
                           return scene->getId() == sceneId;
                         });
  if (it != scenes.end()) {
    scenes.erase(it);
  }
}

void SceneManager::change(const SceneId &sceneId) {
  Scene *scene = getSceneById(sceneId);
  if (scene == nullptr) {
    LOG_WRN("Failed to switch to SceneId=%u",
            static_cast<unsigned int>(sceneId));
    return;
  }
  activeScene = scene;
}

Scene *SceneManager::getSceneById(const SceneId &sceneId) {
  auto it = std::find_if(scenes.begin(), scenes.end(),
                         [&sceneId](std::unique_ptr<Scene> &scene) {
                           return scene->getId() == sceneId;
                         });
  if (it != scenes.end()) {
    return it->get();
  }
  return nullptr;
}

Scene &SceneManager::getActive() { return *activeScene; }

} // namespace SimpleSnake::scene