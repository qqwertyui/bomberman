#pragma once

#include "ResourceManager.hpp"

namespace sf {
class Font;
}

namespace bm::rsrcManagement {
enum class FontId { MENU };

typedef ResourceManager<sf::Font, FontId> FontManager;

}; // namespace bm::rsrcManagement
