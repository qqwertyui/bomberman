#pragma once

#include "client/GlobalConfig.hpp"
#include "gui/Button.hpp"
#include "gui/TextBox.hpp"
#include "scene/SceneBase.hpp"
#include <map>

namespace bm::scene::menu {
class Scene : public SceneBase {
public:
  Scene(SceneManager &sceneMgr);

  void handleEvents(const sf::Event &e) override;
  void update() override;
  void draw() override;

private:
  void handleKeyEvent(const sf::Keyboard::Scancode &scancode);
  void handleMouseClick(const sf::Mouse::Button &button);
  gui::TextBox textBox;
  enum class ButtonId : unsigned int {
    Start,
    Settings,
    Exit,
    NumberOfButtons,
  };
  ButtonId m_activeButton{ButtonId::Start};
  std::map<ButtonId, gui::Button> buttons;
  bool keyboardActive{false};
};
} // namespace bm::scene::menu
