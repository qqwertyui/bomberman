#pragma once

#include "ResourceManager.hpp"

namespace sf {
class Texture;
}

namespace SimpleSnake::rsrcManagement {
enum class TextureId { BUTTON_ACTIVE, BUTTON_INACTIVE };

typedef ResourceManager<sf::Texture, TextureId> TextureManager;
}; // namespace SimpleSnake::rsrcManagement