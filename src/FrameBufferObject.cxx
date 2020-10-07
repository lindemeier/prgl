#include "prgl/FrameBufferObject.hxx"

#include <iostream>

namespace prgl {
std::shared_ptr<FrameBufferObject> FrameBufferObject::Create() {
  return std::make_shared<FrameBufferObject>();
}

FrameBufferObject::FrameBufferObject() : mHandle(INVALID_HANDLE) {
  glGenFramebuffers(1, &mHandle);
}

FrameBufferObject::~FrameBufferObject() {
  glDeleteFramebuffers(1, &mHandle);
  mHandle = INVALID_HANDLE;
}

uint32_t FrameBufferObject::getId() const {
  return mHandle;
}

void FrameBufferObject::bind(bool bind) const {
  if (bind) {
    glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
  } else {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  if (mTarget == nullptr) {
    throw std::runtime_error("framebuffer: not target texture attached.");
  }
  glViewport(0, 0, static_cast<int32_t>(mTarget->getWidth()),
             static_cast<int32_t>(mTarget->getHeight()));
}

void FrameBufferObject::attachTexture(
  const std::shared_ptr<Texture2d>& texture) {
  mTarget = texture;

  bind(true);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         mTarget->getId(), 0);

  bind(false);

  checkStatus();
}

void FrameBufferObject::attachDepth(const std::shared_ptr<Texture2d>& texture) {
  mDepth = texture;

  bind(true);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         mDepth->getId(), 0);

  bind(false);

  checkStatus();
}

const std::shared_ptr<Texture2d>& FrameBufferObject::getTarget() const {
  return mTarget;
}

const std::shared_ptr<Texture2d>& FrameBufferObject::getDepth() const {
  return mDepth;
}

bool FrameBufferObject::checkStatus() const {
  bind(true);

  // glReadBuffer(GL_NONE);

  const auto status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  bool result    = false;

  switch (status) {
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

}  // namespace prgl
