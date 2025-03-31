#include "SceneManager.hpp"
#include "common/Log.hpp"
#include "game/Scene.hpp"
#include "lobby/Scene.hpp"
#include "menu/Scene.hpp"
#include "settings/Scene.hpp"
#include <SFML/Graphics.hpp>
#include <cassert>

namespace bm::scene {
SceneManager::SceneManager(sf::RenderWindow &window) : window(window) {
  add(SceneId::Menu, new menu::Scene(*this));
  add(SceneId::Settings, new settings::Scene(*this));
  add(SceneId::Lobby, new lobby::Scene(*this));
  add(SceneId::Game, new game::Scene(*this));

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

void SceneManager::handleEvents() { getActive().handleEvents(); }

void SceneManager::update() { getActive().update(); }

void SceneManager::draw() { getActive().draw(); }

sf::RenderWindow &SceneManager::getWindow() { return window; }

} // namespace bm::scene
