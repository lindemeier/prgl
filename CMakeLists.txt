cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(prgl)

# GLFW
if (WIN32)
  file(TO_CMAKE_PATH "$ENV{GLFW_HOME}" GLFW_HOME)
  set(GLFW_INCLUDE_DIR "$ENV{GLFW_HOME}/include")
  find_library(GLFW_LIBRARY NAMES glfw3dll glfw glfw3 libglfw libglfw3 PATHS "${GLFW_HOME}/lib" NO_DEFAULT_PATH)
  set(GLFW_LIBS ${GLFW_LIBRARY})
else ()
  find_package(PkgConfig REQUIRED)
  pkg_search_module(GLFW REQUIRED glfw3)
  set(GLFW_LIBS ${GLFW_LIBRARIES}) 
endif ()

#OpenGL
find_package(OpenGL REQUIRED)

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
  include/prgl/Window.h
  include/prgl/Texture2D.h
  include/prgl/GlslProgram.h
  include/prgl/FrameBufferObject.h
  include/prgl/ShaderStorageBuffer.h 	
  src/glCommon.h
  src/ContextImplementation.h
  src/ContextImplementation.cpp
  src/Window.cpp
  src/Texture2D.cpp
  src/GlslProgram.cpp
  src/FrameBufferObject.cpp
  src/ShaderStorageBuffer.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/>
  PUBLIC $<INSTALL_INTERFACE:include/>
  PUBLIC ${OPENGL_INCLUDE_DIR}
  PUBLIC ${GLFW_INCLUDE_DIRS}
  PUBLIC ${GLEW_INCLUDE_DIRS}
)


target_link_libraries(${PROJECT_NAME}
  ${OPENGL_gl_LIBRARY}
  ${GLFW_LIBS}
  ${GLEW_LIBRARIES}
)


## testing

# executable
add_executable(${PROJECT_NAME}_test
  src/test/main.cpp
)

target_link_libraries(${PROJECT_NAME}_test
  ${PROJECT_NAME}
)

set_property(TARGET ${PROJECT_NAME}_test PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME}_test PROPERTY CXX_STANDARD_REQUIRED ON)
