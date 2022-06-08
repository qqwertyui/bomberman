#include "Button.hpp"
#include "rsrcManagement/FontManager.hpp"
#include "rsrcManagement/TextureManager.hpp"

namespace SimpleSnake::interface {
Button::Button(const sf::Vector2f &position, const std::string &label,
               unsigned int characterSize) {
  setActive(false);
  m_buttonSprite.setPosition(position);
  auto buttonSize = m_buttonSprite.getTexture()->getSize();

  auto &fontManager = rsrcManagement::FontManager::instance();
  m_buttonLabel.setFont(fontManager.get(rsrcManagement::FontId::MENU));
  m_buttonLabel.setString(label);
  m_buttonLabel.setCharacterSize(characterSize);

  auto labelRect = m_buttonLabel.getLocalBounds();
  m_buttonLabel.setOrigin(sf::Vector2f(labelRect.left + labelRect.width / 2.f,
                                       labelRect.top + labelRect.height / 2.f));
  m_buttonLabel.setPosition(sf::Vector2f(position.x + buttonSize.x / 2.f,
                                         position.y + buttonSize.y / 2.f));
}

void Button::setActive(bool active) {
  auto &txtManager = rsrcManagement::TextureManager::instance();
  auto &newTexture =
      active ? txtManager.get(rsrcManagement::TextureId::BUTTON_ACTIVE)
             : txtManager.get(rsrcManagement::TextureId::BUTTON_INACTIVE);
  m_buttonSprite.setTexture(newTexture, true);
}

void Button::draw(sf::RenderTarget &target,
                  const sf::RenderStates &states) const {
  sf::RenderStates localStates = states;
  localStates.transform *= getTransform();
  localStates.texture = m_buttonSprite.getTexture();
  target.draw(m_buttonSprite, localStates);
  target.draw(m_buttonLabel, localStates);
}
}; // namespace SimpleSnake::interface