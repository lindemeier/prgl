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
class VertexArrayObject
{
public:
  VertexArrayObject();
  ~VertexArrayObject();

  void bind(bool bind);

  void addVertexBufferObject(const std::shared_ptr<VertexBufferObject>& vbo);

  void render();

private:
  void cleanup();

private:
  uint32_t mVao;

  std::vector<std::shared_ptr<VertexBufferObject>> mVboList;
};
} // namespace prgl

#endif // PRGL_VERTEX_ARRAY_OBJECT_H