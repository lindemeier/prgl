/**
 * @file GlslProgram.h
 * @author Thomas Lindemeier
 * @brief
 * @date 2019-01-02
 *
 */
#ifndef PRGL_PROGRAM_H
#define PRGL_PROGRAM_H

#include "prgl/FrameBufferObject.h"
#include "prgl/ShaderStorageBuffer.h"
#include "prgl/Texture2D.h"

#include <array>
#include <functional>
#include <map>

#define GLSL(version, shader) "#version " #version "\n" #shader

namespace prgl
{

class GlslProgram
{
public:
  GlslProgram();

  virtual ~GlslProgram();

  void use(bool use);
  void seti(const GLchar* label, int32_t arg);
  void setui(const GLchar* label, uint32_t arg);
  void setf(const GLchar* label, float arg);
  void set2i(const GLchar* label, int32_t arg1, int32_t arg2);
  void set2f(const GLchar* label, float arg1, float arg2);
  void set2f(const GLchar* label, const std::array<float, 2>& v);
  void set3i(const GLchar* label, int32_t arg1, int32_t arg2, int32_t arg3);
  void set3f(const GLchar* label, float arg1, float arg2, float arg3);
  void set3f(const GLchar* label, const std::array<float, 3>& v);
  void set4f(const GLchar* label, const std::array<float, 4>& v);
  void set4i(const GLchar* label, int32_t arg1, int32_t arg2, int32_t arg3,
             int32_t arg4);
  void set4f(const GLchar* label, float arg1, float arg2, float arg3,
             float arg4);
  void set3iv(const GLchar* label, const int* args);
  void set3fv(const GLchar* label, const float* args);
  void set4fv(const GLchar* label, const float* args);
  void setMatrix(const GLchar* label, const float* m, bool transpose);
  void setMatrix(const GLchar* label, const double* m, bool transpose);

  static std::string ReadShaderFromFile(const std::string& filename);

protected:
  GLuint getCurrentlyBoundProgram() const;
  GLuint compile(const GLchar* source, GLuint type);

  GLuint mProgHandle;
};

class GlslVertFrag final : public GlslProgram
{
public:
  GlslVertFrag(const std::string& glslVertSource,
               const std::string& glslFragSource);

  virtual ~GlslVertFrag();

  void addSampler(const std::string& name, const std::shared_ptr<Texture2D>& m);

  void execute2D(const std::shared_ptr<Texture2D>&, int32_t x, int32_t y,
                 int32_t w, int32_t h);

  void executeCustom(const std::shared_ptr<Texture2D>& target,
                     std::function<void()>&&           func);

private:
  void attachVertexShader(const std::string& source);
  void attachFragmentShader(const std::string& source);

  std::map<std::string, std::shared_ptr<Texture2D>> mAttachments;
  std::unique_ptr<FrameBufferObject>                mFbo;
  GLuint                                            mVertProg;
  GLuint                                            mFragProg;
};

class GlslCompute final : public GlslProgram
{
public:
  GlslCompute(const std::string& glslSource);

  virtual ~GlslCompute();

  void execute(int32_t x, int32_t y, int32_t w, int32_t h);

  /*
   * GL_READ_ONLY, GL_WRITE_ONLY, GL_READ_WRITE
   */
  void bindImage2D(GLuint location, const std::shared_ptr<Texture2D>& texture,
                   GLenum access);
  // void bindSSBO(GLuint location, std::shared_ptr<SSBO> &buffer);
  void bindSampler(GLuint location, const std::string& name,
                   const std::shared_ptr<Texture2D>& sampler);

private:
  void attach(const std::string& source);

  std::array<int32_t, 3> getWorkGroupSize();
  std::array<int32_t, 3> getMaxWorkGroupSize() const;
  void dispatchCompute(GLuint num_groups_x, GLuint num_groups_y,
                       GLuint num_groups_z) const;
  void memoryBarrier(GLbitfield barrierType = GL_ALL_BARRIER_BITS) const;

  std::map<GLuint, std::shared_ptr<Texture2D>> mBindings;

  GLuint mShaderHandle;
};

} // namespace prgl

#endif // PRGL_PROGRAM_H
