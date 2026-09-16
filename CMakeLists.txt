cmake_minimum_required(VERSION 3.20)
project(AvionEngine VERSION 0.0.1 LANGUAGES C CXX)

set(PROJECT_NAME AvionEngine)

option(BUILD_AVION_MATH "Build avion math" ON)
option(BUILD_AVION_MATH_TESTS "Build avion math tests" ON)

if (BUILD_AVION_MATH) 
  add_subdirectory(AvionMath)
  if (BUILD_AVION_MATH_TESTS)
    enable_testing()
    add_subdirectory(Tests/AvionMathTests)

    add_custom_target(avion_tests
      COMMAND ${CMAKE_CTEST_COMMAND}
              --output-on-failure
      DEPENDS AvionMathTests
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
  endif()
endif()

add_subdirectory(AvionEngineCore)
add_subdirectory(AvionEngineEditor)
add_subdirectory(AvionEngineTest)





set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT AvionEngineTest)
