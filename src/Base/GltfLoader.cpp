#include "GltfLoader.h"

#include <nlohmann/json.hpp>
#include <winrt/base.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>

#include "d3dx12.h"

namespace fs = std::filesystem;

using nlohmann::json;

namespace base {

static std::vector<uint8_t> LoadBinaryDataFromFile(const char* path) {
  std::vector<uint8_t> data;

  std::ifstream strm(path, std::ios::in | std::ios::binary | std::ios::ate);

  if (!strm.is_open())
    throw std::runtime_error("Could not open file: " + std::string(path));

  size_t fileSize = strm.tellg();
  data.resize(fileSize);

  strm.seekg(0, std::ios::beg);

  strm.read(reinterpret_cast<char*>(data.data()), fileSize);

  return data;
}

static BufferView GetBufferViewForAccessorIndex(int accessorIndex, json& gltfJson) {
  BufferView bufferView{};

  auto& accessorJson = gltfJson["accessors"][accessorIndex];

  auto bufferViewIndex = accessorJson["bufferView"].get<int>();
  auto& bufferViewJson = gltfJson["bufferViews"][bufferViewIndex];

  bufferView.BufferIndex = bufferViewJson["buffer"].get<int>();
  bufferView.Length = bufferViewJson["byteLength"].get<int>();
  bufferView.Offset = bufferViewJson["byteOffset"].get<int>();

  return bufferView;
}

Scene LoadGltf(const char* path) {
  Scene scene{};

  std::ifstream strm(path);
  if (!strm.is_open())
    throw std::runtime_error("Could not open file: " + std::string(path));

  json gltfJson;
  strm >> gltfJson;

  for (auto& bufferJson : gltfJson["buffers"]) {
    std::vector<uint8_t> data =
        LoadBinaryDataFromFile(bufferJson["uri"].get<std::string>().c_str());
    scene.Buffers.push_back(std::move(data));
  }

  for (auto& meshJson : gltfJson["meshes"]) {
    Mesh mesh{};

    for (auto& primJson : meshJson["primitives"]) {
      Primitive primitive{};

      int positionIndex = primJson["attributes"]["POSITION"].get<int>();
      int indicesIndex = primJson["indices"].get<int>();

      primitive.Position = GetBufferViewForAccessorIndex(positionIndex, gltfJson);
      primitive.Indices = GetBufferViewForAccessorIndex(indicesIndex, gltfJson);

      mesh.Primitives.push_back(std::move(primitive));
    }

    scene.Meshes.push_back(std::move(mesh));
  }

  return scene;
}

} // namespace base
