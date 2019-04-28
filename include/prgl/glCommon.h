#ifndef PRGL_OPENGL_HEADER
#define PRGL_OPENGL_HEADER

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>

namespace prgl
{
/**
 * @brief Shader, texture handles that are not yet initl
 */
constexpr uint32_t INVALID_HANDLE = 0U;

/**
 * @brief Check for OpenGL errors: checkGLError(__FILE__, __FUNCTION__,
 * __LINE__);
 *
 * @param file the file where the function gets called.
 * @param function the function where the function gets called.
 * @param line the line where the function gets called.
 */
void checkGLError(const char* file, const char* function, int line);

using color3f = std::array<float, 3U>;
using color4f = std::array<float, 4U>;

} //  namespace prgl

#endif // !PRGL_OPENGL_HEADER
