cmake_minimum_required(VERSION 3.15)
project(MerlinEngine)

option(BUILD_DEMO "Build the demo program" ON)
option(BUILD_TESTS "Build the test program" ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/Merlin)

if(BUILD_DEMO)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Demo)
endif()

if (BUILD_TESTS)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Test)
endif()