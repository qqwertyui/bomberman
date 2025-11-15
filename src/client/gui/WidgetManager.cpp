#include "WidgetManager.hpp"
#include "Widget.hpp"
#include "common/Utils.hpp"
#include "common/logging/Log.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cassert>

namespace bm::gui {
WidgetManager::WidgetManager(sf::RenderWindow &window) : window(window) {}

WidgetManager::~WidgetManager() { clear(); }

void WidgetManager::add(Widget *widget) {
  assert(widget != nullptr);
  auto result =
      std::find_if(widgets.begin(), widgets.end(),
                   [&widget](Widget *w) { return (w->id() == widget->id()); });
  assert(result == widgets.end());
  widgets.push_back(widget);
}

void WidgetManager::remove(Widget *widget) {
  assert(widget != nullptr);
  widgets.erase(std::remove(widgets.begin(), widgets.end(), widget),
                widgets.end());
}

void WidgetManager::clear() {
  for (const auto *widget : widgets) {
    delete widget;
  }
  widgets.clear();
}

Widget *WidgetManager::getById(const std::string &id) {
  auto result = std::find_if(widgets.begin(), widgets.end(),
                             [&id](Widget *w) { return (w->id() == id); });
  return (result != widgets.end()) ? *result : nullptr;
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
    activeWidget->reset();
    activeWidget = nullptr;
    return;
  }
  if (shouldActivateWidget(e)) {
    if (hoverWidget->selectable()) {
      activeWidget = hoverWidget;
    }
    hoverWidget->click();
    return;
  }
  if (not activeWidget) {
    updateHover(e);
    return;
  }
  activeWidget->handleEvent(e);
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
  if (not lastHoverWidget) {
    lastHoverWidget = widgets.front();
  }
  const auto lastWidgetIdx{*getIndexByWidget(lastHoverWidget)};
  const auto newWidgetIdx{(unsigned int)std::clamp(
      ((int)lastWidgetIdx + (scancode == sf::Keyboard::Scancode::Up ? -1 : 1)),
      0, (int)widgets.size() - 1)};

  if (not hoverWidget) {
    widgets[newWidgetIdx]->hover();
  } else if (newWidgetIdx != lastWidgetIdx) {
    widgets[newWidgetIdx]->hover();
    widgets[lastWidgetIdx]->reset();
  }
  lastHoverWidget = hoverWidget = widgets[newWidgetIdx];
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
  if (hoverWidget) {
    hoverWidget->reset();
  }
  if (widget) {
    lastHoverWidget = widget;
    widget->hover();
  }
  hoverWidget = widget;
}
} // namespace bm::gui
