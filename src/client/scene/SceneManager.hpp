#pragma once

#include "SceneId.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <map>
#include <memory>

namespace sf {
class RenderWindow;
}

namespace bm::scene {
class SceneBase;
class SharedData;

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
  void setFpsVisible(bool value);

  SharedData &shared();

private:
  SceneBase *getSceneById(const SceneId &sceneId);
  SceneBase &getActive();
  void drawFps();

  std::map<SceneId, SceneBase *> scenes{};
  SceneBase *active{nullptr};
  SharedData *sharedData{nullptr};
  sf::Clock fpsClock;
  bool showFps{false};
  bool hasFocus{true};
  sf::Text fpsText;
  unsigned int frameCount{0};
};

} // namespace bm::scene
