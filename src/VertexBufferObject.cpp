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

namespace prgl
{

VertexBufferObject::~VertexBufferObject() { cleanup(); }

VertexBufferObject::VertexBufferObject() : mVbo(INVALID_HANDLE)
{
  glGenBuffers(1, &mVbo);
}

/**
 * @brief Bind the Vertex Array Object
 *
 * @param bind true if bind
 */
void VertexBufferObject::bind(bool bind)
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

DataType VertexBufferObject::getDataType() const { return mDataType; }

uint32_t VertexBufferObject::getDataColumns() const { return mDataColumns; }

/**
 * @brief Delete the Vertex Array Object
 *
 */
void VertexBufferObject::cleanup()
{
  if (mVbo > INVALID_HANDLE)
    {
      glDeleteBuffers(1, &mVbo);
    }
}

} // namespace prgl
