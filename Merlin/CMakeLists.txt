
set(MERLIN_SRC
    Merlin/Core/logger.cpp
    Merlin/Core/window.cpp
    Merlin/Core/glfw_window.cpp
    Merlin/Render/graphics_context.cpp
    Merlin/Render/opengl_context.cpp
    Merlin/Render/vertex_buffer.cpp
    Merlin/Render/opengl_vertex_buffer.cpp
    Merlin/Render/index_buffer.cpp
    Merlin/Render/opengl_index_buffer.cpp
)

set(MERLIN_INC
    Merlin/Core/logger.hpp
    Merlin/Core/logger.hpp
    Merlin/Core/app_events.hpp
    Merlin/Core/window.hpp
    Merlin/Core/glfw_window.hpp
    Merlin/Render/graphics_context.hpp
    Merlin/Render/opengl_context.hpp
    Merlin/Render/vertex_buffer.hpp
    Merlin/Render/opengl_vertex_buffer.hpp
    Merlin/Render/index_buffer.hpp
    Merlin/Render/opengl_index_buffer.hpp
)

add_library(Merlin STATIC
    ${MERLIN_SRC}
    ${MERLIN_INC}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spdlog)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad)

set_property(TARGET Merlin PROPERTY CXX_STANDARD 17)

target_include_directories(Merlin
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(Merlin
    PRIVATE
    spdlog::spdlog
    glfw
    glad
)