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
  UnsignedByte  = GL_UNSIGNED_BYTE,
  Byte          = GL_BYTE,
  UnsignedShort = GL_UNSIGNED_SHORT,
  Short         = GL_SHORT,
  UnsignedInt   = GL_UNSIGNED_INT,
  Int           = GL_INT,
  HalfFloat     = GL_HALF_FLOAT,
  Float         = GL_FLOAT
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
  DepthComponent = GL_DEPTH_COMPONENT,
  DepthStencil   = GL_DEPTH_STENCIL,
  Red            = GL_RED,
  Rg             = GL_RG,
  Rgb            = GL_RGB,
  Rgba           = GL_RGBA,
  R8             = GL_R8,
  R16            = GL_R16,
  Rg8            = GL_RG8,
  Rg16           = GL_RG16,
  Rgb8           = GL_RGB8,
  Rgba8          = GL_RGBA8,
  Rgba16         = GL_RGBA16,
  R16F           = GL_R16F,
  Rg16F          = GL_RG16F,
  Rgb16F         = GL_RGB16F,
  Rgba16F        = GL_RGBA16F,
  R32F           = GL_R32F,
  Rg32F          = GL_RG32F,
  Rgb32F         = GL_RGB32F,
  Rgba32F        = GL_RGBA32F,
  R8I            = GL_R8I,
  R8Ui           = GL_R8UI,
  R16I           = GL_R16I,
  R16Ui          = GL_R16UI,
  R32I           = GL_R32I,
  R32Ui          = GL_R32UI,
  Rg8I           = GL_RG8I,
  Rg8Ui          = GL_RG8UI,
  Rg16I          = GL_RG16I,
  Rg16Ui         = GL_RG16UI,
  Rg32I          = GL_RG32I,
  Rg32Ui         = GL_RG32UI,
  Rgb8I          = GL_RGB8I,
  Rgb8Ui         = GL_RGB8UI,
  Rgb16I         = GL_RGB16I,
  Rgb16Ui        = GL_RGB16UI,
  Rgb32I         = GL_RGB32I,
  Rgb32Ui        = GL_RGB32UI,
  Rgba8I         = GL_RGBA8I,
  Rgba8Ui        = GL_RGBA8UI,
  Rgba16I        = GL_RGBA16I,
  Rgba16Ui       = GL_RGBA16UI,
  Rgba32I        = GL_RGBA32I,
  Rgba32Ui       = GL_RGBA32UI
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
  Red            = GL_RED,
  Rg             = GL_RG,
  Rgb            = GL_RGB,
  Bgr            = GL_BGR,
  Rgba           = GL_RGBA,
  Bgra           = GL_BGRA,
  RedInteger     = GL_RED_INTEGER,
  RgInteger      = GL_RG_INTEGER,
  RgbInteger     = GL_RGB_INTEGER,
  BgrInteger     = GL_BGR_INTEGER,
  RgbaInteger    = GL_RGBA_INTEGER,
  BgraInteger    = GL_BGRA_INTEGER,
  StencilIndex   = GL_STENCIL_INDEX,
  DepthComponent = GL_DEPTH_COMPONENT,
  DepthStencil   = GL_DEPTH_STENCIL
};

class Texture2D
{
public:
  Texture2D(
    int32_t width, int32_t height,
    TextureFormatInternal internalFormat = TextureFormatInternal::Rgb32F,
    TextureFormat         format         = TextureFormat::Rgb,
    TextureDataType       type           = TextureDataType::Float,
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
