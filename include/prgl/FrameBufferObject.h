/**
 * @file FrameBufferObject.h
 * @author Thomas Lindemeier
 * @brief
 * @date 2019-01-02
 *
 */
#ifndef PRGL_FRAMEBUFFEROBJECT_H
#define PRGL_FRAMEBUFFEROBJECT_H

#include "prgl/Texture2d.h"

#include <memory>

namespace prgl
{

class Texture2D;

class FrameBufferObject
{
  uint32_t                   mHandle;
  std::shared_ptr<Texture2D> mTarget;
  std::shared_ptr<Texture2D> mDepth;

  bool checkStatus();

public:
  FrameBufferObject();
  ~FrameBufferObject();

  uint32_t id() const;

  void bind(bool bind);

  void attachTexture(const std::shared_ptr<Texture2D>& texture);
  void attachDepth(const std::shared_ptr<Texture2D>& texture);
  void attachDepth(uint32_t width, uint32_t height);

  const std::shared_ptr<Texture2D>& getTarget() const;
  const std::shared_ptr<Texture2D>& getDepth() const;
};

} // namespace prgl

#endif // PRGL_FRAMEBUFFEROBJECT_H
