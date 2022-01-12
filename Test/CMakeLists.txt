
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  Test
  source/transform_test.cpp
  source/mesh_test.cpp
  source/cubemap_data_tests.cpp
  source/texture2d_tests.cpp
)
target_link_libraries(
    Test
    gtest_main
    Merlin
)

include(GoogleTest)
gtest_discover_tests(Test)