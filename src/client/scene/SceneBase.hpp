#pragma once

#include <SFML/Graphics.hpp>

namespace bm::scene {
class SceneManager;

enum class SceneId : unsigned int { Menu, Settings, Pause, Lobby, Game };

class SceneBase {
public:
  SceneBase(SceneManager &sceneMgr, sf::RenderWindow &window)
      : m_sceneMgr(sceneMgr), m_window(window) {}
  ~SceneBase() = default;

  virtual void handleEvents() = 0;
  virtual void update() = 0;
  virtual void draw() = 0;
  virtual void onEntry(){};
  virtual void onLeave(){};
  void change(const SceneId &sceneId);

protected:
  sf::RenderWindow &m_window;
  SceneManager &m_sceneMgr;
};

} // namespace bm::scene
