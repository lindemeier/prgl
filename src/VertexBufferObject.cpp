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

std::shared_ptr<VertexBufferObject> VertexBufferObject::Create()
{
  // make shared not usable due to private constructor
  struct MakeSharedEnabler : public VertexBufferObject
  {
  };

  return std::make_shared<MakeSharedEnabler>();
}

VertexBufferObject::VertexBufferObject()
  : mVbo(INVALID_HANDLE), mDataType(DataType::Float), mDataColumns(0U),
    mVerticesCount(0U)
{

  glGenBuffers(1, &mVbo);
}

VertexBufferObject::~VertexBufferObject()
{
  glDeleteBuffers(1, &mVbo);
  mVbo = INVALID_HANDLE;
}

/**
 * @brief Bind the Vertex Array Object
 *
 * @param bind true if bind
 */
void VertexBufferObject::bind(bool bind) const
{
  if (bind)
    {
      glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    }
  else
    {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

DataType VertexBufferObject::getVertexComponentDataType() const
{
  return mDataType;
}

uint32_t VertexBufferObject::getVertexComponentDataColumns() const
{
  return mDataColumns;
}

size_t VertexBufferObject::getVerticesCount() const { return mVerticesCount; }

} // namespace prgl
