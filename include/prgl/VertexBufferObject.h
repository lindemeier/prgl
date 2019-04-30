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
  /**
   * @brief
   * https://www.khronos.org/registry/OpenGL-Refpages/es1.1/xhtml/glBufferData.xml
   */
  enum class Usage : uint32_t
  {
    /**
     * @brief The data store contents will be modified once and used many times
     * as the source for GL drawing commands.
     */
    StaticDraw = GL_STATIC_DRAW,
    /**
     * @brief The data store contents will be modified repeatedly and used many
     * times as the source for GL drawing commands.
     */
    DynamicDraw = GL_DYNAMIC_DRAW
  };

  static std::shared_ptr<VertexBufferObject> Create();

  ~VertexBufferObject();

  void bind(bool bind) const;

  /**
   * @brief Create Vertex Buffer object from data. Only floats supported for
   * now.
   *
   * @tparam N the number of components of each vertex data.
   * @tparam Vec The vec type storing vertex data.
   *
   * @param data the vector storing the vertices data.
   * @param usage Usage pattern of the data.
   */
  template <size_t N, template <class, size_t> class Vec>
  void createBuffer(const std::vector<Vec<float, N>>& data, const Usage usage)
  {
    mDataType      = DataType::Float;
    mDataColumns   = N;
    mVerticesCount = data.size();

    bind(true);
    glBufferData(GL_ARRAY_BUFFER, (N * sizeof(float)) * mVerticesCount,
                 data.data(), static_cast<GLenum>(usage));
    bind(false);
  }

  DataType getVertexComponentDataType() const;
  uint32_t getVertexComponentDataColumns() const;
  size_t   getVerticesCount() const;

private:
  VertexBufferObject(const VertexBufferObject&) = delete;
  VertexBufferObject& operator=(const VertexBufferObject&) = delete;
  VertexBufferObject();

  uint32_t mVbo;

  DataType mDataType;
  uint32_t mDataColumns;
  size_t   mVerticesCount;
};

} // namespace prgl

#endif // PRGL_VERTEX_BUFFER_OBJECT_H