#pragma once

#include "interface/Button.hpp"
#include "scene/SceneBase.hpp"
#include <map>

namespace bm::scene::menu {
class Scene : public SceneBase {
public:
  Scene(SceneManager &sceneMgr, sf::RenderWindow &window,
        const SceneId &sceneId);

  void handleEvents() override;
  void update() override;
  void draw() override;

private:
  void handleKeyEvent(const sf::Keyboard::Scancode &scancode);

  enum class ButtonId : unsigned int { Exit, Settings, Start, NumberOfButtons };
  ButtonId m_activeButton{ButtonId::Start};
  std::map<ButtonId, interface::Button> buttons;
};

} // namespace bm::scene::menu
