#include "Running.hpp"
#include "GlobalConfig.hpp"
#include "common/ConnectionManager.hpp"
#include "common/Log.hpp"
#include "common/messages/core.pb.h"

namespace bomberman::scene {

Running::Running(SceneManager &sceneMgr, sf::RenderWindow &window,
                 const SceneId &sceneId)
    : Scene(sceneMgr, window, sceneId) {}

void Running::handleEvents() {
  while (const auto &e = m_window.pollEvent()) {
    if (e->is<sf::Event::Closed>()) {
      m_window.close();
    } else if (const auto *keyPressed = e->getIf<sf::Event::KeyPressed>()) {
      if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
        LOG_DBG("Sending query");
        bomberman::C2SMessage msg;
        msg.mutable_query()->set_version(true);
        connMgr.send(msg);
      } else if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
        change(SceneId::Menu);
      } else {
        LOG_DBG("Running key pressed!");
      }
    }
  }
}

void Running::onEntry() {
  x = 10;
  y = 10;
  connMgr.connect(GlobalConfig::get().serverIp(),
                  GlobalConfig::get().serverPort());

  bomberman::C2SMessage req;
  auto *query = req.mutable_query();
  query->set_lobbies(true);
  if (not connMgr.send(req)) {
    LOG_ERR("Couldn't connect to server");
    connMgr.disconnect();
    return;
  }

  auto resp = connMgr.receive<bomberman::S2CMessage>();
  if (not resp.has_value()) {
    LOG_ERR("Couldn't connect to server");
    connMgr.disconnect();
    return;
  }

  if (not resp->has_query()) {
    LOG_INF("No active lobbies");
    return;
  }

  LOG_INF("Active lobbies: ");
  for (const auto &lobby : resp->query().lobbies()) {
    LOG_INF("[id: %d]: %d/%d", lobby.id(), lobby.connectedplayers(),
            lobby.maxplayers());
  }
}

void Running::onLeave() { connMgr.disconnect(); }

void Running::update() {}

void Running::draw() {
  m_window.clear(sf::Color::Blue);
  m_window.display();
}

} // namespace bomberman::scene