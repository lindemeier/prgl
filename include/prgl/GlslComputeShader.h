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
  static std::shared_ptr<GlslComputeShader>
  Create(const std::string& glslSource);

  GlslComputeShader(const std::string& glslSource);

  ~GlslComputeShader() override;

  void execute(int32_t x, int32_t y, int32_t w, int32_t h);

  void bindImage2D(uint32_t location, const std::shared_ptr<Texture2d>& texture,
                   TextureAccess access);
  void bindSSBO(uint32_t                                    location,
                const std::shared_ptr<ShaderStorageBuffer>& buffer);

private:
  GlslComputeShader(const GlslComputeShader&) = delete;
  GlslComputeShader& operator=(const GlslComputeShader&) = delete;

  void attach(const std::string& source);

  vec3i getWorkGroupSize();
  vec3i getMaxWorkGroupSize() const;
  void  dispatchCompute(uint32_t num_groups_x, uint32_t num_groups_y,
                        uint32_t num_groups_z) const;
  void  memoryBarrier(GLbitfield barrierType = GL_ALL_BARRIER_BITS) const;

  uint32_t mShaderHandle;
};

} // namespace prgl

#endif // PRGL_GLSL_COMPUTE_SHADER_H
