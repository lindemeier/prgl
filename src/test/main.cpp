#include "prgl/GlslProgram.h"
#include "prgl/Window.h"

#include <random>

int32_t main(int32_t argc, char **args) {
  const std::array<int32_t, 4> rgbaBits = {16, 16, 16, 16};
  auto gl = std::make_unique<prgl::Window>(1024, 768, "window", rgbaBits, 16,
                                              8, 4, true);

  gl->renderLoop(true);

  return EXIT_SUCCESS;
}
