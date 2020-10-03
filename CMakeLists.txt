cmake_minimum_required(VERSION 3.17)

project(prgl)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package (Threads REQUIRED)

# glew
if (UNIX)
  find_package(PkgConfig REQUIRED)
  pkg_search_module(GLEW REQUIRED glew)
else ()
  file(TO_CMAKE_PATH "$ENV{GLEW_HOME}" GLEW_HOME)
  set(GLEW_INCLUDE_DIR "${GLEW_HOME}/include")
  find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s PATHS "${GLEW_HOME}/lib" NO_DEFAULT_PATH)
  set(GLEW_LIBRARIES ${GLEW_LIBRARY})
endif (UNIX)

add_library(${PROJECT_NAME} STATIC
  src/ContextImplementation.cxx
  src/Window.cxx
  src/Texture2d.cxx
  src/GlslProgram.cxx
  src/FrameBufferObject.cxx
  src/ShaderStorageBuffer.cxx
  src/GlslRenderingPipelineProgram.cxx
  src/GlslComputeShader.cxx
  src/VertexBufferObject.cxx
  src/VertexArrayObject.cxx

)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/>
  PUBLIC $<INSTALL_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/>
  PUBLIC ${OPENGL_INCLUDE_DIR}
  PUBLIC ${GLFW_INCLUDE_DIRS}
  PUBLIC ${GLEW_INCLUDE_DIRS}
)


target_link_libraries(${PROJECT_NAME}
  OpenGL::GL
  glfw
  ${GLEW_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)


## testing

# executable
add_executable(${PROJECT_NAME}_test
  test/main.cxx
)

target_link_libraries(${PROJECT_NAME}_test
  ${PROJECT_NAME}
)

set_property(TARGET ${PROJECT_NAME}_test PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME}_test PROPERTY CXX_STANDARD_REQUIRED ON)
