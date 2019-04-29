/**
 * @file VertexBufferObject.cpp
 *
 * @author Thomas Lindemeier
 *
 * @brief
 *
 * @date 2019-04-28
 *
 */
#include "prgl/VertexBufferObject.h"

#include "prgl/glCommon.h"

#include <iostream>

namespace prgl
{

VertexBufferObject::VertexBufferObject() : mVboPtr(nullptr)
{
  mVboPtr = std::shared_ptr<uint32_t>(new uint32_t, [](uint32_t* ptr) {
    glDeleteBuffers(1, ptr);
    *ptr = INVALID_HANDLE;
    delete ptr;
    ptr = nullptr;
  });
  glGenBuffers(1, mVboPtr.get());
}

VertexBufferObject::~VertexBufferObject() {}

/**
 * @brief Bind the Vertex Array Object
 *
 * @param bind true if bind
 */
void VertexBufferObject::bind(bool bind) const
{
  if (bind)
    {
      glBindBuffer(GL_ARRAY_BUFFER, *mVboPtr);
    }
  else
    {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

DataType VertexBufferObject::getDataType() const { return mDataType; }

uint32_t VertexBufferObject::getDataColumns() const { return mDataColumns; }

} // namespace prgl
