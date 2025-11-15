#pragma once

#include "Widget.hpp"
#include <string>

namespace bm::gui {
class Checkbox : public Widget {
public:
  Checkbox(const std::string &id, const sf::Vector2f &position,
           const sf::Vector2f &size);
  void draw(sf::RenderTarget &target,
            const sf::RenderStates states) const override;

  void click() override;
  std::string value() const override;

  bool contains(const sf::Vector2f &coords) const override;

private:
  sf::RectangleShape boxShape;
  sf::RectangleShape checkMark;
  bool checked{false};
};
} // namespace bm::gui