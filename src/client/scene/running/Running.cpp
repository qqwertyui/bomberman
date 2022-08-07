#include "Running.hpp"
#include "GlobalConfig.hpp"
#include "common/ConnectionManager.hpp"
#include "common/Log.hpp"
#include "messages/playerPosition.pb.h"
#include "messages/serverInfo.pb.h"

namespace bomberman::scene {

Running::Running(SceneManager &sceneMgr, sf::RenderWindow &window,
                 const SceneId &sceneId)
    : Scene(sceneMgr, window, sceneId) {}

void Running::handleEvents() {
  static int x = 10, y = 10;
  sf::Event e;
  while (m_window.pollEvent(e)) {
    if (e.type == sf::Event::Closed) {
      m_window.close();
    } else if (e.type == sf::Event::KeyPressed) {
      if (e.key.code == sf::Keyboard::Enter) {
        LOG_DBG("Sending position event");
        bomberman::PlayerPosition playerPos;
        playerPos.set_positionx(x--);
        playerPos.set_positiony(y--);
        connMgr.send(playerPos);
      } else if (e.key.code == sf::Keyboard::Escape) {
        change(SceneId::Menu);
      } else {
        LOG_DBG("Running key pressed!");
      }
    }
  }
}

void Running::onEntry() {
  connMgr.connect(GlobalConfig::get().serverIp(),
                  GlobalConfig::get().serverPort());
  auto si = connMgr.receive<bomberman::ServerInfo>();
  if (not si.has_value()) {
    LOG_ERR("Couldn't connect to server");
    connMgr.disconnect();
    return;
  }
  LOG_INF("Active lobbies: ");
  for (const auto &lobby : si.value().lobbies()) {
    LOG_INF("id: %d | players: %d", lobby.id(), lobby.players());
  }
}

void Running::onLeave() { connMgr.disconnect(); }

void Running::update() {}

void Running::draw() {
  m_window.clear(sf::Color::Blue);
  m_window.display();
}

} // namespace bomberman::scene