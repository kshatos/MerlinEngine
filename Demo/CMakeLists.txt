
set(DEMO_SRC
    source/main.cpp
)

set(DEMO_INC
    include/uv_sphere.hpp
)

add_executable(Demo
    ${DEMO_SRC}
    ${DEMO_INC}
)


set_property(TARGET Demo PROPERTY CXX_STANDARD 17)
set_property(TARGET Demo PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Demo>)

target_include_directories(Demo
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Demo
    Merlin
)

add_custom_command(
    TARGET Demo PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${MERLIN_ASSET_DIR} $<TARGET_FILE_DIR:Demo>/Assets
)