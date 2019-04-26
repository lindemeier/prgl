/**
 * @file Texture2d.h
 * @author Thomas Lindemeier
 * @brief
 * @date 2019-01-02
 *
 */
#ifndef PRGL_TEXTURE_H
#define PRGL_TEXTURE_H

#include <GL/glew.h>

#include <array>
#include <vector>

namespace prgl
{

enum class TextureMinFilter : int32_t
{
  Nearest,
  Linear,
  NearestMipMapNearest,
  LinearMipMapNearest,
  NearestMipMapLinear,
  LinearMipMapLinear
};

enum class TextureMagFilter : int32_t
{
  Nearest,
  Linear
};

enum class TextureEnvMode : int32_t
{
  Replace,
  Modulate,
  Add,
  AddSigned,
  Interpolate,
  Subtract
};

enum class TextureWrapMode : int32_t
{
  Repeat,
  MirroredRepeat,
  ClampToEdge,
  ClampToBorder
};

enum class TextureAccess : int32_t
{
  ReadOnly,
  WriteOnly,
  ReadWrite
};

enum class TextureFormat : int32_t
{
  R8U,
  Rg8U,
  Rgb8U,
  Rgba8U,
  R16U,
  Rg16U,
  Rgb16U,
  Rgba16U,
  R32U,
  Rg32U,
  Rgb32U,
  Rgba32U,
};

class Texture2D
{
public:
  Texture2D();
  Texture2D(int32_t width, int32_t height, int32_t internalFormat = GL_RGB32F,
            uint32_t format = GL_RGB, int32_t type = GL_FLOAT,
            int32_t minFilter = GL_LINEAR, int32_t magFilter = GL_LINEAR,
            int32_t envMode = GL_REPLACE, int32_t wrapMode = GL_REPEAT,
            bool createMipMaps = GL_FALSE);

  ~Texture2D();

  void bind(bool bind) const;
  // used for binding at shader location for write andor read operations,
  // acess:: GL_READ_ONLY, GL_WRITE_ONLY, or GL_READ_WRITE
  void     bindImageTexture(uint32_t unit, uint32_t access = GL_READ_WRITE,
                            int32_t level = 0, bool layered = GL_TRUE,
                            int32_t layer = 0);
  void     create(void* data = nullptr);
  void     upload(void* data);
  void     download(std::vector<float>& data) const;
  void     download(std::vector<std::array<float, 3>>& data) const;
  void     download(std::vector<std::array<float, 4>>& data) const;
  void     download(std::vector<uint8_t>& data) const;
  void     download(std::vector<std::array<uint8_t, 3>>& data) const;
  void     download(std::vector<std::array<uint8_t, 4>>& data) const;
  void     setWrapMode(int32_t wrap);
  void     setEnvMode(int32_t envMode);
  void     setFilter(int32_t minFilter, int32_t magFilter);
  void     setMaxIsotropy(float anisotropy);
  void     render(float posX, float posY, float width, float height);
  uint32_t getId() const;
  uint32_t getWidth() const;
  uint32_t getHeight() const;
  bool     isCreated() const { return mCreated; }

  int32_t  getInternalFormat() const;
  uint32_t getFormat() const;
  int32_t  getBorder() const;
  uint32_t getType() const;
  int32_t  getMinFilter() const;
  int32_t  getMagFilter() const;
  int32_t  getWrap() const;
  int32_t  getEnvMode() const;
  uint32_t getTarget() const;
  void     copyTo(Texture2D& other) const;

private:
  void deleteTex();

private:
  uint32_t mId;
  uint32_t mWidth;
  uint32_t mHeight;
  uint32_t mTarget;
  int32_t  mMipLevel;
  int32_t  mInternalFormat;
  uint32_t mFormat;
  int32_t  mBorder;
  uint32_t mType;
  int32_t  mMinFilter;
  int32_t  mMagFilter;
  int32_t  mWrap;
  int32_t  mEnvMode;
  bool     mCreateMipMaps;
  float    mMaxAnisotropy;
  bool     mCreated;
};

} // namespace prgl

#endif // PRGL_TEXTURE_H
