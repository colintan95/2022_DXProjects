#include <windows.h>

#include <string>

#include <base/window.h>

#include "app.h"

static constexpr int k_windowWidth = 1024;
static constexpr int k_windowHeight = 768;

static const wchar_t* k_windowName = L"Impl";

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR cmdLine, int showCmd) {
  base::Window window(L"My Window", 1024, 768, showCmd);

  App app(window.GetHwnd(), k_windowWidth, k_windowHeight);

  while (!window.ShouldClose()) {
    MSG msg{};
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    app.RenderFrame();
  }

  return 0;
}
