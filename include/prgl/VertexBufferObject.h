/**
 * @file VertexBufferObject.h
 *
 * @author Thomas Lindemeier
 *
 * @brief
 *
 * @date 2019-04-28
 *
 */
#ifndef PRGL_VERTEX_BUFFER_OBJECT_H
#define PRGL_VERTEX_BUFFER_OBJECT_H

#include "prgl/glCommon.h"

#include <array>
#include <memory>
#include <vector>

namespace prgl
{
class VertexBufferObject
{
public:
  VertexBufferObject();
  ~VertexBufferObject();

  void bind(bool bind) const;

  template <uint32_t N>
  void create(const std::vector<std::array<float, N>>& data);

  DataType getDataType() const;
  uint32_t getDataColumns() const;

private:
  std::shared_ptr<uint32_t> mVboPtr;

  DataType mDataType;
  uint32_t mDataColumns;
};

/**
 * @brief Create Vertex Buffer Object from data. Only float vectors supported
 * currently.
 *
 * @tparam N
 * @param data
 */
template <uint32_t N>
void VertexBufferObject::create(const std::vector<std::array<float, N>>& data)
{
  bind(true);
  glBufferData(GL_ARRAY_BUFFER, (N * sizeof(float)) * data.size(), data.data(),
               GL_STATIC_DRAW);

  mDataType    = DataType::Float;
  mDataColumns = N;
}

} // namespace prgl

#endif // PRGL_VERTEX_BUFFER_OBJECT_H