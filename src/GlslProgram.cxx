#include "prgl/GlslProgram.hxx"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

namespace prgl {

std::string GlslProgram::ReadShaderFromFile(const std::string& filename) {
  std::string content;
  std::ifstream fileStream(filename, std::ios::in);

  if (!fileStream.is_open()) {
    throw std::runtime_error("Could not read shader file " + filename);
  }

  std::string line;
  while (!fileStream.eof()) {
    std::getline(fileStream, line);
    content.append(line + "\n");
  }

  fileStream.close();
  return content;
}

std::shared_ptr<GlslProgram> GlslProgram::Create() {
  return std::make_shared<GlslProgram>();
}

GlslProgram::GlslProgram() : mProgHandle(INVALID_HANDLE) {
  mProgHandle = glCreateProgram();
}

GlslProgram::~GlslProgram() {
  glDeleteProgram(mProgHandle);
  mProgHandle = INVALID_HANDLE;
}

uint32_t GlslProgram::compile(const std::string& source, uint32_t type) {
  uint32_t id = glCreateShader(type);

  const char* c_str = source.c_str();
  glShaderSource(id, 1, &c_str, nullptr);
  glCompileShader(id);

  int32_t c = 0;

  glGetShaderiv(id, GL_COMPILE_STATUS, &c);

  if (c == 0) {
    std::unique_ptr<GLchar[]> logstr(new GLchar[2048]);
    glGetShaderInfoLog(id, 2048, nullptr, logstr.get());
    std::stringstream ss;
    ss << "SHADER::Error compiling shader"
       << "\n"
       << source << "\n"
       << logstr.get() << std::endl;
    throw std::runtime_error(ss.str());
  }

  return id;
}

void GlslProgram::seti(const std::string& label, int32_t arg) {
  glUniform1i(glGetUniformLocation(mProgHandle, label.c_str()), arg);
}

void GlslProgram::setui(const std::string& label, uint32_t arg) {
  glUniform1ui(glGetUniformLocation(mProgHandle, label.c_str()), arg);
}

void GlslProgram::setf(const std::string& label, float arg) {
  glUniform1f(glGetUniformLocation(mProgHandle, label.c_str()), arg);
}

void GlslProgram::set2i(const std::string& label, int32_t arg1, int32_t arg2) {
  glUniform2i(glGetUniformLocation(mProgHandle, label.c_str()), arg1, arg2);
}

void GlslProgram::set2f(const std::string& label, float arg1, float arg2) {
  glUniform2f(glGetUniformLocation(mProgHandle, label.c_str()), arg1, arg2);
}

void GlslProgram::set2f(const std::string& label,
                        const std::array<float, 2>& v) {
  glUniform2f(glGetUniformLocation(mProgHandle, label.c_str()), v[0], v[1]);
}

void GlslProgram::set3i(const std::string& label, int32_t arg1, int32_t arg2,
                        int32_t arg3) {
  glUniform3i(glGetUniformLocation(mProgHandle, label.c_str()), arg1, arg2,
              arg3);
}

void GlslProgram::set3f(const std::string& label, float arg1, float arg2,
                        float arg3) {
  glUniform3f(glGetUniformLocation(mProgHandle, label.c_str()), arg1, arg2,
              arg3);
}

void GlslProgram::set3f(const std::string& label,
                        const std::array<float, 3>& v) {
  glUniform3f(glGetUniformLocation(mProgHandle, label.c_str()), v[0], v[1],
              v[2]);
}

void GlslProgram::set4f(const std::string& label,
                        const std::array<float, 4>& v) {
  glUniform4f(glGetUniformLocation(mProgHandle, label.c_str()), v[0], v[1],
              v[2], v[3]);
}

void GlslProgram::set4i(const std::string& label, int32_t arg1, int32_t arg2,
                        int32_t arg3, int32_t arg4) {
  glUniform4i(glGetUniformLocation(mProgHandle, label.c_str()), arg1, arg2,
              arg3, arg4);
}

void GlslProgram::set4f(const std::string& label, float arg1, float arg2,
                        float arg3, float arg4) {
  glUniform4f(glGetUniformLocation(mProgHandle, label.c_str()), arg1, arg2,
              arg3, arg4);
}

void GlslProgram::set3iv(const std::string& label, const int* args) {
  glUniform3iv(glGetUniformLocation(mProgHandle, label.c_str()), 1, args);
}

void GlslProgram::set3fv(const std::string& label, const float* args) {
  glUniform3fv(glGetUniformLocation(mProgHandle, label.c_str()), 1, args);
}

void GlslProgram::set4fv(const std::string& label, const float* args) {
  glUniform4fv(glGetUniformLocation(mProgHandle, label.c_str()), 1, args);
}

void GlslProgram::set2(const std::string& label, const vec2f& vec) {
  glUniform4fv(glGetUniformLocation(mProgHandle, label.c_str()), 1, &(vec[0U]));
}

void GlslProgram::set3(const std::string& label, const vec3f& vec) {
  glUniform4fv(glGetUniformLocation(mProgHandle, label.c_str()), 1, &(vec[0U]));
}

void GlslProgram::set2(const std::string& label, const vec2d& vec) {
  glUniform4dv(glGetUniformLocation(mProgHandle, label.c_str()), 1, &(vec[0U]));
}

void GlslProgram::set3(const std::string& label, const vec3d& vec) {
  glUniform4dv(glGetUniformLocation(mProgHandle, label.c_str()), 1, &(vec[0U]));
}

void GlslProgram::setMatrix(const std::string& label, const float* m,
                            bool transpose) {
  glUniformMatrix4fv(glGetUniformLocation(mProgHandle, label.c_str()), 1,
                     static_cast<GLboolean>(transpose), m);
}

void GlslProgram::setMatrix(const std::string& label, const double* m,
                            bool transpose) {
  glUniformMatrix4dv(glGetUniformLocation(mProgHandle, label.c_str()), 1,
                     static_cast<GLboolean>(transpose), m);
}

void GlslProgram::setMatrix(const std::string& label, const mat3x3<float>& m) {
  setMatrix(label, m.data(), true);
}

void GlslProgram::setMatrix(const std::string& label, const mat3x3<double>& m) {
  setMatrix(label, m.data(), true);
}

void GlslProgram::setMatrix(const std::string& label, const mat4x4<float>& m) {
  setMatrix(label, m.data(), true);
}

void GlslProgram::setMatrix(const std::string& label, const mat4x4<double>& m) {
  setMatrix(label, m.data(), true);
}

auto GlslProgram::isBound() const -> bool {
  return (getCurrentlyBoundProgram() == mProgHandle);
}

void GlslProgram::bind(bool use) const {
  if (use) {
    if (!isBound()) {
      glUseProgram(mProgHandle);
    }
  } else {
    glUseProgram(0);
  }
}

uint32_t GlslProgram::getCurrentlyBoundProgram() {
  int32_t id = 0;
  glGetIntegerv(GL_CURRENT_PROGRAM, &id);

  return static_cast<uint32_t>(id);
}

/**
 * @brief Bind texture to shader
 *
 * @param name Label in the shader
 * @param unit texture unit
 * @param texture the texture
 */
void GlslProgram::bindSampler(const std::string& name, const uint32_t unit,
                              const std::shared_ptr<Texture2d>& texture) {
  texture->bindUnit(unit);
  seti(name, static_cast<int32_t>(unit));
}

}  // namespace prgl
