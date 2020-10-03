#include "prgl/Window.hxx"

#include "prgl/ContextImplementation.hxx"
#include "prgl/glCommon.hxx"

namespace prgl {

Window::Window(uint32_t width, uint32_t height, const std::string& title,
               bool resizable)
    : Window(width, height, title, {8, 8, 8, 8}, 8, 8, 4, resizable) {}

/*uint32_t width, uint32_t height, const std::string &name,
int32_t redBits, int32_t greenBits, int32_t blueBits,
int32_t alphaBits, int32_t depthBits, int32_t stencilBits,
uint32_t samples, bool resizable, bool visible, bool sRGB_capable,
GLFWmonitor *monitor, GLFWwindow *shareContext*/
Window::Window(uint32_t width, uint32_t height, const std::string& title,
               const std::array<int32_t, 4>& rgbaBits, const int32_t depthBits,
               const int32_t stencilBits, const int32_t samples, bool resizable)
    : mContext(std::make_unique<ContextImplementation>(
        width, height, title, rgbaBits[0], rgbaBits[1], rgbaBits[2],
        rgbaBits[3], depthBits, stencilBits, samples, resizable, true, true,
        nullptr, nullptr)),
      mRenderFunction(nullptr),
      mOnKeyFunction(nullptr),
      mOnMouseFunction(nullptr),
      mOnMouseMoveFunction(nullptr),
      mOnScrollFunction(nullptr),
      mOnResizeFunction(nullptr) {
  mRenderFunction = [&]() {
    glClear(GL_COLOR_BUFFER_BIT);
  };

  glfwSetWindowUserPointer(mContext->getGLFW(), this);

  glfwSetKeyCallback(mContext->getGLFW(), glfw_onKey);
  glfwSetScrollCallback(mContext->getGLFW(), glfw_onScroll);
  glfwSetCursorPosCallback(mContext->getGLFW(), glfw_onMouseMove);
  glfwSetMouseButtonCallback(mContext->getGLFW(), glfw_onMouse);
  glfwSetWindowSizeCallback(mContext->getGLFW(), glfw_onResize);
}

Window::~Window() {}

void Window::glfw_onKey(GLFWwindow* window, int32_t key, int32_t scancode,
                        int32_t action, int32_t mods) {
  static_cast<Window*>(glfwGetWindowUserPointer(window))
    ->internalOnKey(key, scancode, action, mods);
}

void Window::glfw_onMouse(GLFWwindow* window, int32_t button, int32_t action,
                          int32_t mods) {
  static_cast<Window*>(glfwGetWindowUserPointer(window))
    ->internalOnMouse(button, action, mods);
}

void Window::glfw_onMouseMove(GLFWwindow* window, double x, double y) {
  static_cast<Window*>(glfwGetWindowUserPointer(window))
    ->internalOnMouseMove(x, y);
}

void Window::glfw_onScroll(GLFWwindow* window, double xo, double yo) {
  static_cast<Window*>(glfwGetWindowUserPointer(window))
    ->internalOnScroll(xo, yo);
}

void Window::glfw_onResize(GLFWwindow* window, int32_t width, int32_t height) {
  static_cast<Window*>(glfwGetWindowUserPointer(window))
    ->internalOnResize(width, height);
}

void Window::internalOnKey(int32_t key, int32_t scancode, int32_t action,
                           int32_t mods) {
  if (mOnKeyFunction)
    mOnKeyFunction(key, scancode, action, mods);
}

void Window::internalOnMouse(int32_t button, int32_t action, int32_t mods) {
  if (mOnMouseFunction)
    mOnMouseFunction(button, action, mods);
}

void Window::internalOnMouseMove(double x, double y) {
  if (mOnMouseMoveFunction)
    mOnMouseMoveFunction(x, y);
}

void Window::internalOnScroll(double xo, double yo) {
  if (mOnScrollFunction)
    mOnScrollFunction(xo, yo);
}

void Window::internalOnResize(int32_t width, int32_t height) {
  glViewport(0, 0, width, height);
  if (mOnResizeFunction)
    mOnResizeFunction(width, height);
}

void Window::update(bool waitForEvents) {
  swapBuffers();

  if (waitForEvents) {
    waitEvents();
  } else {
    pollEvents();
  }
}

void Window::setOnKeyFunction(
  const std::function<void(int32_t, int32_t, int32_t, int32_t)>& onKey) {
  mOnKeyFunction = onKey;
}

void Window::setOnMouseFunction(
  const std::function<void(int32_t, int32_t, int32_t)>& onMouse) {
  mOnMouseFunction = onMouse;
}

void Window::setOnMouseMoveFunction(
  const std::function<void(double, double)>& onMouseMove) {
  mOnMouseMoveFunction = onMouseMove;
}

void Window::setOnScrollFunction(
  const std::function<void(double, double)>& onScroll) {
  mOnScrollFunction = onScroll;
}

void Window::setOnResizeFunction(
  const std::function<void(int32_t, int32_t)>& onResize) {
  mOnResizeFunction = onResize;
}

void Window::setRenderFunction(const std::function<void()>& renderStep) {
  mRenderFunction = renderStep;
}

void Window::renderOnce(bool waitForEvents) {
  mRenderFunction();
  update(waitForEvents);
}

int32_t Window::renderLoop(bool waitForEvents) {
  while (!shouldClose()) {
    renderOnce(waitForEvents);
  }
  return EXIT_SUCCESS;
}

void Window::setVisible(bool show) {
  if (show)
    glfwShowWindow(mContext->getGLFW());
  else
    glfwHideWindow(mContext->getGLFW());
}

void Window::close() {
  glfwSetWindowShouldClose(mContext->getGLFW(), true);
  setVisible(false);
}

void Window::resize(uint32_t width, uint32_t height) {
  glfwSetWindowSize(mContext->getGLFW(), width, height);
  internalOnResize(width, height);
}

void Window::setTitle(const std::string& title) {
  glfwSetWindowTitle(mContext->getGLFW(), title.c_str());
}

void Window::pollEvents() const {
  glfwPollEvents();
}

void Window::waitEvents() const {
  glfwWaitEvents();
}

void Window::swapBuffers() const {
  glfwSwapBuffers(mContext->getGLFW());
}

bool Window::shouldClose() {
  return static_cast<bool>(glfwWindowShouldClose(mContext->getGLFW()));
}

int32_t Window::getWidth() const {
  int32_t width, height;
  glfwGetWindowSize(mContext->getGLFW(), &width, &height);
  return width;
}

int32_t Window::getHeight() const {
  int32_t width, height;
  glfwGetWindowSize(mContext->getGLFW(), &width, &height);
  return height;
}

void Window::getSize(int32_t& width, int32_t& height) const {
  glfwGetWindowSize(mContext->getGLFW(), &width, &height);
}

std::array<double, 2> Window::getCursorPos() const {
  std::array<double, 2> pos;
  glfwGetCursorPos(mContext->getGLFW(), &(pos[0]), &(pos[1]));
  return pos;
}

int32_t Window::getMouseButtonState(int32_t button) const {
  return glfwGetMouseButton(mContext->getGLFW(), button);
}

}  // namespace prgl
