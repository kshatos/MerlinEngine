set(MERLIN_EDITOR_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/merlin_editor_app.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/editor_gui_layer.cpp
)

set(MERLIN_EDITOR_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/merlin_editor_app.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/editor_gui_layer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/panel.hpp
)

add_executable(MerlinEditor
    ${MERLIN_EDITOR_SOURCE}
    ${MERLIN_EDITOR_INCLUDE}
)

set_property(TARGET MerlinEditor PROPERTY CXX_STANDARD 17)

target_include_directories(MerlinEditor
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MerlinEditor
    PUBLIC
    Merlin
)