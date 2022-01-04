#=============================
# CORE
#=============================
set(MERLIN_CORE_SOURCE
    Merlin/Core/logger.cpp
    Merlin/Core/application.cpp
    Merlin/Core/window.cpp
    Merlin/Core/input.cpp
    Merlin/Core/imgui_layer.cpp
    Merlin/Core/layer_stack.cpp
)
set(MERLIN_CORE_INCLUDE
    Merlin/Core/core.hpp
    Merlin/Core/logger.hpp
    Merlin/Core/application.hpp
    Merlin/Core/app_events.hpp
    Merlin/Core/window.hpp
    Merlin/Core/input_codes.hpp
    Merlin/Core/input.hpp
    Merlin/Core/layer.hpp
    Merlin/Core/imgui_layer.hpp
    Merlin/Core/layer_stack.hpp
)
source_group("Core" FILES ${MERLIN_CORE_SOURCE} ${MERLIN_CORE_INCLUDE})

#=============================
# RENDER
#=============================
set(MERLIN_RENDER_SOURCE
    Merlin/Render/graphics_context.cpp
    Merlin/Render/vertex_buffer.cpp
    Merlin/Render/index_buffer.cpp
    Merlin/Render/frame_buffer.cpp
    Merlin/Render/buffer_layout.cpp
    Merlin/Render/vertex_array.cpp
    Merlin/Render/texture2d.cpp
    Merlin/Render/cubemap.cpp
    Merlin/Render/cubemap_data.cpp
    Merlin/Render/shader.cpp
    Merlin/Render/material.cpp
    Merlin/Render/render_api.cpp
    Merlin/Render/renderer.cpp
    Merlin/Render/camera.cpp
    Merlin/Render/perspective_camera.cpp
    Merlin/Render/transform.cpp
    Merlin/Render/mesh_utils.cpp
    Merlin/Render/skybox.cpp
)
set(MERLIN_RENDER_INCLUDE
    Merlin/Render/render.hpp
    Merlin/Render/graphics_context.hpp
    Merlin/Render/vertex_buffer.hpp
    Merlin/Render/index_buffer.hpp
    Merlin/Render/buffer_layout.hpp
    Merlin/Render/frame_buffer.hpp
    Merlin/Render/vertex_array.hpp
    Merlin/Render/texture.hpp
    Merlin/Render/texture2d.hpp
    Merlin/Render/cubemap.hpp
    Merlin/Render/cubemap_data.hpp
    Merlin/Render/shader.hpp
    Merlin/Render/material.hpp
    Merlin/Render/render_api.hpp
    Merlin/Render/renderer.hpp
    Merlin/Render/camera.hpp
    Merlin/Render/perspective_camera.hpp
    Merlin/Render/transform.hpp
    Merlin/Render/lighting.hpp
    Merlin/Render/mesh.hpp
    Merlin/Render/mesh_vertex.hpp
    Merlin/Render/mesh_utils.hpp
    Merlin/Render/skybox.hpp
)
source_group("Render" FILES ${MERLIN_RENDER_SOURCE} ${MERLIN_RENDER_INCLUDE})

#=============================
# SCENE
#=============================
set(MERLIN_SCENE_SOURCE
    Merlin/Scene/game_scene.cpp
    Merlin/Scene/entity.cpp
)
set(MERLIN_SCENE_INCLUDE
    Merlin/Scene/scene.hpp
    Merlin/Scene/game_scene.hpp
    Merlin/Scene/entity.hpp
    Merlin/Scene/component.hpp
    Merlin/Scene/core_components.hpp
)
source_group("Scene" FILES ${MERLIN_SCENE_SOURCE} ${MERLIN_SCENE_INCLUDE})

#=============================
# OPENGL
#=============================
set(MERLIN_OPENGL_SOURCE
    Merlin/Platform/OpenGL/opengl_context.cpp
    Merlin/Platform/OpenGL/opengl_vertex_buffer.cpp
    Merlin/Platform/OpenGL/opengl_index_buffer.cpp
    Merlin/Platform/OpenGL/opengl_vertex_array.cpp
    Merlin/Platform/OpenGL/opengl_frame_buffer.cpp
    Merlin/Platform/OpenGL/opengl_texture2d.cpp
    Merlin/Platform/OpenGL/opengl_cubemap.cpp
    Merlin/Platform/OpenGL/opengl_shader.cpp
    Merlin/Platform/OpenGL/opengl_render_api.cpp
)
set(MERLIN_OPENGL_INCLUDE
    Merlin/Platform/OpenGL/opengl_context.hpp
    Merlin/Platform/OpenGL/opengl_vertex_buffer.hpp
    Merlin/Platform/OpenGL/opengl_index_buffer.hpp
    Merlin/Platform/OpenGL/opengl_vertex_array.hpp
    Merlin/Platform/OpenGL/opengl_frame_buffer.hpp
    Merlin/Platform/OpenGL/opengl_texture2d.hpp
    Merlin/Platform/OpenGL/opengl_cubemap.hpp
    Merlin/Platform/OpenGL/opengl_shader.hpp
    Merlin/Platform/OpenGL/opengl_render_api.hpp
)
source_group("OpenGL" FILES ${MERLIN_OPENGL_SOURCE} ${MERLIN_OPENGL_INCLUDE})

#=============================
# GLFW
#=============================
set(MERLIN_GLFW_SOURCE
    Merlin/Platform/GLFW/glfw_window.cpp
    Merlin/Platform/GLFW/glfw_input.cpp
)
set(MERLIN_GLFW_INCLUDE
    Merlin/Platform/GLFW/glfw_window.hpp
    Merlin/Platform/GLFW/glfw_input.hpp
)
source_group("GLFW" FILES ${MERLIN_GLFW_SOURCE} ${MERLIN_GLFW_INCLUDE})

#=============================
# Third Party Setup
#=============================
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spdlog)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb)

#=============================
# LIBRARY
#=============================
set(MERLIN_SOURCE
    ${MERLIN_CORE_SOURCE}
    ${MERLIN_RENDER_SOURCE}
    ${MERLIN_SCENE_SOURCE}
    ${MERLIN_OPENGL_SOURCE}
    ${MERLIN_GLFW_SOURCE}
)
set(MERLIN_INCLUDE
    ${MERLIN_CORE_INCLUDE}
    ${MERLIN_RENDER_INCLUDE}
    ${MERLIN_SCENE_INCLUDE}
    ${MERLIN_OPENGL_INCLUDE}
    ${MERLIN_GLFW_INCLUDE}
)

add_library(Merlin STATIC
    ${MERLIN_SOURCE}
    ${MERLIN_INCLUDE}
)

set_property(TARGET Merlin PROPERTY CXX_STANDARD 17)

target_include_directories(Merlin
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Merlin
    PUBLIC
    glm
    imgui
    PRIVATE
    glad
    spdlog::spdlog
    glfw
    stb
)
