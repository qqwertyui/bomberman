#pragma once

#include <SFML/Graphics.hpp>

namespace bm::common::itf {
class C2SMessage;
}

namespace bm::scene::game {
class Player : public sf::Drawable {
public:
  Player(sf::RenderTarget &parentWindow);
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
  void update(common::itf::C2SMessage &req);

private:
  sf::Vector2f calculateVelocity() const;
  sf::Vector2f calculatePosition(sf::Vector2f currentPosition,
                                 sf::Vector2f velocity);
  sf::Vector2f alignToMapBounds(sf::Vector2f position) const;
  bool hasMoved(sf::Vector2f posA, sf::Vector2f posB) const;

  sf::CircleShape circle;
  sf::Vector2f velocity;
  sf::Vector2f position;
  sf::Clock updateTime;

  sf::RenderTarget &parentWindow;
};

} // namespace bm::scene::game
