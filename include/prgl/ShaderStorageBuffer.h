/**
 * @file ShaderStorageBuffer.h
 * @author Thomas Lindemeier
 * @brief
 * @date 2019-01-02
 *
 */
#ifndef PRGL_SHADERSTORAGEBUFFER_H
#define PRGL_SHADERSTORAGEBUFFER_H

#include <memory>
#include <stdint.h>

namespace prgl
{

class Context;

class ShaderStorageBuffer
{
  std::shared_ptr<uint32_t> mHandlePtr;

public:
  ShaderStorageBuffer();

  ~ShaderStorageBuffer();

  // retun current buffer size in bytes
  int32_t getSizeInBytes() const;

  // allocate buffer
  void create(const void* dataStart, uint32_t nBytes);

  // upload to previous allocated buffer
  void upload(const void* dataStart, uint32_t nBytes);

  // download to host ram
  void download(void* dataStart, uint32_t nBytes) const;

  void bind(bool bind) const;

  // bind to location to address it in a shader
  void bindBase(uint32_t location) const;

  void copyTo(ShaderStorageBuffer& other) const;

  uint32_t getHandle() const;
};

} // namespace prgl

#endif // PRGL_SHADERSTORAGEBUFFER_H
