
set(DEMO_SRC
    source/main.cpp
)

set(DEMO_INC
)

add_executable(Demo
    ${DEMO_SRC}
    ${DEMOC_INC}
)

target_link_libraries(Demo
    Merlin
)