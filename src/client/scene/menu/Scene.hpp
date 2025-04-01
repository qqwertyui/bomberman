#pragma once

#include "interface/Button.hpp"
#include "scene/SceneBase.hpp"
#include <map>

namespace bm::scene::menu {
class Scene : public SceneBase {
public:
  Scene(SceneManager &sceneMgr);

  void handleEvents() override;
  void update() override;
  void draw() override;

private:
  void handleKeyEvent(const sf::Keyboard::Scancode &scancode);
  void handleMouseEvent(const sf::Mouse::Button &button);
  enum class ButtonId : unsigned int {
    Exit,
    Settings,
    Start,
    NumberOfButtons,
    None
  };
  ButtonId m_activeButton{ButtonId::Start};
  std::map<ButtonId, interface::Button> buttons;
};

} // namespace bm::scene::menu
