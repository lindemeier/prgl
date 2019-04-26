#include "prgl/ShaderStorageBuffer.h"

#include <cstring>
#include <iostream>

#include "glCommon.h"

namespace prgl
{

SSBO::SSBO() : mHandle(0) {}

SSBO::~SSBO()
{
  if (mHandle > 0)
    glDeleteBuffers(1, &mHandle);
}

GLint SSBO::getSizeInBytes() const
{
  GLint size;
  bind(true);
  glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &size);

  bind(false);
  return size;
}

void SSBO::create(const void* dataStart, GLuint nBytes)
{
  if (mHandle > 0)
    {
      glDeleteBuffers(1, &mHandle);
    }
  glGenBuffers(1, &mHandle);

  bind(true);

  glBufferData(GL_SHADER_STORAGE_BUFFER, nBytes, dataStart, GL_STATIC_DRAW);
  // std::cout << "SSBO::allocated:size: " << nBytes << std::endl;

  bind(false);
}

void SSBO::upload(const void* dataStart, GLuint nBytes)
{
  // check if enough bytes allocated
  if (nBytes != (GLuint)getSizeInBytes())
    {
      std::cout << "SSBO::reallocated:size: " << nBytes << std::endl;
      create(dataStart, nBytes);
      return;
    }

  bind(true);

  GLvoid* data = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);

  memcpy(data, dataStart, nBytes);

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  bind(false);
}

void SSBO::download(void* dataStart, GLuint nBytes) const
{
  bind(true);

  void* data = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);

  // std::cout << "SSBO::download:size: " << nBytes << std::endl;

  memcpy(dataStart, data, nBytes);

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  bind(false);
}

void SSBO::bind(bool bind) const
{
  if (bind)
    {
      glBindBuffer(GL_SHADER_STORAGE_BUFFER, mHandle);
    }
  else
    {
      glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
}

void SSBO::bindBase(uint32_t location) const
{
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, mHandle);
}

void SSBO::copyTo(SSBO& other) const
{
  GLint otherSize = other.getSizeInBytes();
  GLint thisSize  = getSizeInBytes();

  GLint cReadBuffer;
  glGetIntegerv(GL_COPY_READ_BUFFER, &cReadBuffer);
  GLint cWriteBuffer;
  glGetIntegerv(GL_COPY_WRITE_BUFFER, &cWriteBuffer);

  if (thisSize != otherSize)
    {
      other.create(nullptr, thisSize);
    }

  glBindBuffer(GL_COPY_READ_BUFFER, mHandle);
  glBindBuffer(GL_COPY_WRITE_BUFFER, other.getHandle());

  glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
                      thisSize);

  glBindBuffer(GL_COPY_READ_BUFFER, cReadBuffer);
  glBindBuffer(GL_COPY_WRITE_BUFFER, cWriteBuffer);
}

uint32_t SSBO::getHandle() const { return mHandle; }

} // namespace prgl
