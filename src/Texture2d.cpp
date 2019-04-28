#include "prgl/Texture2d.h"

#include <iostream>

namespace prgl
{

// Create empty texture
Texture2D::Texture2D(int32_t width, int32_t height,
                     TextureFormatInternal internalFormat, TextureFormat format,
                     TextureDataType type, int32_t minFilter, int32_t magFilter,
                     int32_t envMode, int32_t wrapMode, bool createMipMaps)
  : mHandle(0), mWidth(width), mHeight(height), mTarget(GL_TEXTURE_2D),
    mMipLevel(0), mInternalFormat(internalFormat), mFormat(format), mBorder(0),
    mType(type), mMinFilter(minFilter), mMagFilter(magFilter), mWrap(wrapMode),
    mEnvMode(envMode), mCreateMipMaps(createMipMaps), mMaxAnisotropy(1.0f)
{
  glGenTextures(1, &mHandle);
  if (mHandle == INVALID_HANDLE)
    {
      std::cerr << "invalid texture handle: " << std::endl;
      checkGLError(__FILE__, __FUNCTION__, __LINE__);
    }
}

Texture2D::~Texture2D() { cleanup(); }

void Texture2D::upload(void* data)
{
  bind(true);

  glTexImage2D(mTarget, mMipLevel, static_cast<uint32_t>(mInternalFormat),
               mWidth, mHeight, mBorder, static_cast<uint32_t>(mFormat),
               static_cast<uint32_t>(mType), data);

  if (mCreateMipMaps)
    {
      glTexParameteri(mTarget, GL_GENERATE_MIPMAP, GL_TRUE);
      glGenerateMipmap(GL_TEXTURE_2D);
    }

  glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, mMinFilter);
  glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, mMagFilter);

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mEnvMode);

  glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, mWrap);
  glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, mWrap);
  glTexParameteri(mTarget, GL_TEXTURE_WRAP_R, mWrap);

  glTexParameterf(mTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, mMaxAnisotropy);

  bind(false);
}

void Texture2D::download(std::vector<float>& data) const
{
  data.resize(mWidth * mHeight);
  bind(true);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, data.data());
  bind(false);
}

void Texture2D::download(std::vector<std::array<float, 3>>& data) const
{
  data.resize(mWidth * mHeight);
  bind(true);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, data.data());
  bind(false);
}

void Texture2D::download(std::vector<std::array<float, 4>>& data) const
{
  data.resize(mWidth * mHeight);
  bind(true);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, data.data());
  bind(false);
}

void Texture2D::download(std::vector<uint8_t>& data) const
{
  data.resize(mWidth * mHeight);
  bind(true);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, data.data());
  bind(false);
}

void Texture2D::download(std::vector<std::array<uint8_t, 3>>& data) const
{
  data.resize(mWidth * mHeight);
  bind(true);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
  bind(false);
}

void Texture2D::download(std::vector<std::array<uint8_t, 4>>& data) const
{
  data.resize(mWidth * mHeight);
  bind(true);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
  bind(false);
}

void Texture2D::bind(bool bind) const
{
  if (bind)
    {
      glBindTexture(mTarget, mHandle);
    }
  else
    {
      glBindTexture(mTarget, 0);
    }
}

void Texture2D::bindImageTexture(uint32_t unit, uint32_t access, int32_t level,
                                 bool layered, int32_t layer)
{
  glBindImageTexture(unit, mHandle, level, layered, layer, access,
                     static_cast<uint32_t>(mInternalFormat));
}

void Texture2D::cleanup()
{
  if (mHandle > INVALID_HANDLE)
    {
      glDeleteTextures(1, &mHandle);
    }
}

uint32_t Texture2D::getId() const { return mHandle; }

uint32_t Texture2D::getWidth() const { return mWidth; }

uint32_t Texture2D::getHeight() const { return mHeight; }

TextureFormatInternal Texture2D::getInternalFormat() const
{
  return mInternalFormat;
}

TextureFormat Texture2D::getFormat() const { return mFormat; }

int32_t Texture2D::getBorder() const { return mBorder; }

TextureDataType Texture2D::getType() const { return mType; }

int32_t Texture2D::getMinFilter() const { return mMinFilter; }

int32_t Texture2D::getMagFilter() const { return mMagFilter; }

int32_t Texture2D::getWrap() const { return mWrap; }

int32_t Texture2D::getEnvMode() const { return mEnvMode; }

uint32_t Texture2D::getTarget() const { return mTarget; }

void Texture2D::setWrapMode(int32_t wrap)
{
  mWrap = wrap;

  bind(true);

  glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, mWrap);
  glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, mWrap);
  glTexParameteri(mTarget, GL_TEXTURE_WRAP_R, mWrap);

  bind(false);
}

void Texture2D::setEnvMode(int32_t envMode)
{

  mEnvMode = envMode;

  bind(true);

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mEnvMode);

  bind(false);
}

void Texture2D::setFilter(int32_t minFilter, int32_t magFilter)
{
  mMinFilter = minFilter;
  mMagFilter = magFilter;

  bind(true);

  glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, mMinFilter);
  glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, mMagFilter);

  bind(false);
}

void Texture2D::setMaxIsotropy(float anisotropy)
{
  mMaxAnisotropy = anisotropy;

  bind(true);

  glTexParameterf(mTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, mMaxAnisotropy);

  bind(false);
}

void Texture2D::render(float posX, float posY, float width, float height)
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  // deactivate color clamping
  glClampColor(GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE);
  glClampColor(GL_CLAMP_READ_COLOR_ARB, GL_FALSE);
  glClampColor(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glColor4f(1.0, 1.0, 1.0, 1.0f);

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
  glTranslatef(posX, posY, 0.0f);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);

  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(width, 0.0f, 0.0f);

  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(width, height, 0.0f);

  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0.0, height, 0.0f);
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

void Texture2D::copyTo(Texture2D& other) const
{
  glCopyImageSubData(mHandle, mTarget, 0, 0, 0, 0, other.mHandle, other.mTarget,
                     0, 0, 0, 0, mWidth, mHeight, 1);
}

} // namespace prgl