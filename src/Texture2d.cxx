#include "prgl/Texture2d.hxx"

#include <iostream>

namespace prgl {
// Create empty texture
Texture2d::Texture2d()
    : Texture2d(0, 0, TextureFormatInternal::Rgb32F, TextureFormat::Rgb,
                DataType::Float, TextureMinFilter::Linear,
                TextureMagFilter::Linear, TextureEnvMode::Replace,
                TextureWrapMode::Repeat, false) {}

Texture2d::Texture2d(uint32_t width, uint32_t height,
                     TextureFormatInternal internalFormat, TextureFormat format,
                     DataType type, TextureMinFilter minFilter,
                     TextureMagFilter magFilter, TextureEnvMode envMode,
                     TextureWrapMode wrapMode, bool createMipMaps)
    : mHandle(INVALID_HANDLE),
      mWidth(width),
      mHeight(height),
      mTarget(GL_TEXTURE_2D),
      mMipLevel(0),
      mInternalFormat(internalFormat),
      mFormat(format),
      mBorder(0),
      mType(type),
      mMinFilter(minFilter),
      mMagFilter(magFilter),
      mWrap(wrapMode),
      mEnvMode(envMode),
      mCreateMipMaps(createMipMaps),
      mMaxAnisotropy(1.0F) {
  glGenTextures(1, &mHandle);
}

Texture2d::~Texture2d() {
  glDeleteTextures(1, &mHandle);
  mHandle = INVALID_HANDLE;
}

void Texture2d::upload(void* data) {
  bind(true);

  glTexImage2D(mTarget, mMipLevel, static_cast<GLint>(mInternalFormat),
               static_cast<GLint>(mWidth), static_cast<GLint>(mHeight),
               static_cast<GLint>(mBorder), static_cast<GLuint>(mFormat),
               static_cast<GLuint>(mType), data);

  if (mCreateMipMaps) {
    glTexParameteri(mTarget, GL_GENERATE_MIPMAP, GL_TRUE);
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER,
                  static_cast<GLint>(mMinFilter));
  glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER,
                  static_cast<GLint>(mMagFilter));

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, static_cast<GLint>(mEnvMode));

  glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, static_cast<GLint>(mWrap));
  glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, static_cast<GLint>(mWrap));
  glTexParameteri(mTarget, GL_TEXTURE_WRAP_R, static_cast<GLint>(mWrap));

  glTexParameterf(mTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, mMaxAnisotropy);

  bind(false);
}

void Texture2d::download(void* dataPtr, const TextureFormat format,
                         const DataType type) {
  bind(true);
  glGetTexImage(GL_TEXTURE_2D, 0, static_cast<GLenum>(format),
                static_cast<GLenum>(type), dataPtr);
  bind(false);
}

void Texture2d::bind(bool bind) const {
  if (bind) {
    glBindTexture(mTarget, mHandle);
  } else {
    glBindTexture(mTarget, 0);
  }
}

/**
 * @brief This should be preferred.
 *
 * @param unit
 */
void Texture2d::bindUnit(uint32_t unit) const {
  // glActiveTexture(static_cast<GLenum>(unit));
  // glBindTexture(mTarget, mHandle);
  glBindTextureUnit(static_cast<GLuint>(unit), mHandle);
}

void Texture2d::bindImageTexture(uint32_t unit, TextureAccess access,
                                 int32_t level, bool layered, int32_t layer) {
  glBindImageTexture(static_cast<GLuint>(unit), mHandle, level,
                     static_cast<GLboolean>(layered), layer,
                     static_cast<uint32_t>(access),
                     static_cast<uint32_t>(mInternalFormat));
}

uint32_t Texture2d::getId() const {
  return mHandle;
}

uint32_t Texture2d::getWidth() const {
  return mWidth;
}

uint32_t Texture2d::getHeight() const {
  return mHeight;
}

TextureFormatInternal Texture2d::getInternalFormat() const {
  return mInternalFormat;
}

TextureFormat Texture2d::getFormat() const {
  return mFormat;
}

int32_t Texture2d::getBorder() const {
  return mBorder;
}

DataType Texture2d::getType() const {
  return mType;
}

TextureMinFilter Texture2d::getMinFilter() const {
  return mMinFilter;
}

TextureMagFilter Texture2d::getMagFilter() const {
  return mMagFilter;
}

TextureWrapMode Texture2d::getWrap() const {
  return mWrap;
}

TextureEnvMode Texture2d::getEnvMode() const {
  return mEnvMode;
}

void Texture2d::setWrapMode(TextureWrapMode wrap) {
  mWrap = wrap;

  bind(true);

  glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, static_cast<GLint>(mWrap));
  glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, static_cast<GLint>(mWrap));
  glTexParameteri(mTarget, GL_TEXTURE_WRAP_R, static_cast<GLint>(mWrap));

  bind(false);
}

void Texture2d::setEnvMode(TextureEnvMode envMode) {
  mEnvMode = envMode;

  bind(true);

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, static_cast<GLint>(mEnvMode));

  bind(false);
}

void Texture2d::setFilter(TextureMinFilter minFilter,
                          TextureMagFilter magFilter) {
  mMinFilter = minFilter;
  mMagFilter = magFilter;

  bind(true);

  glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER,
                  static_cast<GLint>(mMinFilter));
  glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER,
                  static_cast<GLint>(mMagFilter));

  bind(false);
}

void Texture2d::setMaxIsotropy(float anisotropy) {
  mMaxAnisotropy = anisotropy;

  bind(true);

  glTexParameterf(mTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, mMaxAnisotropy);

  bind(false);
}

void Texture2d::render(float posX, float posY, float width, float height,
                       bool convert_sRGB) const {
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  if (convert_sRGB) {
    glEnable(GL_FRAMEBUFFER_SRGB);
  } else {
    glDisable(GL_FRAMEBUFFER_SRGB);
  }
  // deactivate color clamping
  glClampColor(GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE);
  glClampColor(GL_CLAMP_READ_COLOR_ARB, GL_FALSE);
  glClampColor(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glColor4f(1.0, 1.0, 1.0, 1.0F);

  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  bind(true);

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  glDisable(GL_DEPTH_TEST);

  int32_t iViewport[4];
  glGetIntegerv(GL_VIEWPORT, iViewport);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glOrtho(iViewport[0], iViewport[0] + iViewport[2],
          iViewport[1] + iViewport[3], iViewport[1], -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  glPushMatrix();
  glTranslatef(posX, posY, 0.0F);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0F, 1.0F);
  glVertex3f(0.0F, 0.0F, 0.0F);

  glTexCoord2f(1.0F, 1.0F);
  glVertex3f(width, 0.0F, 0.0F);

  glTexCoord2f(1.0F, 0.0F);
  glVertex3f(width, height, 0.0F);

  glTexCoord2f(0.0F, 0.0F);
  glVertex3f(0.0, height, 0.0F);
  glEnd();
  glPopMatrix();

  glPopAttrib();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  bind(false);

  glDisable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);

  glPopAttrib();
}

void Texture2d::copyTo(Texture2d& other) const {
  glCopyImageSubData(mHandle, mTarget, 0, 0, 0, 0, other.mHandle, other.mTarget,
                     0, 0, 0, 0, mWidth, mHeight, 1);
}

}  // namespace prgl
