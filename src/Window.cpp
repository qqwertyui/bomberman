#include "Window.hpp"
#include <iostream>

namespace SimpleSnake {

std::queue<Event> Window::m_events{};

Window::Window() {
  const std::string windowClassName{"SimpleSnake_Window"};
  if (not registerWindowClass(windowClassName)) {
    std::cerr << "Failed to register window class\n";
    return;
  }
  HWND hwnd = CreateWindowEx(0, windowClassName.c_str(), "Snake",
                             WS_OVERLAPPEDWINDOW, 200, 200, 200, 200, nullptr,
                             nullptr, GetModuleHandle(nullptr), nullptr);
  if (not hwnd) {
    std::cerr << "Failed to create window\n";
    return;
  }
  ShowWindow(hwnd, SW_SHOW);
  UpdateWindow(hwnd);
  isOkFlag = true;
}

bool Window::registerWindowClass(const std::string &windowClassName) const {
  WNDCLASS windowClass;
  windowClass.style = 0;
  windowClass.lpfnWndProc = &Window::globalOnEvent;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = GetModuleHandleA(nullptr);
  windowClass.hIcon = nullptr;
  windowClass.hCursor = nullptr;
  windowClass.hbrBackground = nullptr;
  windowClass.lpszMenuName = nullptr;
  windowClass.lpszClassName = windowClassName.c_str();
  return RegisterClass(&windowClass);
}

bool Window::isOk() const { return isOkFlag; }

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
  switch (message) {
  case WM_KEYDOWN:
  case WM_SYSKEYDOWN: {
    if ((HIWORD(lParam) & KF_REPEAT) == 0) {
      Event event;
      event.type = Event::Type::KeyPressed;
      event.value.keyboard.code = mapVirtualKeyCodeToKey(wParam, lParam);
      m_events.push(event);
    }
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

Keyboard::Key Window::mapVirtualKeyCodeToKey(WPARAM key, LPARAM flags) {
  switch (key) {
  case VK_SHIFT: {
    static UINT lShift = MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC);
    UINT scancode = static_cast<UINT>((flags & (0xFF << 16)) >> 16);
    return scancode == lShift ? Keyboard::Key::LShift : Keyboard::Key::RShift;
  }
  case VK_MENU:
    return (HIWORD(flags) & KF_EXTENDED) ? Keyboard::Key::RAlt
                                         : Keyboard::Key::LAlt;
  case VK_CONTROL:
    return (HIWORD(flags) & KF_EXTENDED) ? Keyboard::Key::RControl
                                         : Keyboard::Key::LControl;
  case VK_LWIN:
    return Keyboard::Key::LSystem;
  case VK_RWIN:
    return Keyboard::Key::RSystem;
  case VK_APPS:
    return Keyboard::Key::Menu;
  case VK_OEM_1:
    return Keyboard::Key::Semicolon;
  case VK_OEM_2:
    return Keyboard::Key::Slash;
  case VK_OEM_PLUS:
    return Keyboard::Key::Equal;
  case VK_OEM_MINUS:
    return Keyboard::Key::Hyphen;
  case VK_OEM_4:
    return Keyboard::Key::LBracket;
  case VK_OEM_6:
    return Keyboard::Key::RBracket;
  case VK_OEM_COMMA:
    return Keyboard::Key::Comma;
  case VK_OEM_PERIOD:
    return Keyboard::Key::Period;
  case VK_OEM_7:
    return Keyboard::Key::Quote;
  case VK_OEM_5:
    return Keyboard::Key::Backslash;
  case VK_OEM_3:
    return Keyboard::Key::Tilde;
  case VK_ESCAPE:
    return Keyboard::Key::Escape;
  case VK_SPACE:
    return Keyboard::Key::Space;
  case VK_RETURN:
    return Keyboard::Key::Enter;
  case VK_BACK:
    return Keyboard::Key::Backspace;
  case VK_TAB:
    return Keyboard::Key::Tab;
  case VK_PRIOR:
    return Keyboard::Key::PageUp;
  case VK_NEXT:
    return Keyboard::Key::PageDown;
  case VK_END:
    return Keyboard::Key::End;
  case VK_HOME:
    return Keyboard::Key::Home;
  case VK_INSERT:
    return Keyboard::Key::Insert;
  case VK_DELETE:
    return Keyboard::Key::Delete;
  case VK_ADD:
    return Keyboard::Key::Add;
  case VK_SUBTRACT:
    return Keyboard::Key::Subtract;
  case VK_MULTIPLY:
    return Keyboard::Key::Multiply;
  case VK_DIVIDE:
    return Keyboard::Key::Divide;
  case VK_PAUSE:
    return Keyboard::Key::Pause;
  case VK_F1:
    return Keyboard::Key::F1;
  case VK_F2:
    return Keyboard::Key::F2;
  case VK_F3:
    return Keyboard::Key::F3;
  case VK_F4:
    return Keyboard::Key::F4;
  case VK_F5:
    return Keyboard::Key::F5;
  case VK_F6:
    return Keyboard::Key::F6;
  case VK_F7:
    return Keyboard::Key::F7;
  case VK_F8:
    return Keyboard::Key::F8;
  case VK_F9:
    return Keyboard::Key::F9;
  case VK_F10:
    return Keyboard::Key::F10;
  case VK_F11:
    return Keyboard::Key::F11;
  case VK_F12:
    return Keyboard::Key::F12;
  case VK_F13:
    return Keyboard::Key::F13;
  case VK_F14:
    return Keyboard::Key::F14;
  case VK_F15:
    return Keyboard::Key::F15;
  case VK_LEFT:
    return Keyboard::Key::Left;
  case VK_RIGHT:
    return Keyboard::Key::Right;
  case VK_UP:
    return Keyboard::Key::Up;
  case VK_DOWN:
    return Keyboard::Key::Down;
  case VK_NUMPAD0:
    return Keyboard::Key::Numpad0;
  case VK_NUMPAD1:
    return Keyboard::Key::Numpad1;
  case VK_NUMPAD2:
    return Keyboard::Key::Numpad2;
  case VK_NUMPAD3:
    return Keyboard::Key::Numpad3;
  case VK_NUMPAD4:
    return Keyboard::Key::Numpad4;
  case VK_NUMPAD5:
    return Keyboard::Key::Numpad5;
  case VK_NUMPAD6:
    return Keyboard::Key::Numpad6;
  case VK_NUMPAD7:
    return Keyboard::Key::Numpad7;
  case VK_NUMPAD8:
    return Keyboard::Key::Numpad8;
  case VK_NUMPAD9:
    return Keyboard::Key::Numpad9;
  case 'A':
    return Keyboard::Key::A;
  case 'Z':
    return Keyboard::Key::Z;
  case 'E':
    return Keyboard::Key::E;
  case 'R':
    return Keyboard::Key::R;
  case 'T':
    return Keyboard::Key::T;
  case 'Y':
    return Keyboard::Key::Y;
  case 'U':
    return Keyboard::Key::U;
  case 'I':
    return Keyboard::Key::I;
  case 'O':
    return Keyboard::Key::O;
  case 'P':
    return Keyboard::Key::P;
  case 'Q':
    return Keyboard::Key::Q;
  case 'S':
    return Keyboard::Key::S;
  case 'D':
    return Keyboard::Key::D;
  case 'F':
    return Keyboard::Key::F;
  case 'G':
    return Keyboard::Key::G;
  case 'H':
    return Keyboard::Key::H;
  case 'J':
    return Keyboard::Key::J;
  case 'K':
    return Keyboard::Key::K;
  case 'L':
    return Keyboard::Key::L;
  case 'M':
    return Keyboard::Key::M;
  case 'W':
    return Keyboard::Key::W;
  case 'X':
    return Keyboard::Key::X;
  case 'C':
    return Keyboard::Key::C;
  case 'V':
    return Keyboard::Key::V;
  case 'B':
    return Keyboard::Key::B;
  case 'N':
    return Keyboard::Key::N;
  case '0':
    return Keyboard::Key::Num0;
  case '1':
    return Keyboard::Key::Num1;
  case '2':
    return Keyboard::Key::Num2;
  case '3':
    return Keyboard::Key::Num3;
  case '4':
    return Keyboard::Key::Num4;
  case '5':
    return Keyboard::Key::Num5;
  case '6':
    return Keyboard::Key::Num6;
  case '7':
    return Keyboard::Key::Num7;
  case '8':
    return Keyboard::Key::Num8;
  case '9':
    return Keyboard::Key::Num9;
  }

  return Keyboard::Key::Unknown;
}

} // namespace SimpleSnake