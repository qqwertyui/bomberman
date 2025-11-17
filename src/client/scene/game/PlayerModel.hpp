#pragma once

#include <SFML/Graphics.hpp>

namespace bm::scene::game {
class PlayerModel : public sf::Drawable {
public:
  PlayerModel();
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
  void update();
  void setPosition(const sf::Vector2f &position);
  sf::Vector2f getPosition();
  unsigned int size() const;

private:
  sf::CircleShape circle;
  sf::Vector2f position;
};
} // namespace bm::scene::game
