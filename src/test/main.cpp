#include "prgl/FrameBufferObject.h"
#include "prgl/GlslProgram.h"
#include "prgl/GlslRenderingPipelineProgram.h"
#include "prgl/Texture2d.h"
#include "prgl/VertexArrayObject.h"
#include "prgl/VertexBufferObject.h"
#include "prgl/Window.h"

int32_t main(int32_t argc, char** args)
{
  const std::array<int32_t, 4> rgbaBits = {16, 16, 16, 16};
  auto gl = std::make_unique<prgl::Window>(1024, 768, "window", rgbaBits, 16, 8,
                                           4, true);

  auto tex = prgl::Texture2d::Create(
    gl->getWidth(), gl->getHeight(), prgl::TextureFormatInternal::Rgb32F,
    prgl::TextureFormat::Rgb, prgl::DataType::Float);

  std::vector<prgl::vec3> colorBuffer(gl->getWidth() * gl->getHeight());
  // fill the texture with a color
  prgl::vec3 color = {0.25f, 0.1f, 0.3f};
  for (auto& c : colorBuffer)
    {
      c = color;
    }
  tex->upload(colorBuffer.data());

  // create vertex positions
  std::vector<prgl::vec3> positions = {
    {{-1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}};
  auto vboPosition = prgl::VertexBufferObject::Create();
  vboPosition->createBuffer(positions,
                            prgl::VertexBufferObject::Usage::StaticDraw);
  // create vertex colors
  std::vector<prgl::vec3> colors = {
    {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
  auto vboColors = prgl::VertexBufferObject::Create();
  vboColors->createBuffer(colors, prgl::VertexBufferObject::Usage::StaticDraw);

  auto vao = prgl::VertexArrayObject::Create();
  // add positions first (attrib 0)
  vao->addVertexBufferObject(vboPosition);
  // add colors (attrib 1)
  vao->addVertexBufferObject(vboColors);

  // create Frame Buffer Object
  auto fbo = prgl::FrameBufferObject::Create();
  fbo->attachTexture(tex);

  // create shader
  auto glsl = prgl::GlslRenderingPipelineProgram::Create();
  glsl->attachVertexShader(R"(
    #version 330 core

    layout(location = 0) in vec3 vertexPosition; // 0 since we've added positions first
    layout(location = 1) in vec3 vertexColor; // 1since we've added colors second

    out vec3 vColor;

    void main()
    {
      vColor = vertexColor;

      gl_Position = vec4(vertexPosition, 1.0);
    }
  )");
  glsl->attachFragmentShader(R"(
    #version 330 core

    in vec3 vColor;
    out vec3 color;

    void main()
    {
      color = vColor;
    }
  )");

  gl->setRenderFunction([&tex, &gl, &vao, &glsl, &fbo]() {
    {
      // bind the fbo
      const auto fboBinder = prgl::Binder(*fbo);
      {
        // bind the shader program
        const auto shaderBinder = prgl::Binder(*glsl);
        {
          // bind the VertexArrayObject
          const auto vaoBinder = prgl::Binder(*vao);

          // render the object
          vao->render(prgl::DrawMode::Triangles, 0U, 3U);
        }
      }
    }

    // render the texture into the main window
    tex->render(0.0f, 0.0f, gl->getWidth(), gl->getHeight());
  });

  gl->renderLoop(true);

  return EXIT_SUCCESS;
}
