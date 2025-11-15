#pragma once

#include "Widget.hpp"
#include <SFML/Graphics.hpp>

namespace bm::gui {
constexpr int minPrintableAscii{32};
constexpr int maxPrintableAscii{127};
constexpr int backspaceAscii{8};

class TextBox : public Widget {
public:
  TextBox(const sf::Vector2f &position, const sf::Vector2f &size,
          const std::string &placeholder = "", unsigned int characterSize = 15,
          std::size_t maxLength = 20);

  void draw(sf::RenderTarget &target,
            const sf::RenderStates states) const override;
  bool contains(const sf::Vector2f &coords) const override;
  void updateCursor();

  void onActivate() override;
  void onDeactivate() override;
  void onHoverStart() override;
  void onHoverStop() override;
  void onEvent(const sf::Event &e);

private:
  bool m_isActive;
  sf::Text m_text;
  std::size_t m_maxLength;
  sf::RectangleShape m_boxShape;
  sf::Text m_placeHolder;
  std::string m_inputText;
  sf::RectangleShape m_cursor;
  sf::Clock m_cursorClock;
  bool m_cursorActive;
  std::size_t m_cursorPosition;
};
} // namespace bm::gui
