#pragma once

#include <SFML/Graphics.hpp>

namespace sf {
class RenderTarget;
struct RenderStates;
struct Event;
} // namespace sf

namespace bm::gui {
class Widget : public sf::Drawable, public sf::Transformable {
public:
  Widget() = default;

  virtual void onEvent([[maybe_unused]] const sf::Event &e) {}
  virtual void onActivate() {}
  virtual void onDeactivate() {}
  virtual void onHoverStart() {}
  virtual void onHoverStop() {}

  virtual bool contains(const sf::Vector2f &coords) const = 0;
};

} // namespace bm::gui
