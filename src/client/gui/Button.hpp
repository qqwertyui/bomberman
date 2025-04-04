#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace bm::gui {
class Button : public sf::Drawable, public sf::Transformable {
public:
  Button(const sf::Vector2f &position, const std::string &label,
         unsigned int characterSize = 25);
  void draw(sf::RenderTarget &target,
            const sf::RenderStates states) const override;
  void setActive(bool active);
  sf::FloatRect getButtonBounds() const;

private:
  sf::Sprite m_buttonSprite;
  sf::Text m_buttonLabel;
};
} // namespace bm::gui
