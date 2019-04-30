#include "prgl/GlslComputeShader.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace prgl
{

GlslComputeShader::GlslComputeShader(const std::string& glslSource)
  : GlslProgram(), mShaderHandlePtr(create())
{
  attach(glslSource);
}

GlslComputeShader::~GlslComputeShader() {}

std::shared_ptr<uint32_t> GlslComputeShader::create()
{
  auto shaderPtr =
    std::shared_ptr<uint32_t>(new uint32_t, [this](uint32_t* ptr) {
      if (*mProgHandlePtr > INVALID_HANDLE && *ptr > INVALID_HANDLE)
        {
          glDetachShader(*mProgHandlePtr, *ptr);
          glDeleteShader(*ptr);
          *ptr = INVALID_HANDLE;
          delete ptr;
          ptr = nullptr;
        }
    });
  return shaderPtr;
}

void GlslComputeShader::attach(const std::string& source)
{
  if (!source.empty())
    {
      // compile the shader
      *mShaderHandlePtr = compile(source.c_str(), GL_COMPUTE_SHADER);
      glAttachShader(*mProgHandlePtr, *mShaderHandlePtr);

      glLinkProgram(*mProgHandlePtr);
      int32_t linkV;
      glGetProgramiv(*mProgHandlePtr, GL_LINK_STATUS, &linkV);

      if (!linkV)
        {
          std::cerr << "Error in linking compute shader program" << std::endl;
          GLchar  log[10240];
          GLsizei length;
          glGetProgramInfoLog(*mProgHandlePtr, 10239, &length, log);
          std::cerr << source << "\n" << log << std::endl;

          std::stringstream ss;
          ss << "ComputeShader() : linking error with source : " << source
             << "\nerror:\n"
             << log << std::endl;
          throw std::runtime_error(ss.str());
        }
    }
  else
    {
      std::stringstream ss;
      ss << "ComputeShader() : empty : " << source << std::endl;
      throw std::runtime_error(ss.str());
    }
}

vec3i GlslComputeShader::getWorkGroupSize()
{
  vec3i size;
  glGetProgramiv(*mProgHandlePtr, GL_COMPUTE_WORK_GROUP_SIZE, &(size[0]));
  return size;
}

vec3i GlslComputeShader::getMaxWorkGroupSize() const
{
  vec3i size;
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &(size[0]));
  return size;
}

void GlslComputeShader::dispatchCompute(uint32_t num_groups_x,
                                        uint32_t num_groups_y,
                                        uint32_t num_groups_z) const
{
  glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}

void GlslComputeShader::memoryBarrier(GLbitfield barrierType) const
{
  glMemoryBarrier(barrierType);
}

/**
 * @brief Dispatch shader at region of interest.
 *
 * @param x
 * @param y
 * @param w
 * @param h
 */
void GlslComputeShader::execute(int32_t x, int32_t y, int32_t w, int32_t h)
{
  bind(true);

  set2i("offset", x, y);

  const auto workSize = getWorkGroupSize();
  dispatchCompute(w / workSize[0] + 1U, h / workSize[1] + 1U, 1U);
  memoryBarrier(GL_ALL_BARRIER_BITS);

  bind(false);
}

/**
 * @brief Bind texture as image2d.
 *
 * @param location
 * @param texture
 * @param access
 */
void GlslComputeShader::bindImage2D(uint32_t location, Texture2d& texture,
                                    TextureAccess access)
{
  bind(true);
  texture.bindImageTexture(location, access);
  bind(false);
}

/**
 * @brief Bind Shader Storage Buffer.
 *
 * @param location
 * @param buffer
 */
void GlslComputeShader::bindSSBO(uint32_t location, ShaderStorageBuffer& buffer)
{
  bind(true);
  buffer.bindBase(location);
  bind(false);
}

} // namespace prgl
