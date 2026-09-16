cmake_minimum_required(VERSION 3.20)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CTEST_COLOR)
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.18.0
)

FetchContent_MakeAvailable(googletest)

add_executable(AvionMathTests
  vec3_test.cpp
)

target_link_libraries(AvionMathTests
  PRIVATE
    AvionMath
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(AvionMathTests)