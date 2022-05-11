#pragma once

#include "interface/Button.hpp"
#include "scene/Scene.hpp"
#include <memory>

namespace SimpleSnake::scene {
class Menu : public Scene {
public:
  Menu(SceneManager &sceneMgr, sf::RenderWindow &window,
       const SceneId &sceneId);

  void handleEvents() override;
  void update() override;
  void draw() override;

private:
  void handleKeyEvent(const sf::Event::KeyEvent &keyEvent);

  enum class ButtonId : unsigned int { Exit, Settings, Start, NumberOfButtons };
  ButtonId m_activeButton{ButtonId::Start};

  std::vector<std::unique_ptr<interface::Button>> buttons;
};
} // namespace SimpleSnake::scene