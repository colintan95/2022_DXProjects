#include "window.h"

#include <winrt/base.h>

#include <unordered_map>

using winrt::check_bool;

namespace base {

thread_local static std::unordered_map<HWND, Window*> s_hwndMap;

static LRESULT WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
  auto it = s_hwndMap.find(hwnd);

  if (it != s_hwndMap.end()) {
    Window* window = it->second;
    if (auto retVal = window->HandleMessage(message, wparam, lparam))
      return *retVal;
  }

  return DefWindowProc(hwnd, message, wparam, lparam);
}

Window::Window(const std::wstring& title, int width, int height, int showCmd)
  : m_title(title), m_width(width), m_height(height) {
  std::wstring className = m_title;
  HINSTANCE hinstance = GetModuleHandle(NULL);

  WNDCLASSEX wndClass{};
  wndClass.cbSize = sizeof(WNDCLASSEX);
  wndClass.style = CS_HREDRAW | CS_VREDRAW;
  wndClass.lpfnWndProc = WindowProc;
  wndClass.hInstance = hinstance;
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndClass.lpszClassName = className.c_str();
  check_bool(RegisterClassEx(&wndClass) != 0);

  m_hwnd = CreateWindow(wndClass.lpszClassName, m_title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                        CW_USEDEFAULT, width, height, nullptr, nullptr, hinstance, nullptr);
  s_hwndMap[m_hwnd] = this;

  ShowWindow(m_hwnd, showCmd);
}

Window::~Window() {
  s_hwndMap.erase(m_hwnd);

  if (IsWindow(m_hwnd))
    check_bool(DestroyWindow(m_hwnd));
}

bool Window::ShouldClose() {
  return m_shouldClose;
}

HWND Window::GetHwnd() {
  return m_hwnd;
}

std::optional<LRESULT> Window::HandleMessage(UINT message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
    case WM_CLOSE:
      m_shouldClose = true;
      return 0;
  }

  return std::nullopt;
}

} // namespace base
