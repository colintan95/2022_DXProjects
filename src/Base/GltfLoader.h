#pragma once

#include <d3d12.h>
#include <winrt/base.h>

#include <vector>

namespace base {

struct Primitive {
  D3D12_VERTEX_BUFFER_VIEW Position;
};

struct Mesh {
  std::vector<Primitive> Primitives;
};

class GltfLoader {
public:
  void RequestLoad(const char* path);
};

} // namespace base
