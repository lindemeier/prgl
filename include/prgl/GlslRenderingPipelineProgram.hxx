/**
 * @file GlslRenderingPipelineProgram.hxx
 * @author Thomas Lindemeier
 * @brief
 * @date 2019-01-02
 *
 */
#ifndef PRGL_GLSL_RENDERING_PIPELINE_PROGRAM_H
#define PRGL_GLSL_RENDERING_PIPELINE_PROGRAM_H

#include <array>
#include <memory>

#include "prgl/GlslProgram.hxx"
#include "prgl/ShaderStorageBuffer.hxx"
#include "prgl/Texture2d.hxx"

namespace prgl {

/**
 * @brief Program for vertex, tesselation, geometry and fragment shaders.
 *
 */
class GlslRenderingPipelineProgram final : public GlslProgram {
 public:
  static std::shared_ptr<GlslRenderingPipelineProgram> Create();
  GlslRenderingPipelineProgram();
  ~GlslRenderingPipelineProgram() override;

  void attachVertexShader(const std::string& source);
  void attachTesselationControlShader(const std::string& source);
  void attachTesselationEvaluationShader(const std::string& source);
  void attachGeometryShader(const std::string& source);
  void attachFragmentShader(const std::string& source);

 private:
  GlslRenderingPipelineProgram(const GlslRenderingPipelineProgram&) = delete;
  GlslRenderingPipelineProgram& operator=(const GlslRenderingPipelineProgram&) =
    delete;

  uint32_t createShader(const std::string& source, GLenum shaderType);
  void cleanupShader(uint32_t& shader);

  uint32_t mVertProg;
  uint32_t mTesselationControlProg;
  uint32_t mTesselationEvaluationProg;
  uint32_t mGeometryProg;
  uint32_t mFragProg;
};

}  // namespace prgl

#endif  // PRGL_GLSL_RENDERING_PIPELINE_PROGRAM_H
