file(GLOB IMGUI_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp
)

if(MERLIN_ENABLE_VULKAN)
    list(APPEND IMGUI_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_vulkan.cpp
    )
endif()
if(MERLIN_ENABLE_OPENGL)
    list(APPEND IMGUI_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    )
endif()

list(APPEND IMGUI_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
)

add_library(imgui
    STATIC
    ${IMGUI_SRC}
)

target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
)

if(MERLIN_ENABLE_VULKAN)
    target_link_libraries(imgui
        PUBLIC
        glfw
        ${Vulkan_LIBRARY}
    )
    target_include_directories(imgui
        PUBLIC
        ${Vulkan_INCLUDE_DIR}
    )
endif()