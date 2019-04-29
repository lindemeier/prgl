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

/**
 * @brief Bind texture to shader
 *
 * @param name Label in the shader
 * @param unit texture unit (GL_TEXTURE0+x)
 * @param texturethe texture
 */
void GlslRenderingPipelineProgram::bindSampler(
  const std::string& name, const TextureUnit unit,
  const std::shared_ptr<Texture2d>& texture)
{
  glActiveTexture(static_cast<uint32_t>(unit));
  texture->bind(true);
  seti(name.c_str(), static_cast<uint32_t>(unit));
}

void GlslRenderingPipelineProgram::execute2D(
  const std::shared_ptr<Texture2d>& target, int32_t x, int32_t y, int32_t w,
  int32_t h)
{

  // mFbo->attachTexture(target);
  // mFbo->bind(true);

  // bind(true);

  // glPushAttrib(GL_ALL_ATTRIB_BITS);
  // glClampColor(GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE);
  // glClampColor(GL_CLAMP_READ_COLOR_ARB, GL_FALSE);
  // glClampColor(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE);
  // glDisable(GL_FRAMEBUFFER_SRGB);
  // glDisable(GL_DEPTH_TEST);
  // glDisable(GL_LIGHTING);

  // const uint32_t width  = mFbo->getTarget()->getWidth();
  // const uint32_t height = mFbo->getTarget()->getHeight();
  // glViewport(0, 0, width, height);

  // glMatrixMode(GL_PROJECTION);
  // glPushMatrix();
  // glLoadIdentity();
  // glMatrixMode(GL_MODELVIEW);
  // glPushMatrix();
  // glLoadIdentity();

  // // bind samplers
  // int32_t unit = 0;
  // for (auto& p : mAttachments)
  //   { // max 4 samplers
  //     glActiveTexture(GL_TEXTURE0 + unit);
  //     p.second->bind(true);
  //     seti(p.first.data(), unit++);
  //   }

  // if (w <= 0 || h <= 0)
  //   {
  //     glRectf(-1.f, -1.f, 1.f, 1.f);
  //   }
  // else
  //   {
  //     glRectf((x / width) * 2.f - 1.f, (y / height) * 2.f - 1.f,
  //             ((x + w) / width) * 2.f - 1.f, ((y + h) / width) * 2.f - 1.f);
  //   }
  // glFlush();

  // glPopMatrix(); // modelview
  // glMatrixMode(GL_PROJECTION);
  // glPopMatrix();
  // glPopAttrib();

  // bind(false);

  // mFbo->bind(false);
}

} // namespace prgl
