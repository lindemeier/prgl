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
#include "prgl/VertexArrayObject.hxx"

#include "prgl/glCommon.hxx"

namespace prgl {

std::shared_ptr<VertexArrayObject> VertexArrayObject::Create() {
  return std::make_shared<VertexArrayObject>();
}

VertexArrayObject::VertexArrayObject() : mVao(INVALID_HANDLE) {
  glGenVertexArrays(1, &mVao);
}

VertexArrayObject::~VertexArrayObject() {
  glDeleteVertexArrays(1, &mVao);
  mVao = INVALID_HANDLE;
}

/**
 * @brief Bind the Vertex Array Object
 *
 * @param bind true if bind
 */
void VertexArrayObject::bind(bool bind) const {
  if (bind) {
    glBindVertexArray(mVao);
  } else {
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
                               const uint32_t count) {
  glDrawArrays(static_cast<GLenum>(mode), static_cast<GLint>(first),
               static_cast<GLint>(count));
}

void VertexArrayObject::addVertexBufferObject(
  uint32_t location, const std::shared_ptr<VertexBufferObject>& vbo) {
  // add to the map to keep the reference (consider move)
  mVboMap[location] = vbo;

  bind(true);
  {
    Binder<VertexBufferObject> binderVbo(vbo);
    glVertexAttribPointer(
      location, vbo->getVertexComponentDataColumns(),
      static_cast<GLenum>(vbo->getVertexComponentDataType()), GL_FALSE, 0,
      nullptr);
    glEnableVertexAttribArray(location);
  }
  bind(false);
}

}  // namespace prgl
