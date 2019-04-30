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

  void attachTexture(const std::shared_ptr<Texture2d>& texture);
  void attachDepth(const std::shared_ptr<Texture2d>& texture);

  const std::shared_ptr<Texture2d>& getTarget() const;
  const std::shared_ptr<Texture2d>& getDepth() const;

private:
  FrameBufferObject(const FrameBufferObject&) = delete;
  FrameBufferObject& operator=(const FrameBufferObject&) = delete;

  bool checkStatus();

  uint32_t                   mHandle;
  std::shared_ptr<Texture2d> mTarget;
  std::shared_ptr<Texture2d> mDepth;
};

} // namespace prgl

#endif // PRGL_FRAMEBUFFEROBJECT_H
