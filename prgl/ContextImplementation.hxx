/**
 * @file Context.hxx
 * @author Thomas Lindemeier
 * @brief
 * @date 2019-01-02
 *
 */
#ifndef PRGL_CONTEXT_IMPLEMENTATION_H
#define PRGL_CONTEXT_IMPLEMENTATION_H

#include <memory>
#include <string>

#include "prgl/glCommon.hxx"

namespace prgl {

class ContextImplementation {
  GLFWwindow* mGlfwWindow;

  static void initGLFW();

  void initGLEW(GLFWwindow* window);

 public:
  ContextImplementation(uint32_t width, uint32_t height,
                        const std::string& name, int32_t redBits,
                        int32_t greenBits, int32_t blueBits, int32_t alphaBits,
                        int32_t depthBits, int32_t stencilBits, int32_t samples,
                        bool resizable, bool visible, bool sRGB_capable,
                        GLFWmonitor* monitor, GLFWwindow* shareContext);

  ContextImplementation();

  GLFWwindow* getGLFW() const;

  virtual ~ContextImplementation();

  [[noreturn]] static void onError(int32_t errorCode, const char* errorMessage);

  void makeCurrent() const;
};

}  // namespace prgl

#endif  // PRGL_CONTEXT_IMPLEMENTATION_H
