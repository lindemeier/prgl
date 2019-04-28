/**
 * @file Texture2d.h
 * @author Thomas Lindemeier
 * @brief
 * @date 2019-01-02
 *
 */
#ifndef PRGL_TEXTURE_H
#define PRGL_TEXTURE_H

#include "glCommon.h"

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
  Nearest              = GL_NEAREST,
  Linear               = GL_LINEAR,
  NearestMipMapNearest = GL_NEAREST_MIPMAP_NEAREST,
  LinearMipMapNearest  = GL_LINEAR_MIPMAP_NEAREST,
  NearestMipMapLinear  = GL_NEAREST_MIPMAP_LINEAR,
  LinearMipMapLinear   = GL_LINEAR_MIPMAP_LINEAR
};

enum class TextureMagFilter : uint32_t
{
  Nearest = GL_NEAREST,
  Linear  = GL_LINEAR
};

enum class TextureEnvMode : uint32_t
{
  Replace     = GL_REPLACE,
  Modulate    = GL_MODULATE,
  Add         = GL_ADD,
  AddSigned   = GL_ADD_SIGNED,
  Interpolate = GL_INTERPOLATE,
  Subtract    = GL_SUBTRACT
};

enum class TextureWrapMode : uint32_t
{
  Repeat         = GL_REPEAT,
  MirroredRepeat = GL_MIRRORED_REPEAT,
  ClampToEdge    = GL_CLAMP_TO_EDGE,
  ClampToBorder  = GL_CLAMP_TO_BORDER
};

enum class TextureAccess : uint32_t
{
  ReadOnly  = GL_READ_ONLY,
  WriteOnly = GL_WRITE_ONLY,
  ReadWrite = GL_READ_WRITE
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

/**
 * @brief Represents a 2d texture.
 *
 */
class Texture2d
{
public:
  Texture2d(
    uint32_t width, uint32_t height,
    TextureFormatInternal internalFormat = TextureFormatInternal::Rgb32F,
    TextureFormat         format         = TextureFormat::Rgb,
    TextureDataType       type           = TextureDataType::Float,
    TextureMinFilter      minFilter      = TextureMinFilter::Linear,
    TextureMagFilter      magFilter      = TextureMagFilter::Linear,
    TextureEnvMode        envMode        = TextureEnvMode::Replace,
    TextureWrapMode       wrapMode       = TextureWrapMode::Repeat,
    bool                  createMipMaps  = false);

  ~Texture2d();

  void bind(bool bind) const;
  // used for binding at shader location for write andor read operations
  void     bindImageTexture(uint32_t      unit,
                            TextureAccess access = TextureAccess::ReadWrite,
                            int32_t level = 0, bool layered = GL_TRUE,
                            int32_t layer = 0);
  void     upload(void* data);
  void     download(std::vector<float>& data) const;
  void     download(std::vector<std::array<float, 3>>& data) const;
  void     download(std::vector<std::array<float, 4>>& data) const;
  void     download(std::vector<uint8_t>& data) const;
  void     download(std::vector<std::array<uint8_t, 3>>& data) const;
  void     download(std::vector<std::array<uint8_t, 4>>& data) const;
  void     setWrapMode(TextureWrapMode wrap);
  void     setEnvMode(TextureEnvMode envMode);
  void     setFilter(TextureMinFilter minFilter, TextureMagFilter magFilter);
  void     setMaxIsotropy(float anisotropy);
  void     render(float posX, float posY, float width, float height);
  uint32_t getId() const;
  uint32_t getWidth() const;
  uint32_t getHeight() const;

  TextureFormatInternal getInternalFormat() const;
  TextureFormat         getFormat() const;
  int32_t               getBorder() const;
  TextureDataType       getType() const;
  TextureMinFilter      getMinFilter() const;
  TextureMagFilter      getMagFilter() const;
  TextureWrapMode       getWrap() const;
  TextureEnvMode        getEnvMode() const;
  void                  copyTo(Texture2d& other) const;

private:
  void cleanup();

private:
  uint32_t              mHandle;
  uint32_t              mWidth;
  uint32_t              mHeight;
  uint32_t              mTarget;
  int32_t               mMipLevel;
  TextureFormatInternal mInternalFormat;
  TextureFormat         mFormat;
  int32_t               mBorder;
  TextureDataType       mType;
  TextureMinFilter      mMinFilter;
  TextureMagFilter      mMagFilter;
  TextureWrapMode       mWrap;
  TextureEnvMode        mEnvMode;
  bool                  mCreateMipMaps;
  float                 mMaxAnisotropy;
};

} // namespace prgl

#endif // PRGL_TEXTURE_H
