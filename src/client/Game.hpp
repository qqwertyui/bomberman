#pragma once
#include "GlobalConfig.hpp"
#include "scene/SceneManager.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

namespace bm {
class Game {
public:
  int run(int argc, char **argv);

private:
  bool initialize(int argc, char **argv);
  void handleEvents();
  void update();
  void draw();

  std::unique_ptr<scene::SceneManager>
  createSceneManager(sf::RenderWindow &window);

  std::unique_ptr<scene::SceneManager> m_sceneMgr;
  std::unique_ptr<sf::RenderWindow> m_window;
};

} // namespace bm
