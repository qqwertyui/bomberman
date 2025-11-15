#pragma once

#include "Widget.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

namespace bm::gui {
class Button : public Widget {
public:
  Button(const sf::Vector2f &position, const std::string &label,
         const std::function<void()> &callback,
         unsigned int characterSize = 25);
  void draw(sf::RenderTarget &target,
            const sf::RenderStates states) const override;
  bool contains(const sf::Vector2f &coords) const override;
  sf::FloatRect getButtonBounds() const;

  void onActivate() override;
  void onHoverStart() override;
  void onHoverStop() override;

private:
  std::function<void()> callback{};
  sf::Sprite m_buttonSprite;
  sf::Text m_buttonLabel;
};
} // namespace bm::gui
