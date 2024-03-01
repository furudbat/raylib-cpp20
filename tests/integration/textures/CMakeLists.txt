# Executable
add_executable(raylib_cpp_textures_test raylib_cpp_textures_test.cpp)
target_include_directories(raylib_cpp_textures_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(raylib_cpp_textures_test PRIVATE project_warnings project_options)
target_link_libraries(raylib_cpp_textures_test PRIVATE raylib_cpp raylib_cpp_test_helper)
target_link_system_libraries(raylib_cpp_textures_test PRIVATE raylib tl::expected)
target_link_system_libraries(raylib_cpp_textures_test PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
target_compile_options(raylib_cpp_textures_test PRIVATE -fno-exceptions)
target_compile_definitions(raylib_cpp_textures_test PRIVATE RAYLIB_CPP_INCLUDE_EXPECTED="tl/expected.hpp")
target_compile_definitions(raylib_cpp_textures_test PRIVATE RAYLIB_CPP_EXPECTED=tl::expected)
target_compile_definitions(raylib_cpp_textures_test PRIVATE RAYLIB_CPP_UNEXPECTED=tl::unexpected)

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX to whatever you
# want, or use different for different binaries
catch_discover_tests(raylib_cpp_textures_test)

# Copy all the resources
file(COPY resources/ DESTINATION "resources/")
