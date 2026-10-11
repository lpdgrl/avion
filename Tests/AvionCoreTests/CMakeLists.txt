cmake_minimum_required(VERSION 3.20)
project(AvionCoreTests)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CTEST_COLOR)

add_executable(AvionCoreTests
  src/ResourceManagerTests.cpp
  src/SerializationTests.cpp
)

target_link_libraries(AvionCoreTests
  PRIVATE
    AvionEngineCore
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(AvionCoreTests)