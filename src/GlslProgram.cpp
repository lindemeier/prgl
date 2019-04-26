#include "prgl/GlslProgram.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace prgl
{

std::string ReadShaderFromFile(const std::string& filename)
{
  std::string   content;
  std::ifstream fileStream(filename, std::ios::in);

  if (!fileStream.is_open())
    {
      throw std::runtime_error("Could not read shader file " + filename);
    }

  std::string line;
  while (!fileStream.eof())
    {
      std::getline(fileStream, line);
      content.append(line + "\n");
    }

  fileStream.close();
  return content;
}

GlslProgram::GlslProgram() : mProgHandle(0) { mProgHandle = glCreateProgram(); }

GlslProgram::~GlslProgram()
{
  if (mProgHandle)
    {
      glDeleteProgram(mProgHandle);
    }
}

uint32_t GlslProgram::compile(const std::string& source, uint32_t type)
{
  uint32_t id = glCreateShader(type);

  const char* c_str = source.c_str();
  glShaderSource(id, 1, &c_str, NULL);
  glCompileShader(id);

  int32_t c = 0;

  glGetShaderiv(id, GL_COMPILE_STATUS, &c);

  if (!c)
    {
      std::unique_ptr<GLchar[]> logstr(new GLchar[2048]);
      glGetShaderInfoLog(id, 2048, NULL, logstr.get());
      std::stringstream ss;
      ss << "SHADER::Error compiling shader"
         << "\n"
         << source << "\n"
         << logstr.get() << std::endl;
      throw std::runtime_error(ss.str());
    }

  return id;
}

void GlslProgram::seti(const std::string& label, int32_t arg)
{
  bind(true);
  glUniform1i(glGetUniformLocation(mProgHandle, label.c_str()), arg);
}

void GlslProgram::setui(const std::string& label, uint32_t arg)
{
  bind(true);
  glUniform1ui(glGetUniformLocation(mProgHandle, label.c_str()), arg);
}

void GlslProgram::setf(const std::string& label, float arg)
{
  bind(true);
  glUniform1f(glGetUniformLocation(mProgHandle, label.c_str()), arg);
}

void GlslProgram::set2i(const std::string& label, int32_t arg1, int32_t arg2)
{
  bind(true);
  glUniform2i(glGetUniformLocation(mProgHandle, label.c_str()), arg1, arg2);
}

void GlslProgram::set2f(const std::string& label, float arg1, float arg2)
{
  bind(true);
  glUniform2f(glGetUniformLocation(mProgHandle, label.c_str()), arg1, arg2);
}

void GlslProgram::set2f(const std::string& label, const std::array<float, 2>& v)
{
  bind(true);
  glUniform2f(glGetUniformLocation(mProgHandle, label.c_str()), v[0], v[1]);
}

void GlslProgram::set3i(const std::string& label, int32_t arg1, int32_t arg2,
                        int32_t arg3)
{
  bind(true);
  glUniform3i(glGetUniformLocation(mProgHandle, label.c_str()), arg1, arg2,
              arg3);
}

void GlslProgram::set3f(const std::string& label, float arg1, float arg2,
                        float arg3)
{
  bind(true);
  glUniform3f(glGetUniformLocation(mProgHandle, label.c_str()), arg1, arg2,
              arg3);
}

void GlslProgram::set3f(const std::string& label, const std::array<float, 3>& v)
{
  bind(true);
  glUniform3f(glGetUniformLocation(mProgHandle, label.c_str()), v[0], v[1],
              v[2]);
}

void GlslProgram::set4f(const std::string& label, const std::array<float, 4>& v)
{
  bind(true);
  glUniform4f(glGetUniformLocation(mProgHandle, label.c_str()), v[0], v[1],
              v[2], v[3]);
}

void GlslProgram::set4i(const std::string& label, int32_t arg1, int32_t arg2,
                        int32_t arg3, int32_t arg4)
{
  bind(true);
  glUniform4i(glGetUniformLocation(mProgHandle, label.c_str()), arg1, arg2,
              arg3, arg4);
}

void GlslProgram::set4f(const std::string& label, float arg1, float arg2,
                        float arg3, float arg4)
{
  bind(true);
  glUniform4f(glGetUniformLocation(mProgHandle, label.c_str()), arg1, arg2,
              arg3, arg4);
}

void GlslProgram::set3iv(const std::string& label, const int* args)
{
  bind(true);
  glUniform3iv(glGetUniformLocation(mProgHandle, label.c_str()), 1, args);
}

void GlslProgram::set3fv(const std::string& label, const float* args)
{
  bind(true);
  glUniform3fv(glGetUniformLocation(mProgHandle, label.c_str()), 1, args);
}

void GlslProgram::set4fv(const std::string& label, const float* args)
{
  bind(true);
  glUniform4fv(glGetUniformLocation(mProgHandle, label.c_str()), 1, args);
}

void GlslProgram::setMatrix(const std::string& label, const float* m,
                            bool transpose)
{
  bind(true);
  glUniformMatrix4fv(glGetUniformLocation(mProgHandle, label.c_str()), 1,
                     transpose, m);
}

void GlslProgram::setMatrix(const std::string& label, const double* m,
                            bool transpose)
{
  bind(true);
  glUniformMatrix4dv(glGetUniformLocation(mProgHandle, label.c_str()), 1,
                     transpose, m);
}

void GlslProgram::bind(bool use)
{
  if (use)
    {
      if (getCurrentlyBoundProgram() != mProgHandle)
        {
          glUseProgram(mProgHandle);
        }
    }
  else
    {
      glUseProgram(0);
    }
}

uint32_t GlslProgram::getCurrentlyBoundProgram() const
{
  int32_t id;
  glGetIntegerv(GL_CURRENT_PROGRAM, &id);

  return (uint32_t)id;
}

GlslVertFrag::GlslVertFrag(const std::string& glslVertSource,
                           const std::string& glslFragSource)
  : GlslProgram(), mAttachments(), mFbo(nullptr), mVertProg(0), mFragProg(0)
{
  mFbo = std::make_unique<FrameBufferObject>();

  attachVertexShader(glslVertSource);
  attachFragmentShader(glslVertSource);
}

GlslVertFrag::~GlslVertFrag()
{
  if (mVertProg)
    {
      glDetachShader(mProgHandle, mVertProg);
      glDeleteShader(mVertProg);
    }

  if (mFragProg)
    {
      glDetachShader(mProgHandle, mFragProg);
      glDeleteShader(mFragProg);
      mFragProg = 0;
    }
}

void GlslVertFrag::attachVertexShader(const std::string& source)
{
  if (!source.empty())
    {
      mVertProg = compile(source.c_str(), GL_VERTEX_SHADER);
      if (mVertProg)
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

void GlslVertFrag::attachFragmentShader(const std::string& source)
{
  if (!source.empty())
    {
      mFragProg = compile(source.c_str(), GL_FRAGMENT_SHADER);

      if (mFragProg)
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

void GlslVertFrag::addSampler(const std::string&                name,
                              const std::shared_ptr<Texture2D>& input)
{
  mAttachments[name] = input;
}

void GlslVertFrag::execute2D(const std::shared_ptr<Texture2D>& target,
                             int32_t x, int32_t y, int32_t w, int32_t h)
{
  mFbo->attachTexture(target);
  mFbo->bind(true);

  bind(true);

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glClampColor(GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE);
  glClampColor(GL_CLAMP_READ_COLOR_ARB, GL_FALSE);
  glClampColor(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE);
  glDisable(GL_FRAMEBUFFER_SRGB);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  const uint32_t width  = mFbo->getTarget()->getWidth();
  const uint32_t height = mFbo->getTarget()->getHeight();
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  // bind samplers
  int32_t unit = 0;
  for (auto& p : mAttachments)
    { // max 4 samplers
      glActiveTexture(GL_TEXTURE0 + unit);
      p.second->bind(true);
      seti(p.first.data(), unit++);
    }

  if (w <= 0 || h <= 0)
    {
      glRectf(-1.f, -1.f, 1.f, 1.f);
    }
  else
    {
      glRectf((x / width) * 2.f - 1.f, (y / height) * 2.f - 1.f,
              ((x + w) / width) * 2.f - 1.f, ((y + h) / width) * 2.f - 1.f);
    }
  glFlush();

  glPopMatrix(); // modelview
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  bind(false);

  mFbo->bind(false);
}

void GlslVertFrag::executeCustom(const std::shared_ptr<Texture2D>& target,
                                 std::function<void()>&&           func)
{
  mFbo->attachTexture(target);
  mFbo->bind(true);

  bind(true);
  func();
  bind(false);

  mFbo->bind(false);
}

GlslCompute::GlslCompute(const std::string& glslSource)
  : GlslProgram(), mShaderHandle(0)
{
  attach(glslSource);
}

GlslCompute::~GlslCompute()
{
  if (mProgHandle)
    {
      glDetachShader(mProgHandle, mShaderHandle);
      glDeleteShader(mShaderHandle);
    }
}

void GlslCompute::attach(const std::string& source)
{
  if (!source.empty())
    {
      mShaderHandle = compile(source.c_str(), GL_COMPUTE_SHADER);
      glAttachShader(mProgHandle, mShaderHandle);

      glLinkProgram(mProgHandle);
      int32_t linkV;
      glGetProgramiv(mProgHandle, GL_LINK_STATUS, &linkV);

      if (!linkV)
        {
          std::cerr << "Error in linking compute shader program" << std::endl;
          GLchar  log[10240];
          GLsizei length;
          glGetProgramInfoLog(mProgHandle, 10239, &length, log);
          std::cerr << source << "\n" << log << std::endl;

          std::stringstream ss;
          ss << "ComputeShader() : linking error with source : " << source
             << "\nerror:\n"
             << log << std::endl;
          throw std::runtime_error(ss.str());
        }
    }
  else
    {
      std::stringstream ss;
      ss << "ComputeShader() : empty : " << source << std::endl;
      throw std::runtime_error(ss.str());
    }
}

std::array<int32_t, 3> GlslCompute::getWorkGroupSize()
{
  std::array<int32_t, 3> size;
  glGetProgramiv(mProgHandle, GL_COMPUTE_WORK_GROUP_SIZE, &(size[0]));
  return size;
}

std::array<int32_t, 3> GlslCompute::getMaxWorkGroupSize() const
{
  std::array<int32_t, 3> size;
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &(size[0]));
  return size;
}

void GlslCompute::dispatchCompute(uint32_t num_groups_x, uint32_t num_groups_y,
                                  uint32_t num_groups_z) const
{
  glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}

void GlslCompute::memoryBarrier(GLbitfield barrierType) const
{
  glMemoryBarrier(barrierType);
}

void GlslCompute::execute(int32_t x, int32_t y, int32_t w, int32_t h)
{
  bind(true);

  set2i("offset", x, y);

  const auto workSize = getWorkGroupSize();
  dispatchCompute(w / workSize[0] + 1U, h / workSize[1] + 1U, 1U);
  memoryBarrier(GL_ALL_BARRIER_BITS);

  bind(false);
}

/**
 * uniform readonly image2D fromTex;
 */
void GlslCompute::bindImage2D(uint32_t                          location,
                              const std::shared_ptr<Texture2D>& texture,
                              int32_t                           access)
{
  mBindings[location] = texture;
  bind(true);
  texture->bindImageTexture(location, access);
  bind(false);
}

// void GlslCompute::bindSSBO(uint32_t location, std::shared_ptr<SSBO> &buffer)
//{
//  mBindings[location] = buffer;
//  bind(true);
//  buffer->bindBase(location);
//  bind(false);
//}

void GlslCompute::bindSampler(uint32_t location, const std::string& name,
                              const std::shared_ptr<Texture2D>& sampler)
{
  mBindings[location] = sampler;
  bind(true);
  glActiveTexture(GL_TEXTURE0 + location);
  sampler->bind(true);
  seti(name.data(), location);
  bind(false);
}

} // namespace prgl
