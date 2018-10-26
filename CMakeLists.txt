project(imgui-mix-sdl)

cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

include_directories(SYSTEM
  ${CMAKE_SOURCE_DIR}/imgui/examples/libs/gl3w
)

include_directories(
  ${SDL2_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/imgui
  ${CMAKE_SOURCE_DIR}/imgui/examples
)

add_subdirectory(imgui)

set(EXECUTABLE_NAME imgui_mix_sdl2)

add_executable(
    ${EXECUTABLE_NAME}
    Main.cpp
)

target_link_libraries(
    ${EXECUTABLE_NAME}
    ${SDL2_LIBRARY}
    imgui_sdl2
    ${OPENGL_LIBRARIES}
)
