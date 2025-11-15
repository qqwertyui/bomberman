#include "Checkbox.hpp"

namespace bm::gui {
Checkbox::Checkbox(const std::string &id, const sf::Vector2f &position,
                   const sf::Vector2f &size)
    : Widget(id) {
  boxShape.setPosition(position);
  boxShape.setSize(size);
  boxShape.setFillColor(sf::Color::White);
  boxShape.setOutlineThickness(3.f);
  boxShape.setOutlineColor(sf::Color::Black);

  checkMark.setSize({size.x - 6.f, size.y - 6.f});
  checkMark.setFillColor(sf::Color::Green);
  checkMark.setPosition(sf::Vector2f{position.x + 3.f, position.y + 3.f});
}

bool Checkbox::contains(const sf::Vector2f &coords) const {
  return boxShape.getGlobalBounds().contains(coords);
}

std::string Checkbox::value() const { return (checked) ? "set" : ""; }

void Checkbox::click() { checked = (not checked); }

void Checkbox::draw(sf::RenderTarget &target,
                    const sf::RenderStates states) const {
  target.draw(boxShape, states);
  if (checked) {
    target.draw(checkMark, states);
  }
}
} // namespace bm::gui
