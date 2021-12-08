set(STB_SRC
    stb/stb_image.h
)

add_library(stb INTERFACE)

target_include_directories(stb
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)