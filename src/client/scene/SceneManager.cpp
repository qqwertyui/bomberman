#include "SceneManager.hpp"
#include "common/Log.hpp"
#include "scene/lobby/Scene.hpp"
#include "scene/menu/Scene.hpp"
#include "scene/settings/Scene.hpp"
#include <SFML/Graphics.hpp>
#include <cassert>
#include <type_traits>

namespace bm::scene {
SceneManager::SceneManager(sf::RenderWindow &window) {
  add(scene::SceneId::Menu, new menu::Scene(*this, window));
  add(scene::SceneId::Lobby, new lobby::Scene(*this, window));
  add(scene::SceneId::Settings, new settings::Scene(*this, window));

  change(SceneId::Menu);
}

SceneManager::~SceneManager() {
  for (auto &[id, scene] : scenes) {
    if (scene) {
      delete scene;
    }
  }
}

void SceneManager::add(SceneId sceneId, SceneBase *scene) {
  assert(scenes.count(sceneId) == 0);
  scenes[sceneId] = scene;
}

void SceneManager::remove(const SceneId &sceneId) { scenes.erase(sceneId); }

void SceneManager::change(const SceneId &sceneId) {
  auto *newScene = getSceneById(sceneId);
  if (newScene == nullptr) {
    LOG_WRN("Failed to switch to SceneId=%u",
            static_cast<unsigned int>(sceneId));
    return;
  }
  if (newScene == active) {
    return;
  }
  if (active) {
    active->onLeave();
  }
  newScene->onEntry();
  active = newScene;
}

SceneBase *SceneManager::getSceneById(const SceneId &sceneId) {
  if (scenes.count(sceneId) == 0) {
    return nullptr;
  }
  return scenes[sceneId];
}

SceneBase &SceneManager::getActive() {
  assert(active != nullptr);
  return *active;
}

} // namespace bm::scene
