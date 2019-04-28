/**
 * @file GlslRenderingPipelineProgram.h
 * @author Thomas Lindemeier
 * @brief
 * @date 2019-01-02
 *
 */
#ifndef PRGL_GLSL_RENDERING_PIPELINE_PROGRAM_H
#define PRGL_GLSL_RENDERING_PIPELINE_PROGRAM_H

#include "prgl/GlslProgram.h"
#include "prgl/ShaderStorageBuffer.h"
#include "prgl/Texture2d.h"

#include <array>
#include <memory>

namespace prgl
{

/**
 * @brief Program for vertex, tesselation, geometry and fragment shaders.
 *
 */
class GlslRenderingPipelineProgram final : public GlslProgram
{
public:
  GlslRenderingPipelineProgram();

  ~GlslRenderingPipelineProgram() override;

  void bindSampler(const std::string& name, const TextureUnit unit,
                   const std::shared_ptr<Texture2d>& texture);

  void execute2D(const std::shared_ptr<Texture2d>& target, int32_t x, int32_t y,
                 int32_t w, int32_t h);

  void attachVertexShader(const std::string& source);
  void attachTesselationControlShader(const std::string& source);
  void attachTesselationEvaluationShader(const std::string& source);
  void attachGeometryShader(const std::string& source);
  void attachFragmentShader(const std::string& source);

private:
  uint32_t mVertProg;
  uint32_t mTesselationControlProg;
  uint32_t mTesselationEvaluationProg;
  uint32_t mGeometryProg;
  uint32_t mFragProg;
};

} // namespace prgl

#endif // PRGL_GLSL_RENDERING_PIPELINE_PROGRAM_H
