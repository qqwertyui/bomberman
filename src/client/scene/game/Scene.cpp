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
  void updateVelocity(const sf::Keyboard::Scancode &scancode, float velocity);

  std::optional<sf::Vector2f> calculatePlayerPosition();
  void sendServerUpdate(const sf::Vector2f &coordinates);

  Scene *base;

  sf::CircleShape circle;
  sf::Vector2f position;
  sf::Vector2f velocity;
  sf::Clock updateTime;
  common::ConnectionManager connMgr;

  static void messageReceiverLoop(common::ConnectionManager *connMgr);
  std::thread serverMessageHandler;

  static constexpr float pixelsPerSecond{300};
};

Scene::impl::impl(Scene *base) : base(base) {
  constexpr float playerSize{60.f};
  circle.setRadius(playerSize / 2.f);
  circle.setFillColor(sf::Color::Red);

  const auto radius{circle.getRadius()};
  circle.setOrigin(sf::Vector2f(radius, radius));

  position = sf::Vector2f{radius, radius};
  circle.setPosition(position);

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

  connMgr->disconnect();
}

void Scene::impl::onEntry() {
  serverMessageHandler = std::thread{messageReceiverLoop, &connMgr};
}

void Scene::impl::onLeave() { serverMessageHandler.join(); }

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
  auto &window{base->getWindow()};
  if (scancode == sf::Keyboard::Scancode::Escape) {
    window.close();
    return;
  }

  constexpr float normalVelocity{1.f};
  updateVelocity(scancode, normalVelocity);
}

void Scene::impl::handleKeyReleased(const sf::Keyboard::Scancode &scancode) {
  constexpr float zeroVelocity{0.f};
  updateVelocity(scancode, zeroVelocity);
}

void Scene::impl::updateVelocity(const sf::Keyboard::Scancode &scancode,
                                 float velocity) {
  if (scancode == sf::Keyboard::Scancode::Up) {
    this->velocity.y = -velocity;
  } else if (scancode == sf::Keyboard::Scancode::Down) {
    this->velocity.y = velocity;
  } else if (scancode == sf::Keyboard::Scancode::Right) {
    this->velocity.x = velocity;
  } else if (scancode == sf::Keyboard::Scancode::Left) {
    this->velocity.x = -velocity;
  }
}

void Scene::impl::sendServerUpdate(const sf::Vector2f &coordinates) {
  common::itf::C2SMessage req;
  auto *update = req.mutable_update();
  auto *game = update->mutable_game();
  auto *position = game->mutable_position();
  position->set_x(coordinates.x);
  position->set_y(coordinates.y);

  if (not connMgr.send(req)) {
    LOG_ERR("Connection closed");
    base->change(SceneId::Menu);
    return;
  }
}

std::optional<sf::Vector2f> Scene::impl::calculatePlayerPosition() {
  if (velocity == sf::Vector2f{0.f, 0.f}) {
    return std::nullopt;
  }
  constexpr float pixelsPerMs{pixelsPerSecond / 1000};
  unsigned int msSinceLastUpdate = updateTime.getElapsedTime().asMilliseconds();
  float pixelsMoved = msSinceLastUpdate * pixelsPerMs;

  const auto &mapSize{base->getWindow().getSize()};
  const auto halfSize{circle.getRadius()};

  sf::Vector2f coords{position.x, position.y};
  coords.x = std::clamp(coords.x + (velocity.x * pixelsMoved), halfSize,
                        mapSize.x - halfSize);
  coords.y = std::clamp(coords.y + (velocity.y * pixelsMoved), halfSize,
                        mapSize.y - halfSize);

  return (coords == position) ? std::nullopt : std::make_optional(coords);
}

void Scene::impl::update() {
  auto coords = calculatePlayerPosition();
  if (coords) {
    this->position = *coords;
    circle.setPosition(*coords);
    sendServerUpdate(*coords);
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
