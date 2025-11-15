#pragma once

#include "common/NonCopyable.hpp"
#include <SFML/Window.hpp>
#include <optional>
#include <vector>

namespace sf {
struct Event;
class RenderWindow;
} // namespace sf

namespace bm::gui {
class Widget;

class WidgetManager : public common::NonCopyable {
public:
  WidgetManager(sf::RenderWindow &window);
  ~WidgetManager();

  void add(Widget *widget);
  void remove(Widget *widget);
  void clear();

  Widget *getById(const std::string &id);

  void draw();
  void handleEvents(const sf::Event &e);
  void update();

private:
  Widget *getWidgetByCoords(const sf::Vector2f &coords);
  std::optional<unsigned int> getIndexByWidget(Widget *widget);

  bool shouldDeactivateWidget(const sf::Event &e);
  bool shouldActivateWidget(const sf::Event &e);
  void updateHover(const sf::Event &e);

  sf::RenderWindow &window;
  std::vector<Widget *> widgets;
  Widget *activeWidget{nullptr};
  Widget *hoverWidget{nullptr};
  Widget *lastHoverWidget{nullptr};
  sf::Vector2f lastMousePosition{};
};
} // namespace bm::gui
