#include "WidgetManager.hpp"
#include "Widget.hpp"
#include "common/Utils.hpp"
#include "common/logging/Log.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cassert>

namespace bm::gui {
WidgetManager::WidgetManager(sf::RenderWindow &window) : window(window) {}

WidgetManager::~WidgetManager() {
  for (const auto *widget : widgets) {
    delete widget;
  }
}

void WidgetManager::add(Widget *widget) {
  assert(widget != nullptr);
  widgets.push_back(widget);
}

void WidgetManager::remove(Widget *widget) {
  assert(widget != nullptr);
  widgets.erase(std::remove(widgets.begin(), widgets.end(), widget),
                widgets.end());
}

void WidgetManager::draw() {
  for (const auto *widget : widgets) {
    window.draw(*widget);
  }
}

void WidgetManager::handleEvents(const sf::Event &e) {
  if (widgets.empty()) {
    return;
  }
  if (shouldDeactivateWidget(e)) {
    activeWidget->onDeactivate();
    activeWidget = nullptr;
    return;
  }
  if (shouldActivateWidget(e)) {
    activeWidget = hoverWidget;
    activeWidget->onActivate();
    return;
  }
  if (not activeWidget) {
    updateHover(e);
    return;
  }
  activeWidget->onEvent(e);
}

bool WidgetManager::shouldDeactivateWidget(const sf::Event &e) {
  if (not activeWidget) {
    return false;
  }
  const auto *mouseButton = e.getIf<sf::Event::MouseButtonPressed>();
  if ((mouseButton) and (mouseButton->button == sf::Mouse::Button::Left) and
      (not hoverWidget)) {
    return true;
  }
  const auto *keyPressed = e.getIf<sf::Event::KeyPressed>();
  if ((keyPressed) and
      (keyPressed->scancode == sf::Keyboard::Scancode::Escape)) {
    return true;
  }
  return false;
}

bool WidgetManager::shouldActivateWidget(const sf::Event &e) {
  if (activeWidget or (not hoverWidget)) {
    return false;
  }
  const auto *mouseButton = e.getIf<sf::Event::MouseButtonPressed>();
  if ((mouseButton) and (mouseButton->button == sf::Mouse::Button::Left)) {
    return true;
  }
  const auto *keyPressed = e.getIf<sf::Event::KeyPressed>();
  if ((keyPressed) and
      (keyPressed->scancode == sf::Keyboard::Scancode::Enter)) {
    return true;
  }
  return false;
}

void WidgetManager::updateHover(const sf::Event &e) {
  const auto *keyPressed = e.getIf<sf::Event::KeyPressed>();
  if (not keyPressed) {
    return;
  }
  auto scancode = keyPressed->scancode;
  if (scancode != sf::Keyboard::Scancode::Up and
      scancode != sf::Keyboard::Scancode::Down) {
    return;
  }
  const auto newWidgetIdx = std::clamp(
      (previousWidgetIdx + (scancode == sf::Keyboard::Scancode::Up ? -1 : 1)),
      0, (int)widgets.size() - 1);
  if (newWidgetIdx != previousWidgetIdx) {
    widgets[newWidgetIdx]->onHoverStart();
    widgets[previousWidgetIdx]->onHoverStop();
  } else if ((newWidgetIdx == previousWidgetIdx) and (not hoverWidget)) {
    widgets[newWidgetIdx]->onHoverStart();
  }
  previousWidgetIdx = newWidgetIdx;
  hoverWidget = widgets[newWidgetIdx];
}

Widget *WidgetManager::getWidgetByCoords(const sf::Vector2f &coords) {
  for (auto *widget : widgets) {
    if (not widget->contains(coords)) {
      continue;
    }
    return widget;
  }
  return nullptr;
}

std::optional<unsigned int> WidgetManager::getIndexByWidget(Widget *widget) {
  if (not widget) {
    return std::nullopt;
  }
  if (widgets.empty()) {
    return std::nullopt;
  }
  for (auto i = 0u; i < widgets.size(); i++) {
    if (widgets[i] == widget) {
      return i;
    }
  }
  return std::nullopt;
}

void WidgetManager::update() {
  if (widgets.empty()) {
    return;
  }
  auto mousePosition =
      static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
  const auto mousePositionChanged{mousePosition != lastMousePosition};
  if (not mousePositionChanged) {
    return;
  }
  lastMousePosition = mousePosition;
  auto *widget = getWidgetByCoords(mousePosition);
  if (widget == hoverWidget) {
    return;
  }
  if (widget) {
    widget->onHoverStart();
  }
  if (hoverWidget) {
    hoverWidget->onHoverStop();
  }
  hoverWidget = widget;
  previousWidgetIdx = *getIndexByWidget(widget);
}

void WidgetManager::reset() { activeWidget = nullptr; }
} // namespace bm::gui
