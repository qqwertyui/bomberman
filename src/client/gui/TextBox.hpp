#pragma once

#include <SFML/Graphics.hpp>

namespace bm::gui {
class TextBox : public sf::Drawable, public sf::Transformable {
public:
  TextBox(const sf::Vector2f &position, const sf::Vector2f &size,
          const std::string &placeholder = "", unsigned int characterSize = 15,
          std::size_t maxLength = 20);

  void handleEvent(const sf::Event &e);
  void draw(sf::RenderTarget &target,
            const sf::RenderStates states) const override;
  void setActive(bool active);
  void updateCursor();

private:
  bool m_isActive;
  std::size_t m_maxLength;
  sf::RectangleShape m_boxShape;
  sf::Text m_text;
  sf::Text m_placeHolder;
  std::string m_inputText;
  sf::RectangleShape m_cursor;
  sf::Clock m_cursorClock;
  std::size_t m_cursorPosition;
  bool m_cursorActive;
};
} // namespace bm::gui
