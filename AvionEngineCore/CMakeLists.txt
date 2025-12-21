cmake_minimum_required(VERSION 3.12)

set(ENGINE_PROJECT_NAME AvionEngineCore)

find_package(Freetype REQUIRED)
find_package(glfw3 REQUIRED)

SET(CMAKE_EXE_LINKER_FLAGS "-v")

add_library(${ENGINE_PROJECT_NAME} SHARED
    src/AvionEngineCore/logger/logger.cpp
    src/AvionEngineCore/render/render.cpp
    src/AvionEngineCore/render/text_rendering.cpp
    src/AvionEngineCore/render/shader.cpp
    src/AvionEngineCore/render/camera.cpp
    src/AvionEngineCore/controller/controller.cpp
    src/AvionEngineCore/external/glad.c
)

target_include_directories(${ENGINE_PROJECT_NAME} 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes
    PUBLIC ${FREETYPE_INCLUDE_DIRS}
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
