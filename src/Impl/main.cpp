#include <windows.h>

#include <string>

#include "app.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
  }

  return DefWindowProc(hwnd, message, wparam, lparam);
}

static constexpr int k_windowWidth = 1024;
static constexpr int k_windowHeight = 768;

static const wchar_t* k_windowName = L"Impl";

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR cmdLine, int showCmd) {
  WNDCLASSEX wndClass{};
  wndClass.cbSize = sizeof(WNDCLASSEX);
  wndClass.style = CS_HREDRAW | CS_VREDRAW;
  wndClass.lpfnWndProc = WindowProc;
  wndClass.hInstance = hinstance;
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndClass.lpszClassName = k_windowName;
  RegisterClassEx(&wndClass);

  HWND hwnd = CreateWindow(wndClass.lpszClassName, k_windowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                           CW_USEDEFAULT, k_windowWidth, k_windowHeight, nullptr, nullptr,
                           hinstance, nullptr);
  ShowWindow(hwnd, showCmd);

  App app(hwnd, k_windowWidth, k_windowHeight);

  MSG msg{};
  while (msg.message != WM_QUIT) {
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    app.RenderFrame();
  }

  return 0;
}
