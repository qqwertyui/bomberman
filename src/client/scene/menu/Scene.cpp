#include "Scene.hpp"
#include "gui/Button.hpp"
#include "gui/TextBox.hpp"
#include "scene/SharedData.hpp"
#include <SFML/Graphics.hpp>

namespace bm::scene::menu {
Scene::Scene(SceneManager &sceneMgr)
    : SceneBase(sceneMgr), widgetManager(shared().window) {
  auto &window{shared().window};
  float buttonSpacing{60.f};
  float centerX = window.getSize().x / 2.f;
  float centerY = window.getSize().y / 2.f;
  sf::Vector2f ButtonSize(190.f, 49.f);
  sf::Vector2f startButton(centerX - ButtonSize.x / 2.f,
                           centerY - ButtonSize.y / 2.f - buttonSpacing);
  sf::Vector2f settingsButton(centerX - ButtonSize.x / 2.f,
                              centerY - ButtonSize.y / 2.f);
  sf::Vector2f exitButton(centerX - ButtonSize.x / 2.f,
                          centerY - ButtonSize.y / 2.f + buttonSpacing);

  widgetManager.add(new gui::TextBox(
      "server_ip", {shared().window.getSize().x / 2.f - 200.f, 100.f},
      {400.f, 50.f}, "Enter text ...", 30, 20));
  widgetManager.add(new gui::Button("start", startButton, "Start",
                                    [this]() { change(SceneId::Lobby); }));
  widgetManager.add(new gui::Button("settings", settingsButton, "Settings",
                                    [this]() { change(SceneId::Settings); }));
  widgetManager.add(new gui::Button("exit", exitButton, "Exit",
                                    [this]() { shared().window.close(); }));
}

void Scene::handleEvents(const sf::Event &e) { widgetManager.handleEvents(e); }

void Scene::update() {
  widgetManager.update();
  // LOG_INF("%s", widgetManager.getById("server_ip")->value().c_str());
}

void Scene::draw() {
  auto &window{shared().window};
  window.clear(sf::Color::Red);
  widgetManager.draw();
}
} // namespace bm::scene::menu
