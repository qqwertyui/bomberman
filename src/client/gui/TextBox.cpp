#include "TextBox.hpp"
#include "resource/FontManager.hpp"
namespace bm::gui {
TextBox::TextBox(const sf::Vector2f &position, const sf::Vector2f &size,
                 const std::string &placeholder, unsigned int characterSize,
                 std::size_t maxLength)
    : m_isActive(false),
      m_text(resource::FontManager::get().at(resource::FontId::MENU)),
      m_maxLength(maxLength),
      m_placeHolder(resource::FontManager::get().at(resource::FontId::MENU)),
      m_cursorActive(true), m_cursorPosition(0) {

  m_boxShape.setPosition(position);
  m_boxShape.setSize(size);
  m_boxShape.setFillColor(sf::Color::White);
  m_boxShape.setOutlineThickness(3.f);
  m_boxShape.setOutlineColor(sf::Color::Black);

  m_text.setString("");
  m_text.setCharacterSize(characterSize);
  m_text.setFillColor(sf::Color::Black);
  auto textRect = m_text.getLocalBounds();
  m_text.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.f,
                                textRect.position.y + textRect.size.y / 2.f));
  m_text.setPosition(m_boxShape.getPosition());

  m_placeHolder.setString(placeholder);
  m_placeHolder.setCharacterSize(characterSize);
  m_placeHolder.setFillColor(sf::Color::Black);
  auto placeHolderRect = m_placeHolder.getLocalBounds();
  m_placeHolder.setOrigin(
      sf::Vector2f(placeHolderRect.position.x + placeHolderRect.size.x / 2.f,
                   placeHolderRect.position.y + placeHolderRect.size.y / 2.f));
  m_placeHolder.setPosition(
      sf::Vector2f(m_boxShape.getPosition().x + m_boxShape.getSize().x / 2.f,
                   m_boxShape.getPosition().y + m_boxShape.getSize().y / 2.f));

  m_cursor.setSize({3.f, static_cast<float>(characterSize) * 1.5f});
  m_cursor.setFillColor(sf::Color::Black);
  m_cursor.setPosition(m_boxShape.getPosition());
}

void TextBox::handleEvent(const sf::Event &e) {
  if (const auto *mousePressed = e.getIf<sf::Event::MouseButtonPressed>()) {
    sf::Vector2f mousePos(mousePressed->position);
    setActive(m_boxShape.getGlobalBounds().contains(mousePos));
  }
  if (m_isActive) {
    if (const auto *textEntered = e.getIf<sf::Event::TextEntered>()) {
      if (textEntered->unicode >= minPrintableAscii &&
          textEntered->unicode < maxPrintableAscii) {
        if (m_inputText.size() < m_maxLength) {
          m_inputText.insert(m_cursorPosition, 1,
                             static_cast<char>(textEntered->unicode));
          m_cursorPosition++;
          m_text.setString(m_inputText);
        }
      } else if (textEntered->unicode == backspaceAscii &&
                 m_cursorPosition > 0) {
        if (!m_inputText.empty()) {
          m_inputText.erase(m_cursorPosition - 1, 1);
          m_cursorPosition--;
          m_text.setString(m_inputText);
        }
      }
    } else if (const auto *keyPressed = e.getIf<sf::Event::KeyPressed>()) {
      if (keyPressed->scancode == sf::Keyboard::Scancode::Left) {
        if (m_cursorPosition > 0) {
          m_cursorPosition--;
        }
      } else if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
        if (m_cursorPosition < m_inputText.size()) {
          m_cursorPosition++;
        }
      }
    }
    auto textBounds = m_text.findCharacterPos(m_cursorPosition);
    m_cursor.setPosition(sf::Vector2f(textBounds.x, m_text.getPosition().y));
  }
}

void TextBox::draw(sf::RenderTarget &target,
                   const sf::RenderStates states) const {
  sf::RenderStates localStates = states;
  localStates.transform *= getTransform();

  const_cast<TextBox *>(this)->updateCursor();

  target.draw(m_boxShape, localStates);

  if (m_inputText.empty() && !m_isActive) {
    target.draw(m_placeHolder, localStates);
  } else {
    target.draw(m_text, localStates);
  }
  if (m_isActive && m_cursorActive) {
    target.draw(m_cursor, localStates);
  }
}

void TextBox::setActive(bool active) {
  m_isActive = active;
  m_boxShape.setOutlineColor(active ? sf::Color::Green : sf::Color::Black);
}

void TextBox::updateCursor() {
  if (m_cursorClock.getElapsedTime().asSeconds() <= 0.5f) {
    return;
  }
  m_cursorActive = !m_cursorActive;
  m_cursorClock.restart();
}
} // namespace bm::gui
