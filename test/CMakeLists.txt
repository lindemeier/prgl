cmake_minimum_required(VERSION 3.10.2)

project(prglUnitTests)

add_executable(${PROJECT_NAME}
  ProjectionTest.cxx
  test_main.cxx
)

add_test(
  NAME ${PROJECT_NAME}
  COMMAND ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
  gtest
  prgl
)

add_dependencies(${PROJECT_NAME}
  gtest
  prgl
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)
