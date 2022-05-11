#include "Button.hpp"
#include "rsrcManagement/TextureManager.hpp"

namespace SimpleSnake::interface {
Button::Button(const std::string &label) {
  auto txtManager = rsrcManagement::TextureManager::instance();
  auto &texture = txtManager.get(rsrcManagement::TextureId::BUTTON_NORMAL);
  m_buttonSprite.setTexture(texture);
}

void Button::draw(sf::RenderTarget &target,
                  const sf::RenderStates &states) const {
  target.draw(m_buttonSprite, states);
}
}; // namespace SimpleSnake::interface