#include "prgl/GlslProgram.h"
#include "prgl/Texture2d.h"
#include "prgl/VertexArrayObject.h"
#include "prgl/VertexBufferObject.h"
#include "prgl/Window.h"

#include <random>

int32_t main(int32_t argc, char** args)
{
  const std::array<int32_t, 4> rgbaBits = {16, 16, 16, 16};
  auto gl = std::make_unique<prgl::Window>(1024, 768, "window", rgbaBits, 16, 8,
                                           4, true);

  auto tex = prgl::Texture2d(gl->getWidth(), gl->getHeight(),
                             prgl::TextureFormatInternal::Rgb32F,
                             prgl::TextureFormat::Rgb, prgl::DataType::Float);

  std::vector<prgl::color3f> buffer(gl->getWidth() * gl->getHeight());
  // fill the texture with a color
  prgl::color3f color = {0.25f, 0.1f, 0.3f};
  for (auto& c : buffer)
    {
      c = color;
    }
  tex.upload(buffer.data());

  // create a mesh
  std::vector<std::array<float, 2U>> positions = {
    {0.0, 1.0}, {1.0, 0.0}, {0.0, -1.0}, {-1.0, 0.0}};
  auto vboPosition = std::make_shared<prgl::VertexBufferObject>();
  vboPosition->create<2U>(positions);
  std::vector<std::array<float, 3U>> colors = {
    {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 1.0}};
  auto vboColors = std::make_shared<prgl::VertexBufferObject>();
  vboColors->create<3U>(colors);

  prgl::VertexArrayObject vao;
  vao.addVertexBufferObject(vboPosition);
  vao.addVertexBufferObject(vboColors);
  vao.addVertexBufferObject(vboColors);
  vao.addVertexBufferObject(vboColors);

  gl->setRenderFunction([&tex, &gl, &vao]() {
    tex.render(0.0f, 0.0f, gl->getWidth(), gl->getHeight());
    vao.bind(true);
    vao.render();
    vao.bind(false);
  });

  gl->renderLoop(true);

  return EXIT_SUCCESS;
}
