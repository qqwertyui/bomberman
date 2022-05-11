#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace SimpleSnake::interface {
class Button : public sf::Drawable {
public:
  Button(const std::string &label);
  void draw(sf::RenderTarget &target,
            const sf::RenderStates &states) const override;

private:
  sf::Sprite m_buttonSprite;
};

} // namespace SimpleSnake::interface