#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

namespace bomberman::scene {
class SceneManager;

enum class SceneId : unsigned int {
  Menu,
  Settings,
  Pause,
  Running,
  NumberOfScenes
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
  inline SceneId getId() const { return m_sceneId; }

protected:
  const SceneId m_sceneId;
  sf::RenderWindow &m_window;
  SceneManager &m_sceneMgr;
};
} // namespace bomberman::scene