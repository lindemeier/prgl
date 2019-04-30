/**
 * @file VertexArrayObject.h
 *
 * @author Thomas Lindemeier
 *
 * @brief
 *
 * @date 2019-04-28
 *
 */
#ifndef PRGL_VERTEX_ARRAY_OBJECT_H
#define PRGL_VERTEX_ARRAY_OBJECT_H

#include <memory>
#include <stdint.h>

#include "prgl/VertexBufferObject.h"

namespace prgl
{

/**
 * @brief "Specifies what kind of primitives to render."
 * (https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml)
 */
enum class DrawMode : uint32_t
{
  Points                 = GL_POINTS,
  LineStrip              = GL_LINE_STRIP,
  LineLoop               = GL_LINE_LOOP,
  Lines                  = GL_LINES,
  LineStripAdjacency     = GL_LINE_STRIP_ADJACENCY,
  LinesAdjacency         = GL_LINES_ADJACENCY,
  TriangleStrip          = GL_TRIANGLE_STRIP,
  TriangleFan            = GL_TRIANGLE_FAN,
  Triangles              = GL_TRIANGLES,
  TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
  TrianglesAdjacency     = GL_TRIANGLES_ADJACENCY,
  Patches                = GL_PATCHES
};

class VertexArrayObject final
{
public:
  static std::shared_ptr<VertexArrayObject> Create();

  VertexArrayObject();
  ~VertexArrayObject();

  void bind(bool bind) const;

  void addVertexBufferObject(const std::shared_ptr<VertexBufferObject>& vbo);

  void render(const DrawMode& mode, const uint32_t first, const uint32_t count);

private:
  VertexArrayObject(const VertexArrayObject&) = delete;
  VertexArrayObject& operator=(const VertexArrayObject&) = delete;

  uint32_t mVao;

  std::vector<std::shared_ptr<VertexBufferObject>> mVboList;
};
} // namespace prgl

#endif // PRGL_VERTEX_ARRAY_OBJECT_H