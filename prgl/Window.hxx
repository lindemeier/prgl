/**
 * @file Window.hxx
 * @author Thomas Lindemeier
 * @brief
 * @date 2019-01-02
 *
 */
#ifndef PRGL_WINDOW_H
#define PRGL_WINDOW_H

#include <array>
#include <functional>
#include <memory>

struct GLFWwindow;

namespace prgl {
class ContextImplementation;

class Window {
  std::unique_ptr<ContextImplementation> mContext;

  std::function<void()> mRenderFunction;
  std::function<void(int32_t, int32_t, int32_t, int32_t)> mOnKeyFunction;
  std::function<void(int32_t, int32_t, int32_t)> mOnMouseFunction;
  std::function<void(double, double)> mOnMouseMoveFunction;
  std::function<void(double, double)> mOnScrollFunction;
  std::function<void(int32_t, int32_t)> mOnResizeFunction;

 public:
  Window(uint32_t width, uint32_t height, const std::string& title,
         bool resizable);

  /*uint32_t width, uint32_t height, const std::string &name,
          int32_t redBits, int32_t greenBits, int32_t blueBits,
          int32_t alphaBits, int32_t depthBits, int32_t stencilBits,
          uint32_t samples, bool resizable, bool visible, bool sRGB_capable,
          GLFWmonitor *monitor, GLFWwindow *shareContext*/
  Window(uint32_t width, uint32_t height, const std::string& title,
         const std::array<int32_t, 4>& rgbaBits, int32_t depthBits,
         int32_t stencilBits, int32_t samples, bool resizable);

  virtual ~Window();

 private:
  static void glfw_onKey(GLFWwindow* window, int32_t key, int32_t scancode,
                         int32_t action, int32_t mods);
  static void glfw_onMouse(GLFWwindow* window, int32_t button, int32_t action,
                           int32_t mods);
  static void glfw_onMouseMove(GLFWwindow* window, double x, double y);
  static void glfw_onScroll(GLFWwindow* window, double xo, double yo);
  static void glfw_onResize(GLFWwindow* window, int32_t width, int32_t height);

  void internalOnKey(int32_t key, int32_t scancode, int32_t action,
                     int32_t mods);
  void internalOnMouse(int32_t button, int32_t action, int32_t mods);
  void internalOnMouseMove(double x, double y);
  void internalOnScroll(double xo, double yo);
  void internalOnResize(int32_t width, int32_t height);

  void waitEvents() const;
  void swapBuffers() const;

 public:
  // only poll and process events from the OS
  void pollEvents() const;
  // swap buffers, processing events
  void update(bool waitForEvents = false);

  // setting a custom onKey function
  void setOnKeyFunction(
    const std::function<void(int32_t, int32_t, int32_t, int32_t)>& onKey);
  // setting a custom onMouse function
  void setOnMouseFunction(
    const std::function<void(int32_t, int32_t, int32_t)>& onMouse);
  // setting a custom onMouseMove function
  void setOnMouseMoveFunction(
    const std::function<void(double, double)>& onMouseMove);
  // setting a custom onScroll function
  void setOnScrollFunction(const std::function<void(double, double)>& onScroll);
  // setting a custom onResize function
  void setOnResizeFunction(
    const std::function<void(int32_t, int32_t)>& onResize);
  // setting the render function to be called by renderOnce
  void setRenderFunction(const std::function<void()>& renderStep);

  // single render step of the given render function and call to update
  void renderOnce(bool waitForEvents = false);
  // loops renderOnce and updates
  int32_t renderLoop(bool waitForEvents = true);

  void setVisible(bool show);
  void close();
  void resize(uint32_t width, uint32_t height);

  void setTitle(const std::string& title);

  int32_t getWidth() const;
  int32_t getHeight() const;

  void getSize(int32_t& width, int32_t& height) const;

  std::array<double, 2> getCursorPos() const;

  int32_t getMouseButtonState(int32_t button) const;

  bool shouldClose();
};
}  // namespace prgl

#endif  // PRGL_WINDOW_H
