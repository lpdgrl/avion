cmake_minimum_required(VERSION 3.12)

set(ENGINE_PROJECT_NAME AvionEngineCore)

# find_package(Freetype REQUIRED)
# find_package(glfw3 REQUIRED)

SET(CMAKE_EXE_LINKER_FLAGS "-v")

SET(IMGUI_DIR libs/imgui)

SET(GLFW_DIR libs/glfw)

add_subdirectory(${GLFW_DIR})

add_library(${ENGINE_PROJECT_NAME} SHARED
    src/AvionEngineCore/logger/logger.cpp
    src/AvionEngineCore/renderer/renderer.cpp
    # src/AvionEngineCore/renderer/text_rendering.cpp
    src/AvionEngineCore/renderer/pipeline.cpp
    src/AvionEngineCore/renderer/shader.cpp
    src/AvionEngineCore/renderer/camera.cpp
    src/AvionEngineCore/core/scene.cpp
    src/AvionEngineCore/core/object.cpp
    src/AvionEngineCore/core/texture.cpp
    src/AvionEngineCore/core/window.cpp
    src/AvionEngineCore/core/light.cpp
    src/AvionEngineCore/core/material.cpp
    src/AvionEngineCore/core/resource_manager.cpp
    src/AvionEngineCore/gui/widget.cpp
    src/AvionEngineCore/controller/controller.cpp
    src/AvionEngineCore/external/glad.c
    src/AvionEngineCore/external/stb.c
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/imgui.cpp
    libs/imgui/backends/imgui_impl_glfw.cpp
    libs/imgui/backends/imgui_impl_opengl3.cpp
    
)

target_include_directories(${ENGINE_PROJECT_NAME} 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes
    # PUBLIC ${FREETYPE_INCLUDE_DIRS}
    PUBLIC ${IMGUI_DIR}
    PUBLIC ${IMGUI_DIR}/backends
)

target_link_libraries(${ENGINE_PROJECT_NAME} 
    PUBLIC
        ${FREETYPE_LIBRARIES}
        glfw
)

target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_23)
target_compile_options(${ENGINE_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

message(STATUS "FINAL include dirs:")
get_target_property(tmp_inc ${ENGINE_PROJECT_NAME} INCLUDE_DIRECTORIES)
message(STATUS "${tmp_inc}")
