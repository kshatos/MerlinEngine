#=============================
# CORE
#=============================
set(MERLIN_CORE_SOURCE
    Merlin/Core/logger.cpp
    Merlin/Core/application.cpp
    Merlin/Core/window.cpp
    Merlin/Core/input.cpp
    Merlin/Core/layer_stack.cpp
    Merlin/Core/file_util.cpp
    Merlin/Core/uuid.cpp
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
    Merlin/Core/layer_stack.hpp
    Merlin/Core/file_util.hpp
    Merlin/Core/uuid.hpp
)
source_group("Core" FILES ${MERLIN_CORE_SOURCE} ${MERLIN_CORE_INCLUDE})


#=============================
# RENDER
#=============================
set(MERLIN_RENDER_SOURCE
    Merlin/Render/buffer_data.cpp
    Merlin/Render/texture2d_data.cpp
    Merlin/Render/cubemap_data.cpp
    Merlin/Render/material.cpp
    Merlin/Render/render_api.cpp
    Merlin/Render/renderer.cpp
    Merlin/Render/perspective_camera.cpp
    Merlin/Render/transform.cpp
    Merlin/Render/mesh_utils.cpp
    Merlin/Render/skybox.cpp
    Merlin/Render/mesh_buffer.cpp
)
set(MERLIN_RENDER_INCLUDE
    Merlin/Render/render.hpp
    Merlin/Render/vertex_buffer.hpp
    Merlin/Render/index_buffer.hpp
    Merlin/Render/buffer_data.hpp
    Merlin/Render/frame_buffer.hpp
    Merlin/Render/texture.hpp
    Merlin/Render/texture2d.hpp
    Merlin/Render/texture2d_data.hpp
    Merlin/Render/cubemap.hpp
    Merlin/Render/cubemap_data.hpp
    Merlin/Render/shader.hpp
    Merlin/Render/material.hpp
    Merlin/Render/render_api.hpp
    Merlin/Render/scene_render_data.hpp
    Merlin/Render/renderer.hpp
    Merlin/Render/camera.hpp
    Merlin/Render/perspective_camera.hpp
    Merlin/Render/transform.hpp
    Merlin/Render/mesh.hpp
    Merlin/Render/mesh_vertex.hpp
    Merlin/Render/mesh_utils.hpp
    Merlin/Render/skybox.hpp
    Merlin/Render/mesh_buffer.hpp
)
source_group("Render" FILES ${MERLIN_RENDER_SOURCE} ${MERLIN_RENDER_INCLUDE})


#=============================
# SCENE
#=============================
set(MERLIN_SCENE_SOURCE
    Merlin/Scene/game_scene.cpp
    Merlin/Scene/entity.cpp
    Merlin/Scene/scene_serialization.cpp
)
set(MERLIN_SCENE_INCLUDE
    Merlin/Scene/scene.hpp
    Merlin/Scene/game_scene.hpp
    Merlin/Scene/entity.hpp
    Merlin/Scene/core_components.hpp
    Merlin/Scene/scene_serialization.hpp
)
source_group("Scene" FILES ${MERLIN_SCENE_SOURCE} ${MERLIN_SCENE_INCLUDE})


#=============================
# OPENGL
#=============================
if(MERLIN_ENABLE_OPENGL)
    set(MERLIN_OPENGL_SOURCE
        Merlin/Platform/OpenGL/opengl_vertex_buffer.cpp
        Merlin/Platform/OpenGL/opengl_index_buffer.cpp
        Merlin/Platform/OpenGL/opengl_frame_buffer.cpp
        Merlin/Platform/OpenGL/opengl_texture2d.cpp
        Merlin/Platform/OpenGL/opengl_cubemap.cpp
        Merlin/Platform/OpenGL/opengl_shader.cpp
        Merlin/Platform/OpenGL/opengl_render_api.cpp
        Merlin/Platform/OpenGL/opengl_material.cpp
    )
    set(MERLIN_OPENGL_INCLUDE
        Merlin/Platform/OpenGL/opengl_vertex_buffer.hpp
        Merlin/Platform/OpenGL/opengl_index_buffer.hpp
        Merlin/Platform/OpenGL/opengl_frame_buffer.hpp
        Merlin/Platform/OpenGL/opengl_texture2d.hpp
        Merlin/Platform/OpenGL/opengl_cubemap.hpp
        Merlin/Platform/OpenGL/opengl_shader.hpp
        Merlin/Platform/OpenGL/opengl_render_api.hpp
        Merlin/Platform/OpenGL/opengl_material.hpp
    )
    source_group("OpenGL" FILES ${MERLIN_OPENGL_SOURCE} ${MERLIN_OPENGL_INCLUDE})
endif()


#=============================
# VULKAN
#=============================
if(MERLIN_ENABLE_VULKAN)
    set(MERLIN_VULKAN_SOURCE
        Merlin/Platform/Vulkan/vulkan_render_api.cpp
        Merlin/Platform/Vulkan/vulkan_util.cpp
        Merlin/Platform/Vulkan/vulkan_vertex_buffer.cpp
        Merlin/Platform/Vulkan/vulkan_index_buffer.cpp
        Merlin/Platform/Vulkan/vulkan_texture2d.cpp
        Merlin/Platform/Vulkan/vulkan_material.cpp
    )
    set(MERLIN_VULKAN_INCLUDE
        Merlin/Platform/Vulkan/vulkan_render_api.hpp
        Merlin/Platform/Vulkan/vulkan_util.hpp
        Merlin/Platform/Vulkan/vulkan_vertex_buffer.hpp
        Merlin/Platform/Vulkan/vulkan_index_buffer.hpp
        Merlin/Platform/Vulkan/vulkan_texture2d.hpp
        Merlin/Platform/Vulkan/vulkan_material.hpp
    )
    source_group("Vulkan" FILES ${MERLIN_VULKAN_SOURCE} ${MERLIN_VULKAN_INCLUDE})
endif()


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
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/assimp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/entt)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stduuid)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/yaml-cpp)
target_compile_definitions(yaml-cpp PUBLIC YAML_CPP_STATIC_DEFINE)
if(MERLIN_ENABLE_OPENGL)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad)
endif()


#=============================
# LIBRARY
#=============================
set(MERLIN_SOURCE
    ${MERLIN_CORE_SOURCE}
    ${MERLIN_RENDER_SOURCE}
    ${MERLIN_SCENE_SOURCE}
    ${MERLIN_GLFW_SOURCE}
)
set(MERLIN_INCLUDE
    ${MERLIN_CORE_INCLUDE}
    ${MERLIN_RENDER_INCLUDE}
    ${MERLIN_SCENE_INCLUDE}
    ${MERLIN_GLFW_INCLUDE}
)

if(MERLIN_ENABLE_OPENGL)
    list(APPEND MERLIN_SOURCE ${MERLIN_OPENGL_SOURCE})
    list(APPEND MERLIN_INCLUDE ${MERLIN_OPENGL_INCLUDE})
endif()

if(MERLIN_ENABLE_VULKAN)
    list(APPEND MERLIN_SOURCE ${MERLIN_VULKAN_SOURCE})
    list(APPEND MERLIN_INCLUDE ${MERLIN_VULKAN_INCLUDE})
endif()

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
    EnTT::EnTT
    PRIVATE
    stduuid
    spdlog::spdlog
    glfw
    stb
    assimp
    yaml-cpp
)

if(MERLIN_ENABLE_OPENGL)
    target_link_libraries(Merlin
        PRIVATE
        glad
    )
endif()
if(MERLIN_ENABLE_VULKAN)
    target_include_directories(Merlin
    PUBLIC
    ${Vulkan_INCLUDE_DIR}
    )
    target_link_libraries(Merlin
    PUBLIC
    ${Vulkan_LIBRARY}
    )
endif()
