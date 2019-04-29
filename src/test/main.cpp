#include "prgl/GlslProgram.h"
#include "prgl/GlslRenderingPipelineProgram.h"
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

  std::vector<prgl::vec3> buffer(gl->getWidth() * gl->getHeight());
  // fill the texture with a color
  prgl::vec3 color = {0.25f, 0.1f, 0.3f};
  for (auto& c : buffer)
    {
      c = color;
    }
  tex.upload(buffer.data());

  // create vertex positions
  std::vector<prgl::vec3> positions = {
    {{-1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}};
  auto vboPosition = prgl::VertexBufferObject();
  vboPosition.create<3U>(positions);
  // create vertex colors
  std::vector<prgl::vec3> colors = {
    {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 1.0}};
  auto vboColors = prgl::VertexBufferObject();
  vboColors.create<3U>(colors);

  prgl::VertexArrayObject vao;
  vao.addVertexBufferObject(vboPosition);
  vao.addVertexBufferObject(vboColors);

  // create shader
  prgl::GlslRenderingPipelineProgram glsl;
  glsl.attachVertexShader(R"(
    #version 330 core

    layout(location = 0) in vec3 vertexPosition;
    layout(location = 1) in vec3 vertexColor;

    out vec3 vColor;

    void main()
    {
      vColor = vertexColor;

      gl_Position = vec4(vertexPosition, 1.0);
    }
  )");
  glsl.attachFragmentShader(R"(
    #version 330 core

    in vec3 vColor;
    out vec3 color;

    void main()
    {
      color = vColor;
    }
  )");

  gl->setRenderFunction([&tex, &gl, &vao, &glsl]() {
    // render the texture as background
    tex.render(0.0f, 0.0f, gl->getWidth(), gl->getHeight());
    // bind our shader program
    glsl.bind(true);
    // bind the VertexArrayObject
    vao.bind(true);
    // render the object
    vao.render();
    // unbind VertexArrayObject
    vao.bind(false);
    // unbind shader
    glsl.bind(false);
  });

  gl->renderLoop(true);

  return EXIT_SUCCESS;
}
