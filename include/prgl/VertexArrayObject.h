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

  void bind(bool bind) const;

  void addVertexBufferObject(const VertexBufferObject& vbo);

  void render();

private:
  std::shared_ptr<uint32_t> mVaoPtr;

  std::vector<VertexBufferObject> mVboList;
};
} // namespace prgl

#endif // PRGL_VERTEX_ARRAY_OBJECT_H