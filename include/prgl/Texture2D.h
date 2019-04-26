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

enum class TextureMinFilter : uint32_t
{
  Nearest,
  Linear,
  NearestMipMapNearest,
  LinearMipMapNearest,
  NearestMipMapLinear,
  LinearMipMapLinear
};

enum class TextureMagFilter : uint32_t
{
  Nearest,
  Linear
};

enum class TextureEnvMode : uint32_t
{
  Replace,
  Modulate,
  Add,
  AddSigned,
  Interpolate,
  Subtract
};

enum class TextureWrapMode : uint32_t
{
  Repeat,
  MirroredRepeat,
  ClampToEdge,
  ClampToBorder
};

enum class TextureAccess : uint32_t
{
  ReadOnly,
  WriteOnly,
  ReadWrite
};

/**
 * @brief "Specifies the number of color components in the texture. Must be one
 * of base internal formats given in Table 1, one of the sized internal formats
 * given in Table 2, or one of the compressed internal formats given in Table 3,
 * below.""
 */
enum class TextureFormatInternal : uint32_t
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

/**
 * @brief "Specifies the format of the pixel data. The following symbolic values
 * are accepted:"
 */
enum class TextureFormat : uint32_t
{
  RED             = GL_RED,
  RG              = GL_RG,
  RGB             = GL_RGB,
  BGR             = GL_BGR,
  RGBA            = GL_RGBA,
  BGRA            = GL_BGRA,
  RED_INTEGER     = GL_RED_INTEGER,
  RG_INTEGER      = GL_RG_INTEGER,
  RGB_INTEGER     = GL_RGB_INTEGER,
  BGR_INTEGER     = GL_BGR_INTEGER,
  RGBA_INTEGER    = GL_RGBA_INTEGER,
  BGRA_INTEGER    = GL_BGRA_INTEGER,
  STENCIL_INDEX   = GL_STENCIL_INDEX,
  DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
  DEPTH_STENCIL   = GL_DEPTH_STENCIL
};

class Texture2D
{
public:
  Texture2D();
  Texture2D(int32_t width, int32_t height, int32_t internalFormat = GL_RGB32F,
            TextureFormat format = TextureFormat::RGB, int32_t type = GL_FLOAT,
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

  int32_t       getInternalFormat() const;
  TextureFormat getFormat() const;
  int32_t       getBorder() const;
  uint32_t      getType() const;
  int32_t       getMinFilter() const;
  int32_t       getMagFilter() const;
  int32_t       getWrap() const;
  int32_t       getEnvMode() const;
  uint32_t      getTarget() const;
  void          copyTo(Texture2D& other) const;

private:
  void deleteTex();

private:
  uint32_t      mId;
  uint32_t      mWidth;
  uint32_t      mHeight;
  uint32_t      mTarget;
  int32_t       mMipLevel;
  int32_t       mInternalFormat;
  TextureFormat mFormat;
  int32_t       mBorder;
  uint32_t      mType;
  int32_t       mMinFilter;
  int32_t       mMagFilter;
  int32_t       mWrap;
  int32_t       mEnvMode;
  bool          mCreateMipMaps;
  float         mMaxAnisotropy;
  bool          mCreated;
};

} // namespace prgl

#endif // PRGL_TEXTURE_H
