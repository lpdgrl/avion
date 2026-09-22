cmake_minimum_required(VERSION 3.20)

add_executable(AvionMathTests
  src/vec3_test.cpp
)

target_link_libraries(AvionMathTests
  PRIVATE
    AvionMath
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(AvionMathTests)