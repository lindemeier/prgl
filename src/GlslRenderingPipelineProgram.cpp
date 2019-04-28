#include "prgl/GlslRenderingPipelineProgram.h"

#include <iostream>
#include <sstream>

namespace prgl
{

GlslRenderingPipelineProgram::GlslRenderingPipelineProgram()
  : GlslProgram(), mAttachments(), mVertProg(INVALID_HANDLE),
    mTesselationControlProg(INVALID_HANDLE),
    mTesselationEvaluationProg(INVALID_HANDLE), mGeometryProg(INVALID_HANDLE),
    mFragProg(INVALID_HANDLE)
{
}

GlslRenderingPipelineProgram::~GlslRenderingPipelineProgram()
{
  if (mProgHandle > INVALID_HANDLE)
    {
      if (mVertProg > INVALID_HANDLE)
        {
          glDetachShader(mProgHandle, mVertProg);
          glDeleteShader(mVertProg);
          mVertProg = INVALID_HANDLE;
        }
      if (mTesselationControlProg > INVALID_HANDLE)
        {
          glDetachShader(mProgHandle, mTesselationControlProg);
          glDeleteShader(mTesselationControlProg);
          mTesselationControlProg = INVALID_HANDLE;
        }
      if (mTesselationEvaluationProg > INVALID_HANDLE)
        {
          glDetachShader(mProgHandle, mTesselationEvaluationProg);
          glDeleteShader(mTesselationEvaluationProg);
          mTesselationEvaluationProg = INVALID_HANDLE;
        }
      if (mGeometryProg > INVALID_HANDLE)
        {
          glDetachShader(mProgHandle, mGeometryProg);
          glDeleteShader(mGeometryProg);
          mGeometryProg = INVALID_HANDLE;
        }

      if (mFragProg > INVALID_HANDLE)
        {
          glDetachShader(mProgHandle, mFragProg);
          glDeleteShader(mFragProg);
          mFragProg = INVALID_HANDLE;
        }
    }
}

void GlslRenderingPipelineProgram::attachVertexShader(const std::string& source)
{

  if (!source.empty())
    {
      if (mProgHandle > INVALID_HANDLE)
        {
          if (mVertProg > INVALID_HANDLE)
            {
              glDetachShader(mProgHandle, mVertProg);
              glDeleteShader(mVertProg);
              mVertProg = INVALID_HANDLE;
            }
        }

      mVertProg = compile(source.c_str(), GL_VERTEX_SHADER);

      if (mVertProg > INVALID_HANDLE)
        {
          glAttachShader(mProgHandle, mVertProg);
          glLinkProgram(mProgHandle);
        }
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
      if (mProgHandle > INVALID_HANDLE)
        {
          if (mTesselationControlProg > INVALID_HANDLE)
            {
              glDetachShader(mProgHandle, mTesselationControlProg);
              glDeleteShader(mTesselationControlProg);
              mTesselationControlProg = INVALID_HANDLE;
            }
        }

      mTesselationControlProg = compile(source.c_str(), GL_TESS_CONTROL_SHADER);

      if (mTesselationControlProg > INVALID_HANDLE)
        {
          glAttachShader(mProgHandle, mTesselationControlProg);
          glLinkProgram(mProgHandle);
        }
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
      if (mProgHandle > INVALID_HANDLE)
        {
          if (mTesselationEvaluationProg > INVALID_HANDLE)
            {
              glDetachShader(mProgHandle, mTesselationEvaluationProg);
              glDeleteShader(mTesselationEvaluationProg);
              mTesselationEvaluationProg = INVALID_HANDLE;
            }
        }

      mTesselationEvaluationProg =
        compile(source.c_str(), GL_TESS_EVALUATION_SHADER);

      if (mTesselationEvaluationProg > INVALID_HANDLE)
        {
          glAttachShader(mProgHandle, mTesselationEvaluationProg);
          glLinkProgram(mProgHandle);
        }
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
      if (mProgHandle > INVALID_HANDLE)
        {
          if (mGeometryProg > INVALID_HANDLE)
            {
              glDetachShader(mProgHandle, mGeometryProg);
              glDeleteShader(mGeometryProg);
              mGeometryProg = INVALID_HANDLE;
            }
        }

      mGeometryProg = compile(source.c_str(), GL_GEOMETRY_SHADER);

      if (mGeometryProg > INVALID_HANDLE)
        {
          glAttachShader(mProgHandle, mGeometryProg);
          glLinkProgram(mProgHandle);
        }
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
      if (mProgHandle > INVALID_HANDLE)
        {
          if (mFragProg > INVALID_HANDLE)
            {
              glDetachShader(mProgHandle, mFragProg);
              glDeleteShader(mFragProg);
              mFragProg = INVALID_HANDLE;
            }
        }

      mFragProg = compile(source.c_str(), GL_FRAGMENT_SHADER);

      if (mFragProg > INVALID_HANDLE)
        {
          glAttachShader(mProgHandle, mFragProg);
          glLinkProgram(mProgHandle);
        }
    }
  else
    {
      std::stringstream ss;
      ss << "FragmentShader() : source empty: " << source << std::endl;
      throw std::runtime_error(ss.str());
    }
}

void GlslRenderingPipelineProgram::addSampler(
  const std::string& name, const std::shared_ptr<Texture2d>& input)
{
  mAttachments[name] = input;
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
