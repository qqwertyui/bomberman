#pragma once

#include "ResourceManager.hpp"

namespace sf {
class Texture;
}

namespace bomberman::rsrcManagement {
enum class TextureId { BUTTON_ACTIVE, BUTTON_INACTIVE };

typedef ResourceManager<sf::Texture, TextureId> TextureManager;
}; // namespace bomberman::rsrcManagement