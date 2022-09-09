set(MERLIN_EDITOR_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/merlin_editor_app.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/editor_gui_layer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene_hierarchy_panel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/asset_explorer_panel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/inspector_panel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/viewport_panel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/editor_commands.cpp
)

set(MERLIN_EDITOR_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/merlin_editor_app.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/editor_gui_layer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene_hierarchy_panel.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/asset_explorer_panel.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/inspector_panel.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/viewport_panel.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/editor_commands.hpp
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