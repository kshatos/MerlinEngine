set(MERLIN_SRC
    Merlin/dummy.cpp
)

set(MERLIN_INC
    Merlin/dummy.hpp
)

add_library(Merlin STATIC
    ${MERLIN_SRC}
    ${MERLIN_INC}
)

set_property(TARGET Merlin PROPERTY CXX_STANDARD 17)

target_include_directories(Merlin
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
