#include "prgl/GlslRenderingPipelineProgram.h"

#include <iostream>
#include <sstream>

namespace prgl
{

GlslRenderingPipelineProgram::GlslRenderingPipelineProgram()
  : GlslProgram(), mVertProgPtr(nullptr), mTesselationControlProgPtr(nullptr),
    mTesselationEvaluationProgPtr(nullptr), mGeometryProgPtr(nullptr),
    mFragProgPtr(nullptr)
{
}

std::shared_ptr<uint32_t>
GlslRenderingPipelineProgram::create(const std::string& source,
                                     GLenum             shaderType)
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
  *shaderPtr = compile(source.c_str(), shaderType);

  glAttachShader(*mProgHandlePtr, *shaderPtr);

  glLinkProgram(*mProgHandlePtr);
  int32_t linkV;
  glGetProgramiv(*mProgHandlePtr, GL_LINK_STATUS, &linkV);

  if (!linkV)
    {
      std::cerr << "Error in linking GlslRenderingPipelineProgram program"
                << std::endl;
      GLchar  log[10240];
      GLsizei length;
      glGetProgramInfoLog(*mProgHandlePtr, 10239, &length, log);
      std::cerr << source << "\n" << log << std::endl;

      std::stringstream ss;
      ss << "GlslRenderingPipelineProgram() : linking error with source : "
         << source << "\nerror:\n"
         << log << std::endl;
      throw std::runtime_error(ss.str());
    }

  return shaderPtr;
}

GlslRenderingPipelineProgram::~GlslRenderingPipelineProgram() {}

void GlslRenderingPipelineProgram::attachVertexShader(const std::string& source)
{

  if (!source.empty())
    {

      mVertProgPtr = create(source.c_str(), GL_VERTEX_SHADER);
    }
  else
    {
      std::stringstream ss;
      ss << "VertexShader() : source empty: " << source << std::endl;
      throw std::runtime_error(ss.str());
    }
}

void GlslRenderingPipelineProgram::attachTesselationControlShader(
  const std::string& source)
{
  if (!source.empty())
    {
      mTesselationControlProgPtr =
        create(source.c_str(), GL_TESS_CONTROL_SHADER);
    }
  else
    {
      std::stringstream ss;
      ss << "TesselationControlShader() : source empty: " << source
         << std::endl;
      throw std::runtime_error(ss.str());
    }
}

void GlslRenderingPipelineProgram::attachTesselationEvaluationShader(
  const std::string& source)
{
  if (!source.empty())
    {

      mTesselationEvaluationProgPtr =
        create(source.c_str(), GL_TESS_EVALUATION_SHADER);
    }
  else
    {
      std::stringstream ss;
      ss << "TesselationEvaluationShader() : source empty: " << source
         << std::endl;
      throw std::runtime_error(ss.str());
    }
}

void GlslRenderingPipelineProgram::attachGeometryShader(
  const std::string& source)
{
  if (!source.empty())
    {
      mGeometryProgPtr = create(source.c_str(), GL_GEOMETRY_SHADER);
    }
  else
    {
      std::stringstream ss;
      ss << "GeometryShader() : source empty: " << source << std::endl;
      throw std::runtime_error(ss.str());
    }
}

void GlslRenderingPipelineProgram::attachFragmentShader(
  const std::string& source)
{
  if (!source.empty())
    {
      mFragProgPtr = create(source.c_str(), GL_FRAGMENT_SHADER);
    }
  else
    {
      std::stringstream ss;
      ss << "FragmentShader() : source empty: " << source << std::endl;
      throw std::runtime_error(ss.str());
    }
}

} // namespace prgl
