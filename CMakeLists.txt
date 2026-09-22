cmake_minimum_required(VERSION 3.20)
project(AvionEngine VERSION 0.0.1 LANGUAGES C CXX)

set(PROJECT_NAME AvionEngine)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_AVION_MATH "Build avion math" ON)


if (BUILD_AVION_MATH)
  add_subdirectory(AvionMath)
endif()

add_subdirectory(AvionEngineCore)
add_subdirectory(AvionEngineEditor)
add_subdirectory(AvionEngineTest)

if (BUILD_TESTS)
  add_subdirectory(Tests)
endif()


set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT AvionEngineTest)
