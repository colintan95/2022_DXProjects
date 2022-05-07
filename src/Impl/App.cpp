#include "App.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <winrt/base.h>

#include "d3dx12.h"

using winrt::check_bool;
using winrt::check_hresult;
using winrt::com_ptr;

App::App(HWND hwnd, int windowWidth, int windowHeight)
  : m_hwnd(hwnd), m_windowWidth(windowWidth), m_windowHeight(windowHeight) {
  CreateDevice();
  CreateCommandQueueAndSwapChain();
  CreateCommandListAndFence();

  CreatePipeline();
}

void App::CreateDevice() {
  com_ptr<ID3D12Debug> debugController;
  check_hresult(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.put())));

  debugController->EnableDebugLayer();

  com_ptr<ID3D12Debug1> debugController1;
  debugController.as(debugController1);

  debugController1->SetEnableGPUBasedValidation(true);

  check_hresult(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(m_factory.put())));

  com_ptr<IDXGIFactory6> factory6;
  m_factory.as(factory6);

  com_ptr<IDXGIAdapter1> adapter;

  for (UINT adapterIndex = 0;
       factory6->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
                                            IID_PPV_ARGS(adapter.put())) != DXGI_ERROR_NOT_FOUND;
       ++adapterIndex) {
    DXGI_ADAPTER_DESC1 desc;
    adapter->GetDesc1(&desc);

    if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
      continue;

    if (SUCCEEDED(D3D12CreateDevice(adapter.get(), D3D_FEATURE_LEVEL_12_1, _uuidof(ID3D12Device),
                                    nullptr)))
      break;
  }

  check_hresult(D3D12CreateDevice(adapter.get(), D3D_FEATURE_LEVEL_12_1,
                                  IID_PPV_ARGS(m_device.put())));

}

void App::CreateCommandQueueAndSwapChain() {
  D3D12_COMMAND_QUEUE_DESC queueDesc{};
  queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

  check_hresult(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_cmdQueue.put())));

  DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
  swapChainDesc.BufferCount = k_numFrames;
  swapChainDesc.Width = m_windowWidth;
  swapChainDesc.Height = m_windowHeight;
  swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swapChainDesc.SampleDesc.Count = 1;

  winrt::com_ptr<IDXGISwapChain1> swapChain;
  check_hresult(m_factory->CreateSwapChainForHwnd(m_cmdQueue.get(), m_hwnd, &swapChainDesc, nullptr,
                                                  nullptr, swapChain.put()));
  swapChain.as(m_swapChain);
}

void App::CreateCommandListAndFence() {
  for (int i = 0; i < k_numFrames; ++i) {
    check_hresult(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                   IID_PPV_ARGS(m_frames[i].m_cmdAlloc.put())));
  }

  check_hresult(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
                                            m_frames[m_currentFrame].m_cmdAlloc.get(), nullptr,
                                            IID_PPV_ARGS(m_cmdList.put())));
  check_hresult(m_cmdList->Close());

  check_hresult(m_device->CreateFence(m_nextFenceValue, D3D12_FENCE_FLAG_NONE,
                                      IID_PPV_ARGS(m_fence.put())));
  ++m_nextFenceValue;

  m_fenceEvent.reset(CreateEvent(nullptr, false, false, nullptr));
  check_bool(m_fenceEvent.is_valid());
}

//static const char k_vertexShaderSrc[] =
//    "float4 main(float2 screenPos) {"
//    "  return float4(screenPos, 0.f, 1.f);"
//    "}";
//
//static const char k_pixelShaderSrc[] =
//    "float4 main() {"
//    "  return float4(1.f, 0.f, 0.f, 1.f);"
//    "}";

void App::CreatePipeline() {
  D3D12_ROOT_SIGNATURE_DESC1 rootSigDesc{};
  rootSigDesc.NumParameters = 0;
  rootSigDesc.NumStaticSamplers = 0;
  rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

  D3D12_VERSIONED_ROOT_SIGNATURE_DESC versionedDesc{};
  versionedDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
  versionedDesc.Desc_1_1 = rootSigDesc;

  com_ptr<ID3DBlob> signatureBlob;
  com_ptr<ID3DBlob> errorBlob;
  check_hresult(D3D12SerializeVersionedRootSignature(&versionedDesc, signatureBlob.put(),
                                                     errorBlob.put()));
  check_hresult(m_device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
                                              signatureBlob->GetBufferSize(),
                                              IID_PPV_ARGS(m_rootSig.put())));

  //D3D12_SHADER_BYTECODE vertexShader;
  //vertexShader.pShaderBytecode = k_vertexShaderSrc;
  //vertexShader.BytecodeLength = sizeof(k_vertexShaderSrc);

  //D3D12_SHADER_BYTECODE pixelShader;
  //pixelShader.pShaderBytecode = k_pixelShaderSrc;
  //pixelShader.BytecodeLength = sizeof(k_pixelShaderSrc);

  D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
  };

  D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
  inputLayoutDesc.pInputElementDescs = inputElementDescs;
  inputLayoutDesc.NumElements = _countof(inputElementDescs);

  //D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
  //pipelineDesc.InputLayout = inputLayoutDesc;
  //pipelineDesc.pRootSignature = m_rootSig.get();
  //pipelineDesc.VS = vertexShader;
  //pipelineDesc.PS = pixelShader;
  //pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
  //pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
  //pipelineDesc.DepthStencilState.DepthEnable = false;
  //pipelineDesc.DepthStencilState.StencilEnable = false;
  //pipelineDesc.SampleMask = UINT_MAX;
  //pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  //pipelineDesc.NumRenderTargets = 1;
  //pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
  //pipelineDesc.SampleDesc.Count = 1;

  //check_hresult(m_device->CreateGraphicsPipelineState(&pipelineDesc,
  //                                                    IID_PPV_ARGS(m_pipeline.put())));
}
