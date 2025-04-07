#include "Player.hpp"

#include "common/itf/core.pb.h"
#include <SFML/Graphics.hpp>
#include <algorithm>

namespace bm::scene::game {
Player::Player(sf::RenderTarget &parentWindow) : parentWindow(parentWindow) {
  constexpr float playerSize{60.f};
  circle.setRadius(playerSize / 2.f);
  circle.setFillColor(sf::Color::Red);

  const auto radius{circle.getRadius()};
  circle.setOrigin(sf::Vector2f{radius, radius});

  position = sf::Vector2f{radius, radius};
  circle.setPosition(position);

  updateTime.restart();
}

sf::Vector2f Player::calculateVelocity() const {
  static sf::Vector2f previousVelocity{};
  constexpr float pixelsPerSecond{120.f};

  auto calculatedVelocity{sf::Vector2f{}};
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
    calculatedVelocity.y -= pixelsPerSecond;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
    calculatedVelocity.y += pixelsPerSecond;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
    calculatedVelocity.x -= pixelsPerSecond;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
    calculatedVelocity.x += pixelsPerSecond;
  }

  auto finalVelocity{calculatedVelocity};
  if (previousVelocity.x != finalVelocity.x) {
    finalVelocity.x = {};
  }
  if (previousVelocity.y != finalVelocity.y) {
    finalVelocity.y = {};
  }
  previousVelocity = calculatedVelocity;

  return finalVelocity;
}

sf::Vector2f Player::alignToMapBounds(sf::Vector2f position) const {
  const auto windowSize{parentWindow.getSize()};
  auto radius{static_cast<unsigned int>(circle.getRadius())};
  const sf::Vector2u halfModelSize{radius, radius};

  sf::Vector2f lowerBound{halfModelSize};
  sf::Vector2f upperBound{windowSize - halfModelSize};

  return sf::Vector2f{std::clamp(position.x, lowerBound.x, upperBound.x),
                      std::clamp(position.y, lowerBound.y, upperBound.y)};
}

sf::Vector2f Player::calculatePosition(sf::Vector2f currentPosition,
                                       sf::Vector2f velocity) {
  constexpr unsigned int microsecond{1'000'000};
  sf::Vector2f pixelsPerUs{velocity.x / microsecond, velocity.y / microsecond};
  auto usSinceLastUpdate{
      static_cast<float>(updateTime.getElapsedTime().asMicroseconds())};
  sf::Vector2f distance{usSinceLastUpdate * pixelsPerUs};

  currentPosition += distance;
  return alignToMapBounds(currentPosition);
}

bool Player::hasMoved(sf::Vector2f posA, sf::Vector2f posB) const {
  return (static_cast<sf::Vector2i>(posA) != static_cast<sf::Vector2i>(posB));
}

void Player::update(common::itf::C2SMessage &req) {
  auto velocity = calculateVelocity();
  auto oldPosition = position;
  position = calculatePosition(position, velocity);

  if (hasMoved(oldPosition, position)) {
    circle.setPosition(position);

    auto *msg = req.mutable_update()->mutable_game()->mutable_position();
    msg->set_x(position.x);
    msg->set_y(position.y);
  }

  updateTime.restart();
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(circle, states);
}

} // namespace bm::scene::game
