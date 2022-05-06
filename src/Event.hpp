#pragma once

#include "Keyboard.hpp"
#include "utils/Vector2.hpp"

namespace SimpleSnake {

struct Event {
  enum class Type : int { None, KeyPressed, MouseMoved };
  struct KeyboardEvent {
    Keyboard::Key key;
  };
  struct MouseMovedEvent {
    Vector2u pos;
  };
  union Value {
    KeyboardEvent keyboard;
    MouseMovedEvent mouse;
  };

  Type type;
  Value value;
};

} // namespace SimpleSnake