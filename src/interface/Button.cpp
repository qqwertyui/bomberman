#include "Button.hpp"
#include "rsrcManagement/TextureManager.hpp"

namespace SimpleSnake::interface {
Button::Button(const std::string &label) { setActive(false); }

void Button::setActive(bool active) {
  auto &txtManager = rsrcManagement::TextureManager::instance();
  auto &newTexture =
      active ? txtManager.get(rsrcManagement::TextureId::BUTTON_ACTIVE)
             : txtManager.get(rsrcManagement::TextureId::BUTTON_INACTIVE);
  m_buttonSprite.setTexture(newTexture);
}

void Button::draw(sf::RenderTarget &target,
                  const sf::RenderStates &states) const {
  sf::RenderStates localStates = states;
  localStates.transform *= getTransform();
  localStates.texture = m_buttonSprite.getTexture();
  target.draw(m_buttonSprite, localStates);
}
}; // namespace SimpleSnake::interface