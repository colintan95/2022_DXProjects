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

void GltfLoader::RequestLoad(const char* path) {
  std::ifstream strm(path);
  if (!strm.is_open())
    throw std::runtime_error("Could not open file: " + std::string(path));

  json gltfJson;
  strm >> gltfJson;

  for (auto& meshJson : gltfJson["meshes"]) {
    for (auto& primJson : meshJson["primitives"]) {
      Primitive primitive{};

      auto accessorIndex = primJson["attributes"]["POSITION"].get<int>();
      auto& accessor = gltfJson["accessors"][accessorIndex];

      auto bufferViewIndex = accessor["bufferView"].get<int>();
      auto& bufferView = gltfJson["bufferViews"][bufferViewIndex];

      primitive.Position.SizeInBytes = bufferView["byteLength"];
      primitive.Position.StrideInBytes = 0;
    }
  }
}

} // namespace base
