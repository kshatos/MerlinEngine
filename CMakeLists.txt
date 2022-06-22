cmake_minimum_required(VERSION 3.15)
project(MerlinEngine)

set(MERLIN_ASSET_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Assets" CACHE PATH "Merlin core asset path")

option(MERLIN_BUILD_DEMO "Build the demo program" ON)
option(MERLIN_BUILD_TESTS "Build the test program" ON)
option(MERLIN_ENABLE_LOGGING "Enable merlin logging" ON)
option(MERLIN_ENABLE_OPENGL "Use OpenGL as the graphics API" ON)
option(MERLIN_ENABLE_VULKAN "use Vulkan as the graphics API" ON)

if(MERLIN_ENABLE_LOGGING)
    message("Using Logger")
    add_compile_definitions(MERLIN_USE_LOGGING)
endif()

if (MERLIN_ENABLE_OPENGL)
    message("Using OpenGL")
    add_compile_definitions(MERLIN_USE_OPENGL)
endif()

if (MERLIN_ENABLE_VULKAN)
    message("Using Vulkan")
    find_package(Vulkan REQUIRED)
    add_compile_definitions(MERLIN_USE_VULKAN)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/VulkanTest)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Merlin)

if(MERLIN_BUILD_DEMO)
    message("Building Demo")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Demo)
endif()

if (MERLIN_BUILD_TESTS)
    message("Building Tests")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Test)
endif()

