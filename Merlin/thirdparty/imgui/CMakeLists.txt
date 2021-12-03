file(GLOB IMGUI_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp) 

add_library(imgui
    STATIC
    ${IMGUI_SRC}
)

target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

