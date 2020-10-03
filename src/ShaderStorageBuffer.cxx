#include "prgl/ShaderStorageBuffer.hxx"

#include <cstring>
#include <iostream>

#include "prgl/glCommon.hxx"

namespace prgl {

std::shared_ptr<ShaderStorageBuffer> ShaderStorageBuffer::Create() {
  return std::make_shared<ShaderStorageBuffer>();
}

ShaderStorageBuffer::ShaderStorageBuffer() : mHandle(INVALID_HANDLE) {
  glGenBuffers(1, &mHandle);
}

ShaderStorageBuffer::~ShaderStorageBuffer() {
  glDeleteBuffers(1, &mHandle);
  mHandle = INVALID_HANDLE;
}

uint32_t ShaderStorageBuffer::getSizeInBytes() const {
  GLint64 size = 0;
  bind(true);
  glGetBufferParameteri64v(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &size);

  bind(false);
  return static_cast<uint32_t>(size);
}

void ShaderStorageBuffer::create(const void* dataStart, uint32_t nBytes) {
  bind(true);

  glBufferData(GL_SHADER_STORAGE_BUFFER, nBytes, dataStart, GL_STATIC_DRAW);
  // std::cout << "ShaderStorageBuffer::allocated:size: " << nBytes <<
  // std::endl;

  bind(false);
}

void ShaderStorageBuffer::upload(const void* dataStart, uint32_t nBytes) {
  // check if enough bytes allocated
  if (nBytes != static_cast<uint32_t>(getSizeInBytes())) {
    std::cout << "ShaderStorageBuffer::reallocated:size: " << nBytes
              << std::endl;
    create(dataStart, nBytes);
    return;
  }

  bind(true);

  GLvoid* data = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);

  std::memcpy(data, dataStart, nBytes);

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  bind(false);
}

void ShaderStorageBuffer::download(void* dataStart, uint32_t nBytes) const {
  bind(true);

  void* data = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);

  // std::cout << "ShaderStorageBuffer::download:size: " << nBytes << std::endl;

  memcpy(dataStart, data, nBytes);

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  bind(false);
}

void ShaderStorageBuffer::bind(bool bind) const {
  if (bind) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mHandle);
  } else {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  }
}

void ShaderStorageBuffer::bindBase(uint32_t location) const {
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, mHandle);
}

void ShaderStorageBuffer::copyTo(ShaderStorageBuffer& other) const {
  const auto otherSize = other.getSizeInBytes();
  const auto thisSize  = getSizeInBytes();

  int32_t cReadBuffer = 0;
  glGetIntegerv(GL_COPY_READ_BUFFER, &cReadBuffer);
  int32_t cWriteBuffer = 0;
  glGetIntegerv(GL_COPY_WRITE_BUFFER, &cWriteBuffer);

  if (thisSize != otherSize) {
    other.create(nullptr, thisSize);
  }

  glBindBuffer(GL_COPY_READ_BUFFER, mHandle);
  glBindBuffer(GL_COPY_WRITE_BUFFER, other.getHandle());

  glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
                      thisSize);

  glBindBuffer(GL_COPY_READ_BUFFER, cReadBuffer);
  glBindBuffer(GL_COPY_WRITE_BUFFER, cWriteBuffer);
}

uint32_t ShaderStorageBuffer::getHandle() const {
  return mHandle;
}

}  // namespace prgl
