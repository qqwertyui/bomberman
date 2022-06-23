#pragma once

#include "ResourceManager.hpp"

namespace sf {
class Font;
}

namespace bomberman::rsrcManagement {
enum class FontId { MENU };

typedef ResourceManager<sf::Font, FontId> FontManager;
}; // namespace bomberman::rsrcManagement