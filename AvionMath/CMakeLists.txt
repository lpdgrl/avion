cmake_minimum_required(VERSION 3.20)

set(MATH_PROJECT_NAME AvionMath)
set(CMAKE_COLOR_DIAGNOSTICS ON)

add_library(${MATH_PROJECT_NAME} INTERFACE)

target_include_directories(${MATH_PROJECT_NAME} 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(${MATH_PROJECT_NAME} INTERFACE cxx_std_26)
target_compile_options(${MATH_PROJECT_NAME} INTERFACE -Wall -Wextra -Wpedantic)
target_compile_options(${MATH_PROJECT_NAME} INTERFACE -O3)
