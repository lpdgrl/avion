cmake_minimum_required(VERSION 3.20)
project(Tests VERSION 0.0.1 LANGUAGES C CXX)

option(BUILD_AVION_MATH_TESTS "Build avion math tests" ON)
option(BUILD_AVION_CORE_SERIALIZATION_TESTS "Build avion core serialization tests" ON)

set(PROJECT_NAME Tests)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CTEST_COLOR)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.18.0
)

FetchContent_MakeAvailable(googletest)

enable_testing()

if (BUILD_AVION_MATH) 
  if (BUILD_AVION_MATH_TESTS)
    add_subdirectory(AvionMathTests)
  endif()
endif()

if (BUILD_AVION_CORE_SERIALIZATION_TESTS)
  add_subdirectory(AvionSerializationTests)
endif()

add_custom_target(avion_tests
  COMMAND ${CMAKE_CTEST_COMMAND}
          --output-on-failure
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Tests
)

