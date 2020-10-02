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
#include "prgl/VertexBufferObject.hxx"

#include <iostream>

#include "prgl/glCommon.hxx"

namespace prgl {

VertexBufferObject::VertexBufferObject(const Usage usage)
    : mVbo(INVALID_HANDLE),
      mDataType(DataType::Float),
      mDataColumns(0U),
      mVerticesCount(0U),
      mUsage(usage) {
  glGenBuffers(1, &mVbo);
}

VertexBufferObject::VertexBufferObject()
    : VertexBufferObject(Usage::StaticDraw) {}

VertexBufferObject::~VertexBufferObject() {
  glDeleteBuffers(1, &mVbo);
  mVbo = INVALID_HANDLE;
}

/**
 * @brief Bind the Vertex Array Object
 *
 * @param bind true if bind
 */
void VertexBufferObject::bind(bool bind) const {
  if (bind) {
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  } else {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}

DataType VertexBufferObject::getVertexComponentDataType() const {
  return mDataType;
}

uint32_t VertexBufferObject::getVertexComponentDataColumns() const {
  return mDataColumns;
}

size_t VertexBufferObject::getVerticesCount() const {
  return mVerticesCount;
}

}  // namespace prgl
