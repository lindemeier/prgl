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

class FrameBufferObject final
{
public:
  static std::shared_ptr<FrameBufferObject> Create();

  FrameBufferObject();
  ~FrameBufferObject();

  uint32_t getId() const;

  void bind(bool bind) const;

  void attachTexture(const Texture2d& texture);
  void attachDepth(const Texture2d& texture);
  void attachDepth(uint32_t width, uint32_t height);

  const Texture2d& getTarget() const;
  const Texture2d& getDepth() const;

private:
  FrameBufferObject(const FrameBufferObject&) = delete;
  FrameBufferObject& operator=(const FrameBufferObject&) = delete;

  bool checkStatus();

  uint32_t  mHandle;
  Texture2d mTarget;
  Texture2d mDepth;
};

} // namespace prgl

#endif // PRGL_FRAMEBUFFEROBJECT_H
