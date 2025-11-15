#include "Scene.hpp"
#include "gui/Button.hpp"
#include "gui/Checkbox.hpp"
#include "scene/SceneManager.hpp"
#include "scene/SharedData.hpp"
#include <SFML/Graphics.hpp>

namespace bm::scene::settings {
Scene::Scene(SceneManager &sceneMgr)
    : SceneBase(sceneMgr), widgetManager(shared().window) {
  widgetManager.add(new gui::Button{"back",
                                    {shared().window.getSize().x / 2.f - 100.f,
                                     shared().window.getSize().y - 100.f},
                                    "Back to Menu",
                                    [this]() { change(SceneId::Menu); },
                                    25});
  widgetManager.add(
      new gui::Checkbox("fps_visible", {60.f, 60.f}, {30.f, 30.f}));
}

void Scene::handleEvents(const sf::Event &e) {
  auto oldBoxValue = widgetManager.getById("fps_visible")->value();
  widgetManager.handleEvents(e);
  auto newBoxValue = widgetManager.getById("fps_visible")->value();
  if (oldBoxValue != newBoxValue) {
    bool isVisible = not newBoxValue.empty();
    shared().isFpsCounterVisible = isVisible;
  }
}

void Scene::update() { widgetManager.update(); }

void Scene::draw() {
  auto &window{shared().window};

  window.clear(sf::Color::Green);
  widgetManager.draw();
}

} // namespace bm::scene::settings
