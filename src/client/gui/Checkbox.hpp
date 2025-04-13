#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

namespace bm::gui {

class Checkbox : public sf::Drawable, public sf::Transformable {
public:
  Checkbox(const sf::Vector2f &position, const sf::Vector2f &size);
  void handleEvent(const sf::Event &e);
  void draw(sf::RenderTarget &target,
            const sf::RenderStates states) const override;
  bool isChecked() const;
  void setCallback(std::function<void(bool)> callback);

private:
  sf::RectangleShape boxShape;
  sf::RectangleShape checkMark;
  bool checked{false};
  std::function<void(bool)> callback;
};
} // namespace bm::gui