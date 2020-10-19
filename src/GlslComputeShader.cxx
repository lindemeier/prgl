#include "prgl/GlslComputeShader.hxx"

#include <fstream>
#include <iostream>
#include <sstream>

namespace prgl {

std::shared_ptr<GlslComputeShader> GlslComputeShader::Create(
  const std::string& glslSource) {
  return std::make_shared<GlslComputeShader>(glslSource);
}

GlslComputeShader::GlslComputeShader(const std::string& glslSource)
    : mShaderHandle(INVALID_HANDLE) {
  attach(glslSource);
}

GlslComputeShader::~GlslComputeShader() {
  if (mProgHandle > INVALID_HANDLE) {
    if (mShaderHandle > INVALID_HANDLE) {
      glDetachShader(mProgHandle, mShaderHandle);
    }
  }
  if (mShaderHandle > INVALID_HANDLE) {
    glDeleteShader(mShaderHandle);
    mShaderHandle = INVALID_HANDLE;
  }
}

void GlslComputeShader::attach(const std::string& source) {
  if (!source.empty()) {
    // compile the shader
    mShaderHandle = compile(source, GL_COMPUTE_SHADER);
    glAttachShader(mProgHandle, mShaderHandle);

    glLinkProgram(mProgHandle);
    int32_t linkV = 0;
    glGetProgramiv(mProgHandle, GL_LINK_STATUS, &linkV);

    if (linkV == 0) {
      std::cerr << "Error in linking compute shader program" << std::endl;
      GLchar log[10240];
      GLsizei length = 0;
      glGetProgramInfoLog(mProgHandle, 10239, &length, log);
      std::cerr << source << "\n" << log << std::endl;

      std::stringstream ss;
      ss << "ComputeShader() : linking error with source : " << source
         << "\nerror:\n"
         << log << std::endl;
      throw std::runtime_error(ss.str());
    }
  } else {
    std::stringstream ss;
    ss << "ComputeShader() : empty : " << source << std::endl;
    throw std::runtime_error(ss.str());
  }
}

vec3ui GlslComputeShader::getWorkGroupSize() const {
  vec3i size;
  glGetProgramiv(mProgHandle, GL_COMPUTE_WORK_GROUP_SIZE, &(size[0]));
  return {static_cast<uint32_t>(size[0U]), static_cast<uint32_t>(size[1U]),
          static_cast<uint32_t>(size[2U])};
}

vec3ui GlslComputeShader::getMaxWorkGroupSize() {
  vec3i size;
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &(size[0]));
  return {static_cast<uint32_t>(size[0U]), static_cast<uint32_t>(size[1U]),
          static_cast<uint32_t>(size[2U])};
}

void GlslComputeShader::dispatchCompute(uint32_t num_groups_x,
                                        uint32_t num_groups_y,
                                        uint32_t num_groups_z) {
  glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}

void GlslComputeShader::memoryBarrier(GLbitfield barrierType) {
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
void GlslComputeShader::execute(int32_t x, int32_t y, int32_t w, int32_t h) {
  if (!isBound()) {
    throw std::runtime_error(
      "trying to execute program that is not the currently bound program.");
  }

  set2i("offset", x, y);

  const auto workSize = getWorkGroupSize();
  dispatchCompute((static_cast<uint32_t>(w) / workSize[0U]) + 1U,
                  (static_cast<uint32_t>(h) / workSize[1U]) + 1U, 1U);
  memoryBarrier(GL_ALL_BARRIER_BITS);
}

/**
 * @brief Bind texture as image2d.
 *
 * @param location
 * @param texture
 * @param access
 */
void GlslComputeShader::bindImage2D(uint32_t unit,
                                    const std::shared_ptr<Texture2d>& texture,
                                    TextureAccess access) {
  if (!isBound()) {
    throw std::runtime_error(
      "trying to bind image to program that is not the currently bound "
      "program.");
  }
  texture->bindImageTexture(unit, access);
}

/**
 * @brief Bind Shader Storage Buffer.
 *
 * @param location
 * @param buffer
 */
void GlslComputeShader::bindSSBO(
  uint32_t location, const std::shared_ptr<ShaderStorageBuffer>& buffer) {
  if (!isBound()) {
    throw std::runtime_error(
      "trying to bind SSBO to program that is not the currently bound "
      "program.");
  }
  buffer->bindBase(location);
}

}  // namespace prgl
