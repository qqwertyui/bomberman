#pragma once

#include <SFML/Graphics.hpp>

namespace SimpleSnake::scene {
class SceneManager;

enum class SceneId : unsigned int {
  Default = 0,
  Menu = 1,
  Pause = 2,
  Running = 3
};

class Scene {
public:
  Scene(SceneManager &sceneMgr, sf::RenderWindow &window,
        const SceneId &sceneId)
      : m_sceneMgr(sceneMgr), m_window(window), m_sceneId(sceneId) {}
  ~Scene() = default;

  virtual void handleEvents() = 0;
  virtual void update() = 0;
  virtual void draw() = 0;
  void change(const SceneId &sceneId);
  SceneId getId() const { return m_sceneId; }

protected:
  const SceneId m_sceneId;
  sf::RenderWindow &m_window;
  SceneManager &m_sceneMgr;
};
} // namespace SimpleSnake::scene