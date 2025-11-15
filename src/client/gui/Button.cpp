#include "Button.hpp"
#include "resource/FontManager.hpp"
#include "resource/TextureManager.hpp"

namespace bm::gui {
Button::Button(const std::string &id, const sf::Vector2f &position,
               const std::string &label, const std::function<void()> &callback,
               unsigned int characterSize)
    : Widget(id), callback{callback},
      m_buttonSprite{resource::TextureManager::get().at(
          resource::TextureId::BUTTON_INACTIVE)},
      m_buttonLabel{resource::FontManager::get().at(resource::FontId::MENU)} {
  m_buttonSprite.setPosition(position);
  auto buttonSize = m_buttonSprite.getTexture().getSize();

  m_buttonLabel.setString(label);
  m_buttonLabel.setCharacterSize(characterSize);

  auto labelRect = m_buttonLabel.getLocalBounds();
  m_buttonLabel.setOrigin(
      sf::Vector2f(labelRect.position.x + labelRect.size.x / 2.f,
                   labelRect.position.y + labelRect.size.y / 2.f));
  m_buttonLabel.setPosition(sf::Vector2f(position.x + buttonSize.x / 2.f,
                                         position.y + buttonSize.y / 2.f));
}

void Button::click() { callback(); }

void Button::hover() {
  auto &txtManager = resource::TextureManager::get();
  m_buttonSprite.setTexture(txtManager.at(resource::TextureId::BUTTON_ACTIVE),
                            true);
}

void Button::reset() {
  auto &txtManager = resource::TextureManager::get();
  m_buttonSprite.setTexture(txtManager.at(resource::TextureId::BUTTON_INACTIVE),
                            true);
}

bool Button::contains(const sf::Vector2f &coords) const {
  return m_buttonSprite.getGlobalBounds().contains(coords);
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
}; // namespace bm::gui
