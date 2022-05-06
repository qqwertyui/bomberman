#pragma once
#include "Event.hpp"

#include <memory>
#include <queue>
#include <string>
#include <windows.h>

namespace SimpleSnake {

class Window {
public:
  Window(const Window &) = delete;
  Window(Window &&) = delete;
  Window &operator=(const Window &) = delete;
  Window &operator=(Window &&) = delete;
  ~Window() = default;

  static std::unique_ptr<Window> create();
  bool pollEvent(Event &event);

private:
  Window() = default;
  LRESULT handleEvents(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

  static bool registerWindowClass(const std::string &windowClassName);
  static LRESULT CALLBACK globalOnEvent(HWND hwnd, UINT message, WPARAM wParam,
                                        LPARAM lParam);
  std::queue<Event> m_events;
};

} // namespace SimpleSnake