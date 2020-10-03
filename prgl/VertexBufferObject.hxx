/**
 * @file VertexBufferObject.hxx
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

#include <array>
#include <iostream>
#include <memory>
#include <vector>

#include "prgl/glCommon.hxx"

namespace prgl {

class VertexBufferObject final {
 public:
  /**
   * @brief
   * https://www.khronos.org/registry/OpenGL-Refpages/es1.1/xhtml/glBufferData.xml
   */
  enum class Usage : uint32_t {
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

  template <typename... T>
  static std::shared_ptr<VertexBufferObject> Create(T&&... args) {
    return std::make_shared<VertexBufferObject>(std::forward<T>(args)...);
  }

  VertexBufferObject(Usage usage);
  VertexBufferObject();
  ~VertexBufferObject();

  void bind(bool bind) const;

  template <class T>
  void createBuffer(const T* dataPtr, const uint32_t dimensions,
                    const std::size_t count) {
    mDataType      = DataTypeTr<T>::dataType;
    mDataColumns   = dimensions;
    mVerticesCount = count;

    bind(true);
    glBufferData(GL_ARRAY_BUFFER, (count * sizeof(T)) * mVerticesCount,
                 static_cast<const void*>(dataPtr),
                 static_cast<GLenum>(mUsage));
    bind(false);
  }

  /**
   * @brief Create Vertex Buffer object from data.
   *
   * @tparam N the number of components of each vertex data.
   * @tparam Vec The vec type storing vertex data.
   *
   * @param data the vector storing the vertices data.
   * @param usage Usage pattern of the data.
   */
  template <size_t N, class Type, template <class, size_t> class VecType>
  void createBuffer(const std::vector<VecType<Type, N>>& data) {
    createBuffer(data.data()->data(), N, data.size());
  }

  /**
   * @brief Update Vertex Buffer object from data.
   *
   * @tparam N the number of components of each vertex data.
   * @tparam Vec The vec type storing vertex data.
   *
   * @param data the vector storing the vertices data (of the same size as the
   * vector used when VertexBufferObject::createBuffer was called).
   *
   * @param startIndex The inde to data where to start updating.
   * @param nrElements The number of elements to update starting from
   * startIndex.
   */
  template <size_t N, class Type, template <class, size_t> class VecType>
  void updateBuffer(const std::vector<VecType<Type, N>>& data,
                    const uint32_t startIndex, uint32_t nrElements) {
    // only updating the data allowed for now, since resizing results in
    // attribute change, which must be caught by any VertexArrayObject
    // referencing this VertexBufferObject.

    // if ((mDataColumns != N) || (mDataType != DataType::Float) ||
    //     (mVerticesCount < (offset + size)))
    //   {
    //     createBuffer(data);
    //   }
    // else
    //   {
    //     update
    //   }
    if (startIndex >= mVerticesCount) {
      std::cerr
        << "VertexBufferObject::updateBuffer: startIndex >= mVerticesCount, "
           "Ignoring update..."
        << std::endl;
      return;
    }
    if (mVerticesCount < (startIndex + nrElements)) {
      std::cerr << "VertexBufferObject::updateBuffer: Update exceeds the "
                   "originally allocated data. Ignoring update..."
                << std::endl;
      return;
    }

    const auto nrBytes    = (N * sizeof(Type)) * nrElements;
    const auto byteOffset = (N * sizeof(Type)) * startIndex;

    bind(true);
    glBufferSubData(GL_ARRAY_BUFFER, byteOffset, nrBytes,
                    static_cast<const void*>(&(data[startIndex])));
    bind(false);
  }

  DataType getVertexComponentDataType() const;
  uint32_t getVertexComponentDataColumns() const;
  size_t getVerticesCount() const;

 private:
  VertexBufferObject(const VertexBufferObject&) = delete;
  VertexBufferObject& operator=(const VertexBufferObject&) = delete;

  uint32_t mVbo;

  DataType mDataType;
  uint32_t mDataColumns;
  size_t mVerticesCount;

  Usage mUsage;
};

}  // namespace prgl

#endif  // PRGL_VERTEX_BUFFER_OBJECT_H
