#include "Checkbox.hpp"

namespace bm::gui {
Checkbox::Checkbox(const sf::Vector2f &position, const sf::Vector2f &size)
    : checked(false) {
  boxShape.setPosition(position);
  boxShape.setSize(size);
  boxShape.setFillColor(sf::Color::White);
  boxShape.setOutlineThickness(3.f);
  boxShape.setOutlineColor(sf::Color::Black);

  checkMark.setSize({size.x - 6.f, size.y - 6.f});
  checkMark.setFillColor(sf::Color::Green);
  checkMark.setPosition(sf::Vector2f{position.x + 3.f, position.y + 3.f});
}
void Checkbox::handleEvent(const sf::Event &e) {
  if (const auto *mousePressed = e.getIf<sf::Event::MouseButtonPressed>()) {
    sf::Vector2f mousePos(mousePressed->position);
    if (boxShape.getGlobalBounds().contains(mousePos)) {
      checked = !checked;
    }
  }
}

bool Checkbox::isChecked() const { return checked; }

void Checkbox::draw(sf::RenderTarget &target,
                    const sf::RenderStates states) const {
  target.draw(boxShape, states);
  if (checked) {
    target.draw(checkMark, states);
  }
}
} // namespace bm::gui