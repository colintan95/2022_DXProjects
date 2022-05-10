#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <windows.h>
#include <winrt/base.h>
#include <wil/resource.h>

#include <cstdint>
#include <vector>

constexpr int k_numFrames = 3;

class App {
public:
  App(HWND hwnd, int windowWidth, int windowHeight);
  ~App();

  void RenderFrame();

private:
  void CreateDevice();
  void CreateCommandQueueAndSwapChain();
  void CreateCommandListAndFence();

  void CreatePipeline();
  void CreateDescriptorHeaps();

  void CreateDepthTexture();
  void CreateVertexBuffers();
  void CreateConstantBuffer();

  void MoveToNextFrame();
  void WaitForGpu();

  HWND m_hwnd;
  int m_windowWidth = 0;
  int m_windowHeight = 0;

  winrt::com_ptr<IDXGIFactory2> m_factory;
  winrt::com_ptr<ID3D12Device> m_device;
  winrt::com_ptr<ID3D12CommandQueue> m_cmdQueue;
  winrt::com_ptr<IDXGISwapChain3> m_swapChain;

  D3D12_VIEWPORT m_viewport;
  D3D12_RECT m_scissorRect;

  winrt::com_ptr<ID3D12CommandAllocator> m_cmdAlloc;

  struct Frame {
    winrt::com_ptr<ID3D12CommandAllocator> CmdAlloc;
    winrt::com_ptr<ID3D12Resource> SwapChainBuffer;
    D3D12_CPU_DESCRIPTOR_HANDLE RtvHandle;
    uint64_t FenceWaitValue = 0;
  };
  Frame m_frames[k_numFrames];

  int m_currentFrame = 0;

  winrt::com_ptr<ID3D12GraphicsCommandList> m_cmdList;

  winrt::com_ptr<ID3D12Fence> m_fence;
  uint64_t m_nextFenceValue = 0;
  wil::unique_handle m_fenceEvent;

  winrt::com_ptr<ID3D12RootSignature> m_rootSig;
  winrt::com_ptr<ID3D12PipelineState> m_pipeline;

  winrt::com_ptr<ID3D12DescriptorHeap> m_rtvHeap;
  uint32_t m_rtvHandleSize = 0;

  winrt::com_ptr<ID3D12DescriptorHeap> m_dsvHeap;
  uint32_t m_dsvHandleSize = 0;

  D3D12_CPU_DESCRIPTOR_HANDLE m_dsvHandle;

  winrt::com_ptr<ID3D12Resource> m_depthTexture;

  struct Primitive {
    D3D12_VERTEX_BUFFER_VIEW PositionBufferView;
    D3D12_VERTEX_BUFFER_VIEW NormalBufferView;
    D3D12_INDEX_BUFFER_VIEW IndexBufferView;
    uint32_t NumVertices;
  };
  std::vector<Primitive> m_primitives;

  std::vector<winrt::com_ptr<ID3D12Resource>> m_vertexBuffers;

  struct MatrixBuffer {
    DirectX::XMFLOAT4X4 WorldMat;
    DirectX::XMFLOAT4X4 WorldViewProjMat;
  };
  MatrixBuffer m_matrixBuffer;

  winrt::com_ptr<ID3D12Resource> m_constantBuffer;
};
