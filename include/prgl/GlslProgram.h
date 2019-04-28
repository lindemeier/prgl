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
#include "prgl/Texture2d.h"

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

  void bind(bool use);
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
  uint32_t getCurrentlyBoundProgram() const;
  uint32_t compile(const std::string& source, uint32_t type);

  uint32_t mProgHandle;
};

class GlslRenderingPipelineShader final : public GlslProgram
{
public:
  GlslRenderingPipelineShader(const std::string& glslVertSource,
                              const std::string& glslFragSource);

  virtual ~GlslRenderingPipelineShader();

  void addSampler(const std::string& name, const std::shared_ptr<Texture2d>& m);

  void execute2D(const std::shared_ptr<Texture2d>&, int32_t x, int32_t y,
                 int32_t w, int32_t h);

  void executeCustom(const std::shared_ptr<Texture2d>& target,
                     std::function<void()>&&           func);

private:
  void attachVertexShader(const std::string& source);
  void attachFragmentShader(const std::string& source);

  std::map<std::string, std::shared_ptr<Texture2d>> mAttachments;
  std::unique_ptr<FrameBufferObject>                mFbo;
  uint32_t                                          mVertProg;
  uint32_t                                          mFragProg;
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
  void bindImage2D(uint32_t location, const std::shared_ptr<Texture2d>& texture,
                   TextureAccess access);
  // void bindSSBO(uint32_t location, std::shared_ptr<ShaderStorageBuffer>
  // &buffer);
  void bindSampler(uint32_t location, const std::string& name,
                   const std::shared_ptr<Texture2d>& sampler);

private:
  void attach(const std::string& source);

  std::array<int32_t, 3U> getWorkGroupSize();
  std::array<int32_t, 3U> getMaxWorkGroupSize() const;
  void dispatchCompute(uint32_t num_groups_x, uint32_t num_groups_y,
                       uint32_t num_groups_z) const;
  void memoryBarrier(GLbitfield barrierType = GL_ALL_BARRIER_BITS) const;

  std::map<uint32_t, std::shared_ptr<Texture2d>> mBindings;

  uint32_t mShaderHandle;
};

} // namespace prgl

#endif // PRGL_PROGRAM_H
