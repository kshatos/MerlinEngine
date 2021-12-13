cmake_minimum_required(VERSION 3.15)
project(MerlinEngine)

set(MERLIN_ASSET_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Assets" CACHE PATH "Merlin core asset path")

option(BUILD_DEMO "Build the demo program" ON)
option(BUILD_TESTS "Build the test program" ON)
option(ENABLE_MERLIN_LOGGING "Enable merlin logging" ON)

if(ENABLE_MERLIN_LOGGING)
    add_compile_definitions(MERLIN_USE_LOGGING)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Merlin)

if(BUILD_DEMO)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Demo)
endif()

if (BUILD_TESTS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Test)
endif()