#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace SimpleSnake::interface {
class Button : public sf::Drawable, public sf::Transformable {
public:
  Button() = default;
  Button(const std::string &label);
  void draw(sf::RenderTarget &target,
            const sf::RenderStates &states) const override;
  void setActive(bool active);

private:
  sf::Sprite m_buttonSprite;
};

} // namespace SimpleSnake::interface