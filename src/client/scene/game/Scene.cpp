#include "Scene.hpp"

#include "GlobalConfig.hpp"
#include "MessageHandler.hpp"
#include "Player.hpp"
#include "PlayerModel.hpp"
#include "common/itf/core.pb.h"
#include "common/logging/Log.hpp"
#include "scene/SharedData.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <thread>

namespace bm::scene::game {
struct Scene::impl {
  impl(Scene *base);

  void onEntry();
  void onLeave();

  void handleEvents(const sf::Event &e);
  void update();
  void draw();

  void handleKeyPressed(const sf::Keyboard::Scancode &scancode);

  Scene *base;
  std::unique_ptr<Player> player;
  std::unique_ptr<MessageHandler> msgHandler;
};

Scene::impl::impl(Scene *base) : base(base) {}

void Scene::impl::onEntry() {
  msgHandler = std::make_unique<MessageHandler>(base->shared());
  player = std::make_unique<Player>(base->shared().window, base->shared());
}

void Scene::impl::onLeave() {
  msgHandler.reset();
  player.reset();
  for (auto &[id, enemy] : base->shared().gameContext.enemies) {
    delete enemy;
  }
  base->shared().gameContext.enemies.clear();
}

void Scene::impl::handleEvents(const sf::Event &e) {
  const auto *keyPressed = e.getIf<sf::Event::KeyPressed>();
  if (keyPressed and (keyPressed->scancode == sf::Keyboard::Scancode::Escape)) {
    base->change(SceneId::Lobby);
  }
}

void Scene::impl::update() {
  msgHandler->handle();

  if (not base->shared().isWindowFocused) {
    return;
  }
  common::itf::C2SMessage req;
  player->update(req);
  if (req.has_update() and (not msgHandler->send(req))) {
    LOG_ERR("Lost connection with the server");
    base->change(SceneId::Menu);
  }
}

void Scene::impl::draw() {
  auto &window{base->shared().window};

  window.clear(sf::Color::Cyan);
  window.draw(*player);
  for (auto &[id, enemy] : base->shared().gameContext.enemies) {
    window.draw(*enemy);
  }
}

Scene::Scene(SceneManager &sceneMgr)
    : SceneBase(sceneMgr), pimpl(new impl{this}) {}

void Scene::handleEvents(const sf::Event &e) { pimpl->handleEvents(e); }

void Scene::update() { pimpl->update(); }

void Scene::draw() { pimpl->draw(); }

void Scene::onEntry() { pimpl->onEntry(); }

void Scene::onLeave() { pimpl->onLeave(); }

} // namespace bm::scene::game
