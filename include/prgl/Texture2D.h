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

/**
 * @brief "Specifies the data type of the pixel data. The following symbolic
 * values are accepted:""
 *
 */
enum class TextureDataType : uint32_t
{
  UNSIGNED_BYTE  = GL_UNSIGNED_BYTE,
  BYTE           = GL_BYTE,
  UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
  SHORT          = GL_SHORT,
  UNSIGNED_INT   = GL_UNSIGNED_INT,
  INT            = GL_INT,
  HALF_FLOAT     = GL_HALF_FLOAT,
  FLOAT          = GL_FLOAT
};

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
 * below."
 *
 * "internalFormat (2nd argument) defines the format that OpenGL should use to
 * store the data internally."
 */
enum class TextureFormatInternal : uint32_t
{
  DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
  DEPTH_STENCIL   = GL_DEPTH_STENCIL,
  RED             = GL_RED,
  RG              = GL_RG,
  RGB             = GL_RGB,
  RGBA            = GL_RGBA,
  R8              = GL_R8,
  R16             = GL_R16,
  RG8             = GL_RG8,
  RG16            = GL_RG16,
  RGB8            = GL_RGB8,
  RGBA8           = GL_RGBA8,
  RGBA16          = GL_RGBA16,
  R16F            = GL_R16F,
  RG16F           = GL_RG16F,
  RGB16F          = GL_RGB16F,
  RGBA16F         = GL_RGBA16F,
  R32F            = GL_R32F,
  RG32F           = GL_RG32F,
  RGB32F          = GL_RGB32F,
  RGBA32F         = GL_RGBA32F,
  R8I             = GL_R8I,
  R8UI            = GL_R8UI,
  R16I            = GL_R16I,
  R16UI           = GL_R16UI,
  R32I            = GL_R32I,
  R32UI           = GL_R32UI,
  RG8I            = GL_RG8I,
  RG8UI           = GL_RG8UI,
  RG16I           = GL_RG16I,
  RG16UI          = GL_RG16UI,
  RG32I           = GL_RG32I,
  RG32UI          = GL_RG32UI,
  RGB8I           = GL_RGB8I,
  RGB8UI          = GL_RGB8UI,
  RGB16I          = GL_RGB16I,
  RGB16UI         = GL_RGB16UI,
  RGB32I          = GL_RGB32I,
  RGB32UI         = GL_RGB32UI,
  RGBA8I          = GL_RGBA8I,
  RGBA8UI         = GL_RGBA8UI,
  RGBA16I         = GL_RGBA16I,
  RGBA16UI        = GL_RGBA16UI,
  RGBA32I         = GL_RGBA32I,
  RGBA32UI        = GL_RGBA32UI
};

/**
 * @brief "Specifies the format of the pixel data. The following symbolic values
 * are accepted:"
 *
 * "The format together with the type argument, describes the
 * data you pass in as the last argument. So the format/type combination defines
 * the memory layout of the data you pass in."
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
  Texture2D(
    int32_t width, int32_t height,
    TextureFormatInternal internalFormat = TextureFormatInternal::RGB32F,
    TextureFormat         format         = TextureFormat::RGB,
    TextureDataType       type           = TextureDataType::FLOAT,
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

  TextureFormatInternal getInternalFormat() const;
  TextureFormat         getFormat() const;
  int32_t               getBorder() const;
  TextureDataType       getType() const;
  int32_t               getMinFilter() const;
  int32_t               getMagFilter() const;
  int32_t               getWrap() const;
  int32_t               getEnvMode() const;
  uint32_t              getTarget() const;
  void                  copyTo(Texture2D& other) const;

private:
  void deleteTex();

private:
  uint32_t              mId;
  uint32_t              mWidth;
  uint32_t              mHeight;
  uint32_t              mTarget;
  int32_t               mMipLevel;
  TextureFormatInternal mInternalFormat;
  TextureFormat         mFormat;
  int32_t               mBorder;
  TextureDataType       mType;
  int32_t               mMinFilter;
  int32_t               mMagFilter;
  int32_t               mWrap;
  int32_t               mEnvMode;
  bool                  mCreateMipMaps;
  float                 mMaxAnisotropy;
  bool                  mCreated;
};

} // namespace prgl

#endif // PRGL_TEXTURE_H
