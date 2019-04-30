#include "prgl/GlslProgram.h"

#include <fstream>
#include <iostream>
#include <memory>
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

GlslProgram::GlslProgram() : mProgHandlePtr(nullptr)
{
  mProgHandlePtr  = std::shared_ptr<uint32_t>(new uint32_t, [](uint32_t* ptr) {
    glDeleteProgram(*ptr);
    *ptr = INVALID_HANDLE;
    delete ptr;
    ptr = nullptr;
  });
  *mProgHandlePtr = glCreateProgram();
}

GlslProgram::~GlslProgram() {}

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
  glUniform1i(glGetUniformLocation(*mProgHandlePtr, label.c_str()), arg);
}

void GlslProgram::setui(const std::string& label, uint32_t arg)
{
  glUniform1ui(glGetUniformLocation(*mProgHandlePtr, label.c_str()), arg);
}

void GlslProgram::setf(const std::string& label, float arg)
{
  glUniform1f(glGetUniformLocation(*mProgHandlePtr, label.c_str()), arg);
}

void GlslProgram::set2i(const std::string& label, int32_t arg1, int32_t arg2)
{
  glUniform2i(glGetUniformLocation(*mProgHandlePtr, label.c_str()), arg1, arg2);
}

void GlslProgram::set2f(const std::string& label, float arg1, float arg2)
{
  glUniform2f(glGetUniformLocation(*mProgHandlePtr, label.c_str()), arg1, arg2);
}

void GlslProgram::set2f(const std::string& label, const std::array<float, 2>& v)
{
  glUniform2f(glGetUniformLocation(*mProgHandlePtr, label.c_str()), v[0], v[1]);
}

void GlslProgram::set3i(const std::string& label, int32_t arg1, int32_t arg2,
                        int32_t arg3)
{
  glUniform3i(glGetUniformLocation(*mProgHandlePtr, label.c_str()), arg1, arg2,
              arg3);
}

void GlslProgram::set3f(const std::string& label, float arg1, float arg2,
                        float arg3)
{
  glUniform3f(glGetUniformLocation(*mProgHandlePtr, label.c_str()), arg1, arg2,
              arg3);
}

void GlslProgram::set3f(const std::string& label, const std::array<float, 3>& v)
{
  glUniform3f(glGetUniformLocation(*mProgHandlePtr, label.c_str()), v[0], v[1],
              v[2]);
}

void GlslProgram::set4f(const std::string& label, const std::array<float, 4>& v)
{
  glUniform4f(glGetUniformLocation(*mProgHandlePtr, label.c_str()), v[0], v[1],
              v[2], v[3]);
}

void GlslProgram::set4i(const std::string& label, int32_t arg1, int32_t arg2,
                        int32_t arg3, int32_t arg4)
{
  glUniform4i(glGetUniformLocation(*mProgHandlePtr, label.c_str()), arg1, arg2,
              arg3, arg4);
}

void GlslProgram::set4f(const std::string& label, float arg1, float arg2,
                        float arg3, float arg4)
{
  glUniform4f(glGetUniformLocation(*mProgHandlePtr, label.c_str()), arg1, arg2,
              arg3, arg4);
}

void GlslProgram::set3iv(const std::string& label, const int* args)
{
  glUniform3iv(glGetUniformLocation(*mProgHandlePtr, label.c_str()), 1, args);
}

void GlslProgram::set3fv(const std::string& label, const float* args)
{
  glUniform3fv(glGetUniformLocation(*mProgHandlePtr, label.c_str()), 1, args);
}

void GlslProgram::set4fv(const std::string& label, const float* args)
{
  glUniform4fv(glGetUniformLocation(*mProgHandlePtr, label.c_str()), 1, args);
}

void GlslProgram::setMatrix(const std::string& label, const float* m,
                            bool transpose)
{
  glUniformMatrix4fv(glGetUniformLocation(*mProgHandlePtr, label.c_str()), 1,
                     transpose, m);
}

void GlslProgram::setMatrix(const std::string& label, const double* m,
                            bool transpose)
{
  glUniformMatrix4dv(glGetUniformLocation(*mProgHandlePtr, label.c_str()), 1,
                     transpose, m);
}

void GlslProgram::bind(bool use) const
{
  if (use)
    {
      if (getCurrentlyBoundProgram() != *mProgHandlePtr)
        {
          glUseProgram(*mProgHandlePtr);
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

/**
 * @brief Bind texture to shader
 *
 * @param name Label in the shader
 * @param unit texture unit
 * @param texture the texture
 */
void GlslProgram::bindSampler(const std::string& name, const TextureUnit unit,
                              const Texture2d& texture)
{
  bind(true);
  glActiveTexture(static_cast<uint32_t>(unit));
  texture.bind(true);
  seti(name.c_str(), static_cast<uint32_t>(unit));
  bind(false);
}

} // namespace prgl
