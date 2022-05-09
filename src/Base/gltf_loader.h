#pragma once

#include <d3d12.h>
#include <winrt/base.h>

#include <vector>

namespace base {

struct BufferView {
  int BufferIndex;
  int Length;
  int Offset;
};

struct Primitive {
  BufferView Position;
  BufferView Normal;
  BufferView Indices;
};

struct Mesh {
  std::vector<Primitive> Primitives;
};

struct Scene {
  std::vector<std::vector<uint8_t>> Buffers;
  std::vector<Mesh> Meshes;
};

Scene LoadGltf(const char* path);

} // namespace base
