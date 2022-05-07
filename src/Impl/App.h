#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <windows.h>
#include <winrt/base.h>
#include <wil/resource.h>

#include <cstdint>

constexpr int k_numFrames = 3;

class App {
public:
  App(HWND hwnd, int windowWidth, int windowHeight);

private:
  void CreateDevice();
  void CreateCommandQueueAndSwapChain();
  void CreateCommandListAndFence();

  void CreatePipeline();

  HWND m_hwnd;
  int m_windowWidth = 0;
  int m_windowHeight = 0;

  winrt::com_ptr<IDXGIFactory2> m_factory;
  winrt::com_ptr<ID3D12Device> m_device;
  winrt::com_ptr<ID3D12CommandQueue> m_cmdQueue;
  winrt::com_ptr<IDXGISwapChain3> m_swapChain;

  struct Frame {
    winrt::com_ptr<ID3D12CommandAllocator> m_cmdAlloc;
  };
  Frame m_frames[k_numFrames];

  int m_currentFrame = 0;

  winrt::com_ptr<ID3D12GraphicsCommandList> m_cmdList;

  winrt::com_ptr<ID3D12Fence> m_fence;
  uint64_t m_nextFenceValue = 0;
  wil::unique_handle m_fenceEvent;

  winrt::com_ptr<ID3D12RootSignature> m_rootSig;
  winrt::com_ptr<ID3D12PipelineState> m_pipeline;
};
