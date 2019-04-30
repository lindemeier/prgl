/**
 * @file GlslComputeShader.h
 * @author Thomas Lindemeier
 * @brief
 * @date 2019-01-02
 *
 */
#ifndef PRGL_GLSL_COMPUTE_SHADER_H
#define PRGL_GLSL_COMPUTE_SHADER_H

#include "prgl/GlslProgram.h"
#include "prgl/ShaderStorageBuffer.h"
#include "prgl/Texture2d.h"

#include <array>
#include <functional>
#include <map>
#include <memory>

namespace prgl
{

/**
 * @brief GLSL compute shader.
 *
 */
class GlslComputeShader final : public GlslProgram
{
public:
  GlslComputeShader(const std::string& glslSource);

  ~GlslComputeShader() override;

  void execute(int32_t x, int32_t y, int32_t w, int32_t h);

  void bindImage2D(uint32_t location, Texture2d& texture, TextureAccess access);
  void bindSSBO(uint32_t location, ShaderStorageBuffer& buffer);

private:
  std::shared_ptr<uint32_t> create();

  void attach(const std::string& source);

  vec3i getWorkGroupSize();
  vec3i getMaxWorkGroupSize() const;
  void  dispatchCompute(uint32_t num_groups_x, uint32_t num_groups_y,
                        uint32_t num_groups_z) const;
  void  memoryBarrier(GLbitfield barrierType = GL_ALL_BARRIER_BITS) const;

  std::shared_ptr<uint32_t> mShaderHandlePtr;
};

} // namespace prgl

#endif // PRGL_GLSL_COMPUTE_SHADER_H
