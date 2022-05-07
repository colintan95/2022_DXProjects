#include <windows.h>

#include <string>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
  }

  return DefWindowProc(hwnd, message, wparam, lparam);
}

static constexpr int k_WindowWidth = 1024;
static constexpr int k_WindowHeight = 768;

static const wchar_t* k_WindowName = L"Impl";

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR cmdLine, int showCmd) {
  WNDCLASSEX wndClass{};
  wndClass.cbSize = sizeof(WNDCLASSEX);
  wndClass.style = CS_HREDRAW | CS_VREDRAW;
  wndClass.lpfnWndProc = WindowProc;
  wndClass.hInstance = instance;
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndClass.lpszClassName = k_WindowName;
  RegisterClassEx(&wndClass);

  HWND hwnd = CreateWindow(wndClass.lpszClassName, k_WindowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                           CW_USEDEFAULT, k_WindowWidth, k_WindowHeight, nullptr, nullptr, instance,
                           nullptr);
  ShowWindow(hwnd, showCmd);

  MSG msg{};
  while (msg.message != WM_QUIT) {
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return 0;
}
