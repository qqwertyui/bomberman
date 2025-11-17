#include "SceneManager.hpp"
#include "SharedData.hpp"
#include "common/ToUnderlying.hpp"
#include "common/logging/Log.hpp"
#include "game/Scene.hpp"
#include "lobby/Scene.hpp"
#include "menu/Scene.hpp"
#include "resource/FontManager.hpp"
#include "settings/Scene.hpp"
#include <SFML/Graphics.hpp>
#include <cassert>

namespace bm::scene {
SceneManager::SceneManager(sf::RenderWindow &window)
    : fpsText(resource::FontManager::get().at(resource::FontId::MENU)) {
  fpsText.setCharacterSize(30);
  fpsText.setFillColor(sf::Color::Cyan);
  fpsText.setPosition(sf::Vector2f(30, 30));
  fpsText.setString("FPS: " + std::to_string(0));

  sharedData = new SharedData(window);

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
  if (sharedData) {
    delete sharedData;
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
    LOG_WRN("Failed to switch to SceneId=%u", common::toUnderlying(sceneId));
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

void SceneManager::drawFps() {
  frameCount++;
  if (fpsClock.getElapsedTime().asSeconds() > 1.f) {
    fpsText.setString("FPS: " + std::to_string(frameCount));
    fpsClock.restart();
    frameCount = 0;
  }
  shared().window.draw(fpsText);
}

void SceneManager::handleEvents() {
  while (const auto &e = shared().window.pollEvent()) {
    if (e->is<sf::Event::Closed>()) {
      shared().window.close();
    } else if (e->is<sf::Event::FocusGained>()) {
      LOG_DBG("Window focus gained");
      shared().isWindowFocused = true;
    } else if (e->is<sf::Event::FocusLost>()) {
      LOG_DBG("Window focus lost");
      shared().isWindowFocused = false;
    } else {
      getActive().handleEvents(*e);
    }
  }
}

void SceneManager::update() { getActive().update(); }

void SceneManager::draw() {
  getActive().draw();
  if (shared().isFpsCounterVisible) {
    drawFps();
  }
  shared().window.display();
}

SharedData &SceneManager::shared() { return *sharedData; }

} // namespace bm::scene
