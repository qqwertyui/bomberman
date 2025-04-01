#include "Button.hpp"
#include "resource/FontManager.hpp"
#include "resource/TextureManager.hpp"

namespace bm::interface {
Button::Button(const sf::Vector2f &position, const std::string &label,
               unsigned int characterSize)
    : m_buttonSprite{resource::TextureManager::get().at(
          resource::TextureId::BUTTON_INACTIVE)},
      m_buttonLabel{resource::FontManager::get().at(resource::FontId::MENU)}

{
  m_buttonSprite.setPosition(position);
  auto buttonSize = m_buttonSprite.getTexture().getSize();

  auto &fontManager = resource::FontManager::get();
  m_buttonLabel.setString(label);
  m_buttonLabel.setCharacterSize(characterSize);

  auto labelRect = m_buttonLabel.getLocalBounds();
  m_buttonLabel.setOrigin(
      sf::Vector2f(labelRect.position.x + labelRect.size.x / 2.f,
                   labelRect.position.y + labelRect.size.y / 2.f));
  m_buttonLabel.setPosition(sf::Vector2f(position.x + buttonSize.x / 2.f,
                                         position.y + buttonSize.y / 2.f));
}

void Button::setActive(bool active) {
  auto &txtManager = resource::TextureManager::get();
  auto &newTexture = active
                         ? txtManager.at(resource::TextureId::BUTTON_ACTIVE)
                         : txtManager.at(resource::TextureId::BUTTON_INACTIVE);
  m_buttonSprite.setTexture(newTexture, true);
}
sf::FloatRect Button::getButtonBounds() const {
  return m_buttonSprite.getGlobalBounds();
}
void Button::draw(sf::RenderTarget &target,
                  const sf::RenderStates states) const {
  sf::RenderStates localStates = states;
  localStates.transform *= getTransform();
  localStates.texture = &m_buttonSprite.getTexture();
  target.draw(m_buttonSprite, localStates);
  target.draw(m_buttonLabel, localStates);
}
}; // namespace bm::interface
