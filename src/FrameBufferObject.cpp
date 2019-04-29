#include "prgl/FrameBufferObject.h"
#include "prgl/Texture2d.h"

#include "prgl/glCommon.h"

#include <iostream>

namespace prgl
{

FrameBufferObject::FrameBufferObject()
  : mHandlePtr(nullptr), mTarget(), mDepth()
{
  mHandlePtr = std::shared_ptr<uint32_t>(new uint32_t, [](uint32_t* ptr) {
    glDeleteFramebuffers(1, ptr);
    *ptr = INVALID_HANDLE;
    delete ptr;
    ptr = nullptr;
  });
  glGenFramebuffers(1, mHandlePtr.get());
}

FrameBufferObject::~FrameBufferObject() {}

uint32_t FrameBufferObject::getId() const { return *mHandlePtr; }

void FrameBufferObject::bind(bool bind) const
{
  if (bind)
    {
      glBindFramebuffer(GL_FRAMEBUFFER, *mHandlePtr);
    }
  else
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void FrameBufferObject::attachTexture(const Texture2d& texture)
{
  mTarget = texture;

  bind(true);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         mTarget.getId(), 0);

  bind(false);

  checkStatus();
}

void FrameBufferObject::attachDepth(const Texture2d& texture)
{
  mDepth = texture;

  bind(true);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         mDepth.getId(), 0);

  bind(false);

  checkStatus();
}

void FrameBufferObject::attachDepth(uint32_t width, uint32_t height)
{
  Texture2d depth(width, height, TextureFormatInternal::DepthComponent,
                  TextureFormat::DepthComponent, DataType::UnsignedShort,
                  TextureMinFilter::Linear, TextureMagFilter::Linear,
                  TextureEnvMode::Replace, TextureWrapMode::Repeat);
  attachDepth(depth);
}

const Texture2d& FrameBufferObject::getTarget() const { return mTarget; }

const Texture2d& FrameBufferObject::getDepth() const { return mDepth; }

bool FrameBufferObject::checkStatus()
{
  bind(true);

  // glReadBuffer(GL_NONE);

  int32_t status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  bool    result = false;

  switch (status)
    {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
      // std::cerr << "FRAMEBUFFER::Complete" << std::endl;
      result = true;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
      std::cerr << "[ERROR]FRAMEBUFFER::incomplete: Attachment is NOT complete"
                << std::endl;
      result = false;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
      std::cerr << "[ERROR]FRAMEBUFFER::incomplete: No image is attached to FBO"
                << std::endl;
      result = false;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
      std::cerr << "[ERROR]FRAMEBUFFER::incomplete: Attached images have "
                   "different dimensions"
                << std::endl;
      result = false;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
      std::cout << "[ERROR]FRAMEBUFFER::incomplete: Color attached images have "
                   "different internal formats"
                << std::endl;
      result = false;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
      std::cerr << "[ERROR]FRAMEBUFFER::incomplete: Draw buffer" << std::endl;
      result = false;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
      std::cerr << "[ERROR]FRAMEBUFFER::incomplete: Read buffer" << std::endl;
      result = false;
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
      std::cerr << "[ERROR]FRAMEBUFFER::Unsupported by FBO implementation"
                << std::endl;
      result = false;
      break;
    default:
      std::cerr << "[ERROR]FRAMEBUFFER::Unknow error" << std::endl;
      result = false;
      break;
    }

  bind(false);

  return result;
}

} // namespace prgl
