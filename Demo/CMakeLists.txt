
set(DEMO_SRC
    source/main.cpp
)

set(DEMO_INC
)

add_executable(Demo
    ${DEMO_SRC}
    ${DEMOC_INC}
)


set_property(TARGET Demo PROPERTY CXX_STANDARD 17)
set_property(TARGET Demo PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Demo>)

target_link_libraries(Demo
    Merlin
)

add_custom_command(
    TARGET Demo PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${MERLIN_ASSET_DIR} $<TARGET_FILE_DIR:Demo>/Assets
)