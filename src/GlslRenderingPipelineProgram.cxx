#include "prgl/GlslRenderingPipelineProgram.hxx"

#include <iostream>
#include <sstream>

namespace prgl {

std::shared_ptr<GlslRenderingPipelineProgram>
GlslRenderingPipelineProgram::Create() {
  return std::make_shared<GlslRenderingPipelineProgram>();
}

GlslRenderingPipelineProgram::GlslRenderingPipelineProgram()
    : mVertProg(INVALID_HANDLE),
      mTesselationControlProg(INVALID_HANDLE),
      mTesselationEvaluationProg(INVALID_HANDLE),
      mGeometryProg(INVALID_HANDLE),
      mFragProg(INVALID_HANDLE) {}

uint32_t GlslRenderingPipelineProgram::createShader(const std::string& source,
                                                    GLenum shaderType) {
  uint32_t shader = compile(source, shaderType);

  glAttachShader(mProgHandle, shader);

  glLinkProgram(mProgHandle);
  int32_t linkV = 0;
  glGetProgramiv(mProgHandle, GL_LINK_STATUS, &linkV);

  if (linkV == 0) {
    std::cerr << "Error in linking GlslRenderingPipelineProgram program"
              << std::endl;
    GLchar log[10240];
    GLsizei length = 0;
    glGetProgramInfoLog(mProgHandle, 10239, &length, log);
    std::cerr << source << "\n" << log << std::endl;

    std::stringstream ss;
    ss << "GlslRenderingPipelineProgram() : linking error with source : "
       << source << "\nerror:\n"
       << log << std::endl;
    throw std::runtime_error(ss.str());
  }

  return shader;
}

void GlslRenderingPipelineProgram::cleanupShader(uint32_t& shader) {
  if (mProgHandle > INVALID_HANDLE) {
    if (shader > INVALID_HANDLE) {
      glDetachShader(mProgHandle, shader);
    }
  }
  if (shader > INVALID_HANDLE) {
    glDeleteShader(shader);
    shader = INVALID_HANDLE;
  }
}

GlslRenderingPipelineProgram::~GlslRenderingPipelineProgram() {
  cleanupShader(mVertProg);
  cleanupShader(mTesselationControlProg);
  cleanupShader(mTesselationEvaluationProg);
  cleanupShader(mGeometryProg);
  cleanupShader(mFragProg);
}

void GlslRenderingPipelineProgram::attachVertexShader(
  const std::string& source) {
  if (!source.empty()) {
    cleanupShader(mVertProg);

    mVertProg = createShader(source, GL_VERTEX_SHADER);
  } else {
    std::stringstream ss;
    ss << "VertexShader() : source empty: " << source << std::endl;
    throw std::runtime_error(ss.str());
  }
}

void GlslRenderingPipelineProgram::attachTesselationControlShader(
  const std::string& source) {
  if (!source.empty()) {
    cleanupShader(mTesselationControlProg);

    mTesselationControlProg = createShader(source, GL_TESS_CONTROL_SHADER);
  } else {
    std::stringstream ss;
    ss << "TesselationControlShader() : source empty: " << source << std::endl;
    throw std::runtime_error(ss.str());
  }
}

void GlslRenderingPipelineProgram::attachTesselationEvaluationShader(
  const std::string& source) {
  if (!source.empty()) {
    cleanupShader(mTesselationEvaluationProg);

    mTesselationEvaluationProg =
      createShader(source, GL_TESS_EVALUATION_SHADER);
  } else {
    std::stringstream ss;
    ss << "TesselationEvaluationShader() : source empty: " << source
       << std::endl;
    throw std::runtime_error(ss.str());
  }
}

void GlslRenderingPipelineProgram::attachGeometryShader(
  const std::string& source) {
  if (!source.empty()) {
    cleanupShader(mGeometryProg);

    mGeometryProg = createShader(source, GL_GEOMETRY_SHADER);
  } else {
    std::stringstream ss;
    ss << "GeometryShader() : source empty: " << source << std::endl;
    throw std::runtime_error(ss.str());
  }
}

void GlslRenderingPipelineProgram::attachFragmentShader(
  const std::string& source) {
  if (!source.empty()) {
    cleanupShader(mFragProg);

    mFragProg = createShader(source, GL_FRAGMENT_SHADER);
  } else {
    std::stringstream ss;
    ss << "FragmentShader() : source empty: " << source << std::endl;
    throw std::runtime_error(ss.str());
  }
}

}  // namespace prgl
