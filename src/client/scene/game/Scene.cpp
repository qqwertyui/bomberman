#include "Scene.hpp"
#include <SFML/Graphics.hpp>

namespace bm::scene::game {
Scene::Scene(SceneManager &sceneMgr) : SceneBase(sceneMgr) {}

void Scene::handleEvents() {
  //
}

void Scene::update() {
  //
}

void Scene::draw() {
  auto &window{getWindow()};

  window.clear(sf::Color::Cyan);
  window.display();
}

} // namespace bm::scene::game
