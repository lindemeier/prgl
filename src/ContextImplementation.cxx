/**
 * @author Thomas Lindemeier
 *
 */

#include "prgl/ContextImplementation.hxx"

#include <iostream>
#include <sstream>

namespace prgl {

void checkGLError(const char* file, const char* function, int line) {
  auto err = glGetError();

  while (err != GL_NO_ERROR) {
    std::string error;

    switch (err) {
      case GL_INVALID_OPERATION:
        error = "GL_INVALID_OPERATION";
        break;
      case GL_INVALID_ENUM:
        error = "GL_INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        error = "GL_INVALID_VALUE";
        break;
      case GL_OUT_OF_MEMORY:
        error = "GL_OUT_OF_MEMORY";
        break;
      case GL_STACK_OVERFLOW:
        error = "GL_STACK_OVERFLOW";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        error = "GL_INVALID_FRAMEBUFFER_OPERATION";
        break;
        //  case GL_CONTEXT_LOST:  error = "GL_CONTEXT_LOST";  break;
    }

    std::cerr << "ERROR::" << error.c_str() << "\n\tfile:\t" << file
              << "\n\tfunction:\t" << function
              << ((line == -1) ? " " : "\n\tline:\t" + std::to_string(line))
              << std::endl;
    err = glGetError();
  }
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
// http://blog.nobel-joergensen.com/2013/02/17/debugging-opengl-part-2-using-gldebugmessagecallback/
static void APIENTRY openGlDebugCallback(GLenum /*unused*/, GLenum type,
                                         GLuint id, GLenum severity,
                                         GLsizei /*unused*/,
                                         const GLchar* message,
                                         const void* /*unused*/) {
  if (severity != GL_DEBUG_SEVERITY_MEDIUM &&
      severity != GL_DEBUG_SEVERITY_HIGH) {
    return;
  }

  std::cerr << "---------------------opengl-callback-start------------"
            << std::endl;
  std::cerr << "message: " << message << std::endl;
  std::cerr << "type: ";
  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      std::cerr << "ERROR";
      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      std::cerr << "DEPRECATED_BEHAVIOR";
      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      std::cerr << "UNDEFINED_BEHAVIOR";
      break;
    case GL_DEBUG_TYPE_PORTABILITY:
      std::cerr << "PORTABILITY";
      break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      std::cerr << "PERFORMANCE";
      break;
    case GL_DEBUG_TYPE_OTHER:
      std::cerr << "OTHER";
      break;
  }
  std::cerr << std::endl;

  std::cerr << "id: " << id << std::endl;
  std::cerr << "severity: ";
  switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
      std::cerr << "LOW";
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      std::cerr << "MEDIUM";
      break;
    case GL_DEBUG_SEVERITY_HIGH:
      std::cerr << "HIGH";
      break;
  }
  std::cerr << std::endl;
  std::cerr << "---------------------opengl-callback-end--------------"
            << std::endl;
  std::cerr << std::endl;
}
#pragma clang diagnostic pop

ContextImplementation::ContextImplementation()
    : ContextImplementation(640, 480, "", 8, 8, 8, 8, 8, 8, 4, false, false,
                            true, nullptr, nullptr) {}

ContextImplementation::~ContextImplementation() {
  if (getGLFW() != nullptr) {
    glfwDestroyWindow(getGLFW());
    glfwTerminate();
  }
}

void ContextImplementation::onError(int32_t /*unused*/,
                                    const char* errorMessage) {
  std::stringstream ss;
  ss << "GLWindow::ERROR_GLFW:\t" << errorMessage;
  throw std::runtime_error(ss.str());
}

void ContextImplementation::initGLFW() {
  bool error = glfwInit() != 0;
  if (!error) {
    std::cerr << "could not init GLFW:" << std::endl;
    std::cout << "press enter to exit";
    std::cin.get();
    exit(EXIT_FAILURE);
  }
  glfwSetErrorCallback(ContextImplementation::onError);
}

void ContextImplementation::initGLEW(GLFWwindow* window) {
  makeCurrent();

  // glewExperimental = GL_TRUE;
  auto err = glewInit();
  if (GLEW_OK != err) {
    std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    std::cout << "press enter to exit";
    std::cin.get();
    exit(EXIT_FAILURE);
  }
  // Print out GLFW, OpenGL version and GLEW Version:
  const auto iOpenGLMajor =
    glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
  const auto iOpenGLMinor =
    glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
  const auto iOpenGLRevision =
    glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
  std::cout << "Status: Using GLFW Version " << glfwGetVersionString()
            << std::endl;
  std::cout << "Status: Using OpenGL Version: " << iOpenGLMajor << "."
            << iOpenGLMinor << ", Revision: " << iOpenGLRevision << std::endl;
  std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION)
            << std::endl;
}

GLFWwindow* ContextImplementation::getGLFW() const {
  return mGlfwWindow;
}

ContextImplementation::ContextImplementation(
  uint32_t width, uint32_t height, const std::string& name, int32_t redBits,
  int32_t greenBits, int32_t blueBits, int32_t alphaBits, int32_t depthBits,
  int32_t stencilBits, int32_t samples, bool resizable, bool visible,
  bool sRGB_capable, GLFWmonitor* monitor, GLFWwindow* shareContext)
    : mGlfwWindow(nullptr) {
  initGLFW();

  glfwWindowHint(GLFW_RED_BITS, redBits);
  glfwWindowHint(GLFW_GREEN_BITS, greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, blueBits);
  glfwWindowHint(GLFW_ALPHA_BITS, alphaBits);
  glfwWindowHint(GLFW_STENCIL_BITS, stencilBits);
  glfwWindowHint(GLFW_DEPTH_BITS, depthBits);
  glfwWindowHint(GLFW_SAMPLES, samples);
  glfwWindowHint(GLFW_RESIZABLE, static_cast<int32_t>(resizable));
  glfwWindowHint(GLFW_VISIBLE, static_cast<int32_t>(visible));
  glfwWindowHint(GLFW_SRGB_CAPABLE, static_cast<int32_t>(sRGB_capable));
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#else
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);
#endif
  mGlfwWindow =
    glfwCreateWindow(static_cast<int32_t>(width), static_cast<int32_t>(height),
                     name.c_str(), monitor, shareContext);

  if (mGlfwWindow == nullptr) {
    glfwTerminate();
    return;
  }

  initGLEW(mGlfwWindow);

  // opengl error callback
#ifndef NDEBUG
  if (glDebugMessageCallback) {
    std::cout << "Register OpenGL debug callback " << std::endl;
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openGlDebugCallback, nullptr);
    uint32_t unusedIds = 0;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                          &unusedIds, true);
  } else {
    std::cout << "glDebugMessageCallback not available" << std::endl;
  }
#endif

  // check viewport size
  int32_t dims = 0;
  glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &dims);
  if (width > static_cast<uint32_t>(dims) ||
      height > static_cast<uint32_t>(dims)) {
    std::cerr << "OPENGL: "
              << "maximum GL_MAX_RENDERBUFFER_SIZE size exceeded: width: "
              << width << ">" << dims << " height: " << height << ">" << dims
              << std::endl;
  }
  glViewport(0, 0, static_cast<int32_t>(width), static_cast<int32_t>(height));

  // disable any sRGB conversion. Should not be needed as long as no sRGB
  // textures are used.
  glDisable(GL_FRAMEBUFFER_SRGB);
  // disable any clamping
  glClampColor(GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE);
  glClampColor(GL_CLAMP_READ_COLOR_ARB, GL_FALSE);
  glClampColor(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE);
}

void ContextImplementation::makeCurrent() const {
  GLFWwindow* current = glfwGetCurrentContext();
  if (current != mGlfwWindow) {
    glfwMakeContextCurrent(mGlfwWindow);
  }
}

}  // namespace prgl
