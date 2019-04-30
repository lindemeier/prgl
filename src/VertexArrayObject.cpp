/**
 * @file VertexArrayObject.cpp
 *
 * @author Thomas Lindemeier
 *
 * @brief
 *
 * @date 2019-04-28
 *
 */
#include "prgl/VertexArrayObject.h"

#include "prgl/glCommon.h"

namespace prgl
{

VertexArrayObject::VertexArrayObject() : mVaoPtr(nullptr)
{
  mVaoPtr = std::shared_ptr<uint32_t>(new uint32_t, [](uint32_t* ptr) {
    glDeleteVertexArrays(1, ptr);
    *ptr = INVALID_HANDLE;
    delete ptr;
    ptr = nullptr;
  });
  glGenVertexArrays(1, mVaoPtr.get());
}

VertexArrayObject::~VertexArrayObject() {}

/**
 * @brief Bind the Vertex Array Object
 *
 * @param bind true if bind
 */
void VertexArrayObject::bind(bool bind) const
{
  if (bind)
    {
      glBindVertexArray(*mVaoPtr);
    }
  else
    {
      glBindVertexArray(0);
    }
}

/**
 * @brief
 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml
 *
 * @param mode Specifies what kind of primitives to render.
 * @param first Specifies the starting index in the enabled arrays.
 * @param count Specifies the number of indices to be rendered.
 */
void VertexArrayObject::render(const DrawMode& mode, const uint32_t first,
                               const uint32_t count)
{

  glDrawArrays(static_cast<GLenum>(mode), first, count);
}

void VertexArrayObject::addVertexBufferObject(
  const std::shared_ptr<VertexBufferObject>& vbo)
{
  const auto index = mVboList.size();
  // add to the list to keep the reference (unique_ptr and move may be better )
  mVboList.push_back(vbo);

  Binder binderVao(*this);
  {
    Binder binderVbo(*vbo);
    glVertexAttribPointer(
      index, vbo->getVertexComponentDataColumns(),
      static_cast<GLenum>(vbo->getVertexComponentDataType()), GL_FALSE, 0, 0);
    glEnableVertexAttribArray(index);
  }
}

} // namespace prgl
