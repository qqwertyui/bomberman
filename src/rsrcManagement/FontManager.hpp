#pragma once

#include "ResourceManager.hpp"

namespace sf {
class Font;
}

namespace SimpleSnake::rsrcManagement {
enum class FontId { MENU };

typedef ResourceManager<sf::Font, FontId> FontManager;
}; // namespace SimpleSnake::rsrcManagement