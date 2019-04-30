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

  void attachVertexShader(const std::string& source);
  void attachTesselationControlShader(const std::string& source);
  void attachTesselationEvaluationShader(const std::string& source);
  void attachGeometryShader(const std::string& source);
  void attachFragmentShader(const std::string& source);

private:
  std::shared_ptr<uint32_t> create(const std::string& source,
                                   GLenum             shaderType);

  std::shared_ptr<uint32_t> mVertProgPtr;
  std::shared_ptr<uint32_t> mTesselationControlProgPtr;
  std::shared_ptr<uint32_t> mTesselationEvaluationProgPtr;
  std::shared_ptr<uint32_t> mGeometryProgPtr;
  std::shared_ptr<uint32_t> mFragProgPtr;
};

} // namespace prgl

#endif // PRGL_GLSL_RENDERING_PIPELINE_PROGRAM_H
