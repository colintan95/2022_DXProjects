#pragma once

namespace base {

constexpr size_t GetAlignedSize(size_t size, size_t alignment) {
  return (size + (alignment - 1)) & ~(alignment - 1);
}

template<typename T>
size_t GetAlignedSize(const T& object, size_t alignment) {
  return GetAlignedSize(sizeof(T), alignment);
}

} // namespace base
