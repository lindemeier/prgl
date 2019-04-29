/**
 * @file GlslProgram.h
 * @author Thomas Lindemeier
 * @brief
 * @date 2019-01-02
 *
 */
#ifndef PRGL_PROGRAM_H
#define PRGL_PROGRAM_H

#include <memory>

#include "prgl/glCommon.h"

#define GLSL(version, shader) "#version " #version "\n" #shader

namespace prgl
{

/**
 * @brief Abstract class for shader programs.
 *
 */
class GlslProgram
{
public:
  GlslProgram();

  virtual ~GlslProgram();

  void bind(bool use) const;
  void seti(const std::string& label, int32_t arg);
  void setui(const std::string& label, uint32_t arg);
  void setf(const std::string& label, float arg);
  void set2i(const std::string& label, int32_t arg1, int32_t arg2);
  void set2f(const std::string& label, float arg1, float arg2);
  void set2f(const std::string& label, const std::array<float, 2>& v);
  void set3i(const std::string& label, int32_t arg1, int32_t arg2,
             int32_t arg3);
  void set3f(const std::string& label, float arg1, float arg2, float arg3);
  void set3f(const std::string& label, const std::array<float, 3>& v);
  void set4f(const std::string& label, const std::array<float, 4>& v);
  void set4i(const std::string& label, int32_t arg1, int32_t arg2, int32_t arg3,
             int32_t arg4);
  void set4f(const std::string& label, float arg1, float arg2, float arg3,
             float arg4);
  void set3iv(const std::string& label, const int* args);
  void set3fv(const std::string& label, const float* args);
  void set4fv(const std::string& label, const float* args);
  void setMatrix(const std::string& label, const float* m, bool transpose);
  void setMatrix(const std::string& label, const double* m, bool transpose);

  static std::string ReadShaderFromFile(const std::string& filename);

protected:
  uint32_t        getCurrentlyBoundProgram() const;
  static uint32_t compile(const std::string& source, uint32_t type);

  std::shared_ptr<uint32_t> mProgHandlePtr;
};

} // namespace prgl

#endif // PRGL_PROGRAM_H
