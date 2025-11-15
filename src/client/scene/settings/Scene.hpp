#pragma once

#include "gui/Checkbox.hpp"
#include "gui/WidgetManager.hpp"
#include "scene/SceneBase.hpp"

namespace bm::scene::settings {
class Scene : public SceneBase {
public:
  Scene(SceneManager &sceneMgr);

  void handleEvents(const sf::Event &e) override;
  void update() override;
  void draw() override;

private:
  gui::WidgetManager widgetManager;
};
} // namespace bm::scene::settings
