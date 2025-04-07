#include "Scene.hpp"

#include "GlobalConfig.hpp"
#include "MessageHandler.hpp"
#include "Player.hpp"
#include "common/Log.hpp"
#include "common/itf/core.pb.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
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

  Scene *base;
  std::unique_ptr<Player> player;
  std::unique_ptr<MessageHandler> msgHandler;
};

Scene::impl::impl(Scene *base) : base(base) {}

void Scene::impl::onEntry() {
  msgHandler = std::make_unique<MessageHandler>(
      GlobalConfig::get().serverIp(), GlobalConfig::get().serverPort());

  if (not msgHandler->isConnected()) {
    LOG_ERR("Couldn't connect to server");
    base->change(SceneId::Lobby);
  }

  player = std::make_unique<Player>(base->getWindow());
}

void Scene::impl::onLeave() {
  msgHandler.reset();
  player.reset();
}

void Scene::impl::handleEvents() {
  auto &window{base->getWindow()};

  while (const auto &e = window.pollEvent()) {
    if (e->is<sf::Event::Closed>()) {
      window.close();
    } else if (const auto *keyPressed = e->getIf<sf::Event::KeyPressed>()) {
      handleKeyPressed(keyPressed->scancode);
    }
  }
}

void Scene::impl::handleKeyPressed(const sf::Keyboard::Scancode &scancode) {
  if (scancode == sf::Keyboard::Scancode::Escape) {
    base->change(SceneId::Lobby);
    return;
  }
}

void Scene::impl::update() {
  common::itf::C2SMessage req;
  player->update(req);
  msgHandler->handle();

  if (req.has_update()) {
    auto success = msgHandler->send(req);
    if (not success) {
      LOG_ERR("Couldn't send an update to the server, leaving...");
      base->change(SceneId::Menu);
    }
  }
}

void Scene::impl::draw() {
  auto &window{base->getWindow()};

  window.clear(sf::Color::Cyan);
  window.draw(*player);
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
