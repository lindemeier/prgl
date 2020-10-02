/**
 * @author Thomas Lindemeier
 *
 */

#include "ContextImplementation.hxx"

#include <iostream>

namespace prgl {

void checkGLError(const char* file, const char* function, int line) {
  int32_t err(glGetError());

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

// http://blog.nobel-joergensen.com/2013/02/17/debugging-opengl-part-2-using-gldebugmessagecallback/
void APIENTRY openGlDebugCallback(GLenum source, GLenum type, GLuint id,
                                  GLenum severity, GLsizei length,
                                  const GLchar* message,
                                  const void* userParam) {
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

ContextImplementation::ContextImplementation()
    : ContextImplementation(640, 480, "", 8, 8, 8, 8, 8, 8, 4, false, false,
                            true, nullptr, nullptr) {}

ContextImplementation::~ContextImplementation() {
  if (getGLFW()) {
    glfwDestroyWindow(getGLFW());
    glfwTerminate();
  }
}

void ContextImplementation::onError(int32_t errorCode,
                                    const char* errorMessage) {
  std::cerr << "GLWindow::ERROR_GLFW:\t" << errorMessage << std::endl;
  std::cout << "press continue to exit";
  std::cin.get();
  exit(EXIT_FAILURE);
}

void ContextImplementation::initGLFW() {
  bool error = glfwInit();
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
  int32_t err = glewInit();
  if (GLEW_OK != err) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
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
  printf("Status: Using GLFW Version %s\n", glfwGetVersionString());
  printf("Status: Using OpenGL Version: %i.%i, Revision: %i\n", iOpenGLMajor,
         iOpenGLMinor, iOpenGLRevision);
  printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

GLFWwindow* ContextImplementation::getGLFW() const {
  return mGlfwWindow;
}

ContextImplementation::ContextImplementation(
  uint32_t width, uint32_t height, const std::string& name, int32_t redBits,
  int32_t greenBits, int32_t blueBits, int32_t alphaBits, int32_t depthBits,
  int32_t stencilBits, uint32_t samples, bool resizable, bool visible,
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
  glfwWindowHint(GLFW_RESIZABLE, resizable);
  glfwWindowHint(GLFW_VISIBLE, visible);
  glfwWindowHint(GLFW_SRGB_CAPABLE, sRGB_capable);
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
    glfwCreateWindow(width, height, name.c_str(), monitor, shareContext);

  if (!mGlfwWindow) {
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
  int32_t dims;
  glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &dims);
  if (width > static_cast<uint32_t>(dims) ||
      height > static_cast<uint32_t>(dims)) {
    std::cerr << "OPENGL: "
              << "maximum GL_MAX_RENDERBUFFER_SIZE size exceeded: width: "
              << width << ">" << dims << " height: " << height << ">" << dims
              << std::endl;
  }
  glViewport(0, 0, width, height);

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
