#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace sf {
class RenderTarget;
struct RenderStates;
struct Event;
} // namespace sf

namespace bm::gui {
class Widget : public sf::Drawable, public sf::Transformable {
public:
  Widget(const std::string &id) : m_id(id) {}
  inline std::string id() const { return m_id; }
  virtual std::string value() { return ""; }

protected:
  virtual void click() {}
  virtual void hover() {}
  virtual void reset() {}

  virtual bool selectable() const { return false; }
  virtual void handleEvent([[maybe_unused]] const sf::Event &e) {}

  virtual bool contains(const sf::Vector2f &coords) const = 0;
  friend class WidgetManager;

private:
  std::string m_id;
};

} // namespace bm::gui
