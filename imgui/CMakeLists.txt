set(LIBRARY_NAME imgui_sdl2)

set(SRC_GRP_IMGUI
    imgui.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
    imgui_demo.cpp
)

set(SRC_GRP_IMGUI_SDL2_IMPL
    examples/imgui_impl_sdl.cpp
    examples/imgui_impl_opengl3.cpp
)

set(SRC_GRP_IMGUI_GL3W
    examples/libs/gl3w/GL/gl3w.c
)

set(LIBRARY_SRCS
    ${SRC_GRP_IMGUI}
    ${SRC_GRP_IMGUI_SDL2_IMPL}
    ${SRC_GRP_IMGUI_GL3W}
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/examples
)

include_directories(SYSTEM
  ${CMAKE_CURRENT_SOURCE_DIR}/examples/libs/gl3w
  ${SDL2_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIR}
  ${SDL2_MIXER_INCLUDE_DIR}
  ${SDL2_TTF_INCLUDE_DIR}
)

add_library(${LIBRARY_NAME} STATIC
  ${LIBRARY_SRCS}
)

target_link_libraries(
    ${LIBRARY_NAME}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${CMAKE_DL_LIBS}
)
