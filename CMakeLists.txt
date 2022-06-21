cmake_minimum_required(VERSION 3.15)
project(MerlinEngine)

set(MERLIN_ASSET_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Assets" CACHE PATH "Merlin core asset path")

option(BUILD_DEMO "Build the demo program" ON)
option(BUILD_TESTS "Build the test program" ON)
option(ENABLE_MERLIN_LOGGING "Enable merlin logging" ON)
option(ENABLE_OPENGL "Use OpenGL as the graphics API" ON)
option(ENABLE_VULKAN "use Vulkan as the graphics API" ON)

if(ENABLE_MERLIN_LOGGING)
    add_compile_definitions(MERLIN_USE_LOGGING)
endif()

if (ENABLE_OPENGL)
    message("Using OpenGL")
    add_compile_definitions(MERLIN_USE_OPENGL)
endif()

if (ENABLE_VULKAN)
    message("Using Vulkan")
    find_package(Vulkan REQUIRED)
    add_compile_definitions(MERLIN_USE_VULKAN)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/VulkanTest)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Merlin)

if(BUILD_DEMO)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Demo)
endif()

if (BUILD_TESTS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Test)
endif()

