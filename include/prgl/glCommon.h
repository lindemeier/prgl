#ifndef PRGL_OPENGL_HEADER
#define PRGL_OPENGL_HEADER

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace prgl
{
/**
 * @brief Shader, texture handles that are not yet initl
 */
constexpr uint32_t INVALID_HANDLE = 0U;

void checkGLError(const char* file, const char* function, int line);

} //  namespace prgl

#endif // !PRGL_OPENGL_HEADER
