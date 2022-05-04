#pragma once

#include "Keyboard.hpp"

namespace SimpleSnake {

struct Event {
  enum class Type : int { None, KeyPressed, MouseMoved };
  struct KeyboardEvent {
    Keyboard::Key code;
  };
  struct MouseEvent {
    unsigned int x;
    unsigned int y;
  };
  union Value {
    KeyboardEvent keyboard;
    MouseEvent mouse;
  };

  Type type;
  Value value;
};

} // namespace SimpleSnake