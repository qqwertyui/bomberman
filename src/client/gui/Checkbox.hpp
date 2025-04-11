#pragma once

#include <SFML/Graphics.hpp>

namespace bm::gui {

class Checkbox : public sf::Drawable, public sf::Transformable {
public:
  Checkbox(const sf::Vector2f &position, const sf::Vector2f &size);
  void handleEvent(const sf::Event &e);
  void draw(sf::RenderTarget &target,
            const sf::RenderStates states) const override;
  bool isChecked() const;

private:
  sf::RectangleShape boxShape;
  sf::RectangleShape checkMark;
  bool checked{false};
};
} // namespace bm::gui