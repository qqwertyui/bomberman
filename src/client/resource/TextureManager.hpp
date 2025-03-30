#pragma once

#include "ResourceManager.hpp"

namespace sf {
class Texture;
}

namespace bm::resource {
enum class TextureId { BUTTON_ACTIVE, BUTTON_INACTIVE };

typedef ResourceManager<sf::Texture, TextureId> TextureManager;

}; // namespace bm::resource
