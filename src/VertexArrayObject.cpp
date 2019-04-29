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

VertexArrayObject::~VertexArrayObject() { cleanup(); }

VertexArrayObject::VertexArrayObject() : mVao(INVALID_HANDLE)
{
  glGenVertexArrays(1, &mVao);
}

/**
 * @brief Bind the Vertex Array Object
 *
 * @param bind true if bind
 */
void VertexArrayObject::bind(bool bind)
{
  if (bind)
    {
      glBindVertexArray(mVao);
    }
  else
    {
      glBindVertexArray(0);
    }
}

/**
 * @brief
 *
mode
    Specifies what kind of primitives to render. Symbolic constants GL_POINTS,
GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY,
GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES,
GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES are accepted.
first
    Specifies the starting index in the enabled arrays.
count
    Specifies the number of indices to be rendered.
 *
 */
void VertexArrayObject::render()
{
  // TODO wrap the arguments
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void VertexArrayObject::addVertexBufferObject(
  const std::shared_ptr<VertexBufferObject>& vbo)
{
  const auto index = mVboList.size();
  // add to the list to keep the reference (unique_ptr and move may bet better )
  mVboList.push_back(vbo);

  bind(true);
  vbo->bind(true);
  glVertexAttribPointer(index, vbo->getDataColumns(),
                        static_cast<GLenum>(vbo->getDataType()), GL_FALSE, 0,
                        0);
  glEnableVertexAttribArray(index);
  vbo->bind(false);

  // TODO getAttribLocation to find which is color attrib etc.
  // shaders make that obsolete
}

/**
 * @brief Delete the Vertex Array Object
 *
 */
void VertexArrayObject::cleanup()
{
  if (mVao > INVALID_HANDLE)
    {
      glDeleteVertexArrays(1, &mVao);
    }
}

} // namespace prgl
