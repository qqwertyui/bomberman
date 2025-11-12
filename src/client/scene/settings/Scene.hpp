#pragma once

#include "gui/Button.hpp"
#include "gui/Checkbox.hpp"
#include "scene/SceneBase.hpp"

namespace bm::scene::settings {
class Scene : public SceneBase {
public:
  Scene(SceneManager &sceneMgr);

  void handleEvents(const sf::Event &e) override;
  void update() override;
  void draw() override;

private:
  gui::Checkbox fpsCheckbox;
  gui::Button backButton;
  void handleMouseClick(const sf::Mouse::Button &button);
};
} // namespace bm::scene::settings
