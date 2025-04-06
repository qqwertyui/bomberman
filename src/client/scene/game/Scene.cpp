#include "Scene.hpp"

#include "GlobalConfig.hpp"
#include "common/ConnectionManager.hpp"
#include "common/Log.hpp"
#include "common/itf/core.pb.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <optional>
#include <thread>

namespace bm::scene::game {
struct Scene::impl {
  impl(Scene *base);

  void onEntry();
  void onLeave();

  void handleEvents();
  void update();
  void draw();

  void handleKeyPressed(const sf::Keyboard::Scancode &scancode);
  void handleKeyReleased(const sf::Keyboard::Scancode &scancode);
  sf::Vector2f calculateVelocity();

  sf::Vector2f calculatePlayerPosition(sf::Vector2f oldVelocity,
                                       sf::Vector2f newVelocity);
  void sendServerUpdate(const sf::Vector2i &coordinates);

  Scene *base;

  sf::CircleShape circle;
  sf::Vector2f positionFloat;
  sf::Vector2i positionInt;

  sf::Vector2f velocity;
  sf::Clock updateTime;

  common::ConnectionManager connMgr;

  static void messageReceiverLoop(common::ConnectionManager *connMgr);
  std::thread serverMessageHandler;

  static constexpr float pixelsPerSecond{120};
};

Scene::impl::impl(Scene *base) : base(base) {
  constexpr float playerSize{60.f};
  circle.setRadius(playerSize / 2.f);
  circle.setFillColor(sf::Color::Red);

  const auto radius{circle.getRadius()};
  circle.setOrigin(sf::Vector2f(radius, radius));

  positionFloat = sf::Vector2f{radius, radius};
  positionInt =
      sf::Vector2i{static_cast<int>(radius), static_cast<int>(radius)};
  circle.setPosition(positionFloat);

  velocity = {0.f, 0.f};
}

void Scene::impl::messageReceiverLoop(common::ConnectionManager *connMgr) {
  connMgr->connect(GlobalConfig::get().serverIp(),
                   GlobalConfig::get().serverPort());
  if (not connMgr->isConnected()) {
    LOG_ERR("Couldn't connect to server");
    return;
  }

  while (auto resp = connMgr->receive<common::itf::S2CMessage>()) {
    const bool ack{resp->has_update() and resp->update().has_game() and
                   resp->update().game().has_ispositionok() and
                   resp->update().game().ispositionok()};
    LOG_DBG(ack ? "ack" : "nack");
  }
}

void Scene::impl::onEntry() {
  serverMessageHandler = std::thread{messageReceiverLoop, &connMgr};
  updateTime.restart();
}

void Scene::impl::onLeave() {
  connMgr.disconnect();
  serverMessageHandler.join();
}

void Scene::impl::handleEvents() {
  auto &window{base->getWindow()};

  while (const auto &e = window.pollEvent()) {
    if (e->is<sf::Event::Closed>()) {
      window.close();
    } else if (const auto *keyPressed = e->getIf<sf::Event::KeyPressed>()) {
      handleKeyPressed(keyPressed->scancode);
    } else if (const auto *keyReleased = e->getIf<sf::Event::KeyReleased>()) {
      handleKeyReleased(keyReleased->scancode);
    }
  }
}

void Scene::impl::handleKeyPressed(const sf::Keyboard::Scancode &scancode) {
  if (scancode == sf::Keyboard::Scancode::Escape) {
    base->change(SceneId::Lobby);
    return;
  }
}

void Scene::impl::handleKeyReleased(
    [[maybe_unused]] const sf::Keyboard::Scancode &scancode) {
  //
}

sf::Vector2f Scene::impl::calculateVelocity() {
  constexpr float unit{1.f};

  auto velocity{sf::Vector2f{}};
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
    velocity.y -= unit;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
    velocity.y += unit;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
    velocity.x -= unit;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
    velocity.x += unit;
  }
  return velocity;
}

void Scene::impl::sendServerUpdate(const sf::Vector2i &coordinates) {
  common::itf::C2SMessage req;
  auto *update = req.mutable_update();
  auto *game = update->mutable_game();
  auto *position = game->mutable_position();
  position->set_x(coordinates.x);
  position->set_y(coordinates.y);

  if (not connMgr.send(req)) {
    base->change(SceneId::Menu);
    return;
  }
}

sf::Vector2f Scene::impl::calculatePlayerPosition(sf::Vector2f oldVelocity,
                                                  sf::Vector2f newVelocity) {
  if (newVelocity == sf::Vector2f{0.f, 0.f}) {
    return positionFloat;
  }
  constexpr float pixelsPerUs{pixelsPerSecond / 1'000'000};
  float usSinceLastUpdate = updateTime.getElapsedTime().asMicroseconds();
  float distanceInPixels = usSinceLastUpdate * pixelsPerUs;

  const auto &mapSize{base->getWindow().getSize()};
  const auto halfPlayerModelSize{
      sf::Vector2f{circle.getRadius(), circle.getRadius()}};

  auto position{positionFloat};
  if (oldVelocity.x == newVelocity.x) {
    float direction{newVelocity.x};
    position.x =
        std::clamp(position.x + (direction * distanceInPixels),
                   halfPlayerModelSize.x, mapSize.x - halfPlayerModelSize.x);
  }
  if (oldVelocity.y == newVelocity.y) {
    float direction{newVelocity.y};
    position.y =
        std::clamp(position.y + (direction * distanceInPixels),
                   halfPlayerModelSize.y, mapSize.y - halfPlayerModelSize.y);
  }
  return position;
}

void Scene::impl::update() {
  auto newVelocity = calculateVelocity();
  auto oldVelocity = this->velocity;
  this->velocity = newVelocity;

  positionFloat = calculatePlayerPosition(oldVelocity, newVelocity);
  auto newPositionInt = sf::Vector2i{static_cast<int>(positionFloat.x),
                                     static_cast<int>(positionFloat.y)};

  if (positionInt != newPositionInt) {
    positionInt = newPositionInt;

    circle.setPosition(positionFloat);
    sendServerUpdate(positionInt);
  }

  updateTime.restart();
}

void Scene::impl::draw() {
  auto &window{base->getWindow()};

  window.clear(sf::Color::Cyan);
  window.draw(circle);
  window.display();
}

Scene::Scene(SceneManager &sceneMgr)
    : SceneBase(sceneMgr), pimpl(new impl{this}) {}

void Scene::handleEvents() { pimpl->handleEvents(); }

void Scene::update() { pimpl->update(); }

void Scene::draw() { pimpl->draw(); }

void Scene::onEntry() { pimpl->onEntry(); }

void Scene::onLeave() { pimpl->onLeave(); }

} // namespace bm::scene::game
