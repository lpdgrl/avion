cmake_minimum_required(VERSION 3.12)

set(ENGINE_PROJECT_NAME AvionEngineCore)

find_package(Freetype REQUIRED)
find_package(glfw3 REQUIRED)

SET(CMAKE_EXE_LINKER_FLAGS "-v")

SET(IMGUI_DIR libs/imgui)

add_library(${ENGINE_PROJECT_NAME} SHARED
    src/AvionEngineCore/logger/logger.cpp
    src/AvionEngineCore/render/renderer.cpp
    src/AvionEngineCore/render/text_rendering.cpp
    src/AvionEngineCore/render/shader.cpp
    src/AvionEngineCore/render/camera.cpp
    src/AvionEngineCore/render/scene.cpp
    src/AvionEngineCore/render/object.cpp
    src/AvionEngineCore/render/gui.cpp
    src/AvionEngineCore/render/window.cpp
    src/AvionEngineCore/controller/controller.cpp
    src/AvionEngineCore/external/glad.c
    libs/imgui/imgui_demo.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/imgui.cpp
    libs/imgui/backends/imgui_impl_glfw.cpp
    libs/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(${ENGINE_PROJECT_NAME} 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes
    PUBLIC ${FREETYPE_INCLUDE_DIRS}
    PUBLIC ${IMGUI_DIR}
    PUBLIC ${IMGUI_DIR}/backends
)

target_link_libraries(${ENGINE_PROJECT_NAME} 
    PUBLIC
        ${FREETYPE_LIBRARIES}
        glfw
)

target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_20)

message(STATUS "FINAL include dirs:")
get_target_property(tmp_inc ${ENGINE_PROJECT_NAME} INCLUDE_DIRECTORIES)
message(STATUS "${tmp_inc}")
