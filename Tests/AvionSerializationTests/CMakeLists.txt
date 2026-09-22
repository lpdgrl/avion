cmake_minimum_required(VERSION 3.20)

set(TARGET_NAME AvionSerializationTests)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CTEST_COLOR)

add_executable(TARGET_NAME
  src/serialization_test.cpp
)

target_link_libraries(TARGET_NAME
  PRIVATE
    AvionEngineCore
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(TARGET_NAME)