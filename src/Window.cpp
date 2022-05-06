#include "Window.hpp"
#include "Keyboard.hpp"
#include "utils/Log.hpp"
#include "utils/Vector2.hpp"

namespace SimpleSnake {

std::unique_ptr<Window> Window::create() {
  auto window = std::unique_ptr<Window>(new Window);
  const std::string windowClassName{"SimpleSnake_WindowClass"};
  if (not registerWindowClass(windowClassName)) {
    LOG("Failed to register window class\n");
    return nullptr;
  }
  HWND hwnd = CreateWindow(windowClassName.c_str(), "Snake",
                           WS_OVERLAPPEDWINDOW, 200, 200, 200, 200, nullptr,
                           nullptr, GetModuleHandle(nullptr), window.get());
  if (not hwnd) {
    LOG("Failed to create window\n");
    return nullptr;
  }
  ShowWindow(hwnd, SW_SHOW);
  UpdateWindow(hwnd);
  return window;
}

bool Window::registerWindowClass(const std::string &windowClassName) {
  WNDCLASS windowClass{};
  windowClass.lpfnWndProc = &Window::globalOnEvent;
  windowClass.hInstance = GetModuleHandleA(nullptr);
  windowClass.lpszClassName = windowClassName.c_str();
  return RegisterClass(&windowClass);
}

bool Window::pollEvent(Event &event) {
  MSG message;
  while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }
  if (m_events.empty()) {
    return false;
  }
  event = m_events.front();
  m_events.pop();
  return true;
}

LRESULT CALLBACK Window::globalOnEvent(HWND hwnd, UINT message, WPARAM wParam,
                                       LPARAM lParam) {
  if (message == WM_CREATE) {
    auto window = reinterpret_cast<LONG_PTR>(
        reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, window);
  }
  auto window =
      hwnd ? reinterpret_cast<Window *>(GetWindowLongPtr(hwnd, GWLP_USERDATA))
           : nullptr;
  if (window) {
    return window->handleEvents(hwnd, message, wParam, lParam);
  }
  return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT Window::handleEvents(HWND hwnd, UINT message, WPARAM wParam,
                             LPARAM lParam) {
  switch (message) {
  case WM_KEYDOWN:
  case WM_SYSKEYDOWN: {
    Event event;
    event.type = Event::Type::KeyPressed;
    event.value.keyboard.key = Keyboard::mapVirtualKeyCodeToKey(wParam, lParam);
    m_events.push(event);
    break;
  }
  case WM_MOUSEMOVE: {
    Event event;
    event.type = Event::Type::MouseMoved;
    event.value.mouse.pos = Vector2u(LOWORD(lParam), HIWORD(lParam));
    m_events.push(event);
    break;
  }
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hwnd, message, wParam, lParam);
  }
  return 0;
}

} // namespace SimpleSnake