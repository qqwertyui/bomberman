#pragma once
#include "Event.hpp"
#include "Keyboard.hpp"

#include <queue>
#include <string>
#include <windows.h>

namespace SimpleSnake {

class Window {
public:
  Window();
  bool pollEvent(Event &event);
  bool isOk() const;

private:
  bool registerWindowClass(const std::string &windowClassName) const;
  static LRESULT globalOnEvent(HWND hwnd, UINT message, WPARAM wParam,
                               LPARAM lParam);
  static Keyboard::Key mapVirtualKeyCodeToKey(WPARAM key, LPARAM flags);
  static std::queue<Event> m_events;
  bool isOkFlag{false};
};

} // namespace SimpleSnake