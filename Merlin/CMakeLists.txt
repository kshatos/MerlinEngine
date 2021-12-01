
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
    Merlin/Render/buffer_layout.cpp
    Merlin/Render/vertex_array.cpp
    Merlin/Render/opengl_vertex_array.cpp
    Merlin/Render/shader.cpp
    Merlin/Render/opengl_shader.cpp
    Merlin/Render/render_api.cpp
    Merlin/Render/opengl_render_api.cpp
    Merlin/Render/renderer.cpp
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
    Merlin/Render/buffer_layout.hpp
    Merlin/Render/vertex_array.hpp
    Merlin/Render/opengl_vertex_array.hpp
    Merlin/Render/shader.hpp
    Merlin/Render/opengl_shader.hpp
    Merlin/Render/render_api.hpp
    Merlin/Render/opengl_render_api.hpp
    Merlin/Render/renderer.hpp
)

add_library(Merlin STATIC
    ${MERLIN_SRC}
    ${MERLIN_INC}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spdlog)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm)

set_property(TARGET Merlin PROPERTY CXX_STANDARD 17)

target_include_directories(Merlin
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(Merlin
    PUBLIC
    glm
    PRIVATE
    glad
    spdlog::spdlog
    glfw
)