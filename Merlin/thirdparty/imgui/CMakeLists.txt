file(GLOB IMGUI_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

list(APPEND IMGUI_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui
    STATIC
    ${IMGUI_SRC}
)

target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

