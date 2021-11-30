
set(MERLIN_SRC
    Merlin/Core/logger.cpp
    Merlin/Core/window.cpp
    Merlin/Core/glfw_window.cpp
    Merlin/Render/graphics_context.cpp
    Merlin/Render/opengl_context.cpp
)

set(MERLIN_INC
    Merlin/Core/logger.hpp
    Merlin/Core/logger.hpp
    Merlin/Core/app_events.hpp
    Merlin/Core/window.hpp
    Merlin/Core/glfw_window.hpp
    Merlin/Render/graphics_context.hpp
    Merlin/Render/opengl_context.hpp
)

add_library(Merlin STATIC
    ${MERLIN_SRC}
    ${MERLIN_INC}
)

find_package(OpenGL REQUIRED)
target_include_directories(Merlin
    PUBLIC
    ${OPENGL_INCLUDE_DIRS})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spdlog)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw)

set_property(TARGET Merlin PROPERTY CXX_STANDARD 17)

target_include_directories(Merlin
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(Merlin
    PRIVATE
    spdlog::spdlog
    glfw
    ${OPENGL_LIBRARIES}
)