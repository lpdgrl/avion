cmake_minimum_required(VERSION 3.12)
project(AvionEngine VERSION 0.0.1 LANGUAGES C CXX)

set(PROJECT_NAME AvionEngine)

add_subdirectory(AvionEngineCore)
# add_subdirectory(AvionEngineTest)

# set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT AvionEngineTest)
