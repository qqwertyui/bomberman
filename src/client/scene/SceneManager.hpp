#pragma once

#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Clock.hpp"
#include "SceneId.hpp"
#include <map>

namespace sf {
class RenderWindow;
}

namespace bm::scene {
class SceneBase;

class SceneManager {
public:
  SceneManager(sf::RenderWindow &window);
  ~SceneManager();

  void add(SceneId sceneId, SceneBase *scene);
  void remove(const SceneId &sceneId);
  void change(const SceneId &sceneId);

  void handleEvents();
  void update();
  void draw();

  sf::RenderWindow &getWindow();

private:
  SceneBase *getSceneById(const SceneId &sceneId);
  SceneBase &getActive();

  std::map<SceneId, SceneBase *> scenes{};
  SceneBase *active{nullptr};
  sf::RenderWindow &window;
  void drawFps();
  sf::Clock fpsClock;
  sf::Text fpsText;
  unsigned int frameCount{0};
  float elapsedTime{0.0f};
};

} // namespace bm::scene
