#include "PlayerModel.hpp"
#include "common/logging/Log.hpp"

namespace bm::scene::game {
PlayerModel::PlayerModel() {
  constexpr float playerSize{60.f};
  circle.setRadius(playerSize / 2.f);
  circle.setFillColor(sf::Color::Red);

  const auto radius{circle.getRadius()};
  circle.setOrigin(sf::Vector2f{radius, radius});

  position = sf::Vector2f{radius, radius};
  circle.setPosition(position);
}

void PlayerModel::draw(sf::RenderTarget &target,
                       sf::RenderStates states) const {
  target.draw(circle, states);
}

void PlayerModel::setPosition(const sf::Vector2f &position) {
  circle.setPosition(position);
  this->position = position;
}

sf::Vector2f PlayerModel::getPosition() { return position; }

unsigned int PlayerModel::size() const { return circle.getRadius(); }
} // namespace bm::scene::game
