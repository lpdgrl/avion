cmake_minimum_required(VERSION 3.20)

set(TEST_PROJECT_NAME AvionEngineTest)

add_executable(${TEST_PROJECT_NAME} 
#    src/main.cpp
     src/editor.cpp
)

target_link_libraries(${TEST_PROJECT_NAME} AvionEngineEditor)
target_compile_features(${TEST_PROJECT_NAME} PUBLIC cxx_std_23)

set_target_properties(${TEST_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)