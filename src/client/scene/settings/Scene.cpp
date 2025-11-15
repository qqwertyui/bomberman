#include "Scene.hpp"
#include "gui/Button.hpp"
#include "gui/Checkbox.hpp"
#include "scene/SceneManager.hpp"
#include "scene/SharedData.hpp"
#include <SFML/Graphics.hpp>

namespace bm::scene::settings {
Scene::Scene(SceneManager &sceneMgr)
    : SceneBase(sceneMgr), fpsCheckbox({60.f, 60.f}, {30.f, 30.f}),
      widgetManager(shared().window) {
  widgetManager.add(new gui::Button{{shared().window.getSize().x / 2.f - 100.f,
                                     shared().window.getSize().y - 100.f},
                                    "Back to Menu",
                                    [this]() { change(SceneId::Menu); },
                                    25});
  fpsCheckbox.setCallback(
      [&sceneMgr](bool isChecked) { sceneMgr.setFpsVisible(isChecked); });
}

void Scene::onEntry() { widgetManager.reset(); }

void Scene::handleEvents(const sf::Event &e) {
  widgetManager.handleEvents(e);
  fpsCheckbox.handleEvent(e);
}

void Scene::update() { widgetManager.update(); }

void Scene::draw() {
  auto &window{shared().window};

  window.clear(sf::Color::Green);
  widgetManager.draw();
  window.draw(fpsCheckbox);
}

} // namespace bm::scene::settings
