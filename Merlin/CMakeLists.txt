
set(MERLIN_SRC
    Merlin/Core/logger.cpp
)

set(MERLIN_INC
    Merlin/Core/logger.hpp
    Merlin/Core/logger.hpp
    Merlin/Core/app_events.hpp
)

add_library(Merlin STATIC
    ${MERLIN_SRC}
    ${MERLIN_INC}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spdlog)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw)

set_property(TARGET Merlin PROPERTY CXX_STANDARD 17)

target_include_directories(Merlin
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Merlin
    PRIVATE
    spdlog::spdlog
    glfw
)