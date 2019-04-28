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

  /*
   * GL_READ_ONLY, GL_WRITE_ONLY, GL_READ_WRITE
   */
  void bindImage2D(uint32_t location, const std::shared_ptr<Texture2d>& texture,
                   TextureAccess access);
  // void bindSSBO(uint32_t location, std::shared_ptr<ShaderStorageBuffer>
  // &buffer);
  void bindSampler(uint32_t location, const std::string& name,
                   const std::shared_ptr<Texture2d>& sampler);

private:
  void attach(const std::string& source);

  std::array<int32_t, 3U> getWorkGroupSize();
  std::array<int32_t, 3U> getMaxWorkGroupSize() const;
  void dispatchCompute(uint32_t num_groups_x, uint32_t num_groups_y,
                       uint32_t num_groups_z) const;
  void memoryBarrier(GLbitfield barrierType = GL_ALL_BARRIER_BITS) const;

  std::map<uint32_t, std::shared_ptr<Texture2d>> mBindings;

  uint32_t mShaderHandle;
};

} // namespace prgl

#endif // PRGL_GLSL_COMPUTE_SHADER_H
