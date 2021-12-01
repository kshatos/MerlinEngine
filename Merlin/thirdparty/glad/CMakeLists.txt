set(GLAD_SRC
    include/glad/glad.h
    src/glad.c
)

add_library(glad
    STATIC
    ${GLAD_SRC}
)

target_include_directories(glad
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)