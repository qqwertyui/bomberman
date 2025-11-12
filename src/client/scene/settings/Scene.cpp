#include "Scene.hpp"
#include "common/logging/Log.hpp"
#include "scene/SceneManager.hpp"
#include "scene/SharedData.hpp"
#include <SFML/Graphics.hpp>

namespace bm::scene::settings {
Scene::Scene(SceneManager &sceneMgr)
    : SceneBase(sceneMgr), fpsCheckbox({60.f, 60.f}, {30.f, 30.f}),
      backButton({shared().window.getSize().x / 2.f - 100.f,
                  shared().window.getSize().y - 100.f},
                 "Back to Menu", 25) {
  fpsCheckbox.setCallback(
      [&sceneMgr](bool isChecked) { sceneMgr.setFpsVisible(isChecked); });
}

void Scene::handleEvents(const sf::Event &e) {
  if (const auto *keyPressed = e.getIf<sf::Event::KeyPressed>()) {
    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
      change(SceneId::Menu);
    }
  } else if (const auto *mouseButton =
                 e.getIf<sf::Event::MouseButtonPressed>()) {
    handleMouseClick(mouseButton->button);
  }
  fpsCheckbox.handleEvent(e);
}

void Scene::handleMouseClick(const sf::Mouse::Button &button) {
  auto &window{shared().window};
  auto localPos = sf::Mouse::getPosition(window);
  auto mousePos{static_cast<sf::Vector2f>(localPos)};

  if (button == sf::Mouse::Button::Left) {
    if (backButton.getButtonBounds().contains(mousePos)) {
      backButton.setActive(true);
      change(SceneId::Menu);
    }
  }
}

void Scene::update() {
  auto &window{shared().window};
  auto localPos = sf::Mouse::getPosition(window);
  auto mousePos{static_cast<sf::Vector2f>(localPos)};

  backButton.setActive(false);
  if (backButton.getButtonBounds().contains(mousePos)) {
    backButton.setActive(true);
  } else {
    backButton.setActive(false);
  }
}

void Scene::draw() {
  auto &window{shared().window};

  window.clear(sf::Color::Green);
  window.draw(fpsCheckbox);
  window.draw(backButton);
}

} // namespace bm::scene::settings
