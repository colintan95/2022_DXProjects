#pragma once

#include <windows.h>

#include <optional>
#include <string>

namespace base {

class Window {
public:
  Window(const std::wstring& title, int width, int height, int showCmd);
  ~Window();

  bool ShouldClose();

  HWND GetHwnd();

  std::optional<LRESULT> HandleMessage(UINT message, WPARAM wparam, LPARAM lparam);

private:
  std::wstring m_title;

  HWND m_hwnd;
  int m_width;
  int m_height;

  bool m_shouldClose = false;
};

} // namespace base
