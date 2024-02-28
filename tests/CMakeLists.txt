CPMAddPackage("gh:catchorg/Catch2@3.5.2")
#find_package(Catch2 CONFIG REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)

# Test Options
set(CTEST_CUSTOM_TESTS_IGNORE pkg-config--static)

# Executable
add_executable(raylib_cpp_test raylib_cpp_test.cpp)
target_include_directories(raylib_cpp_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(raylib_cpp_test PRIVATE project_warnings project_options)
if (MSVC)
    target_compile_options(raylib_cpp_test PRIVATE /Wall /W4)
else()
    target_compile_options(raylib_cpp_test PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion)
endif()
target_link_libraries(raylib_cpp_test PRIVATE raylib_cpp)
target_link_system_libraries(raylib_cpp_test PRIVATE raylib)
target_link_system_libraries(raylib_cpp_test PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
target_compile_options(raylib_cpp_test PRIVATE -fexceptions)
target_compile_options(raylib_cpp_test PRIVATE -Wno-shadow)

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX to whatever you
# want, or use different for different binaries
catch_discover_tests(raylib_cpp_test)

# Copy all the resources
file(COPY resources/ DESTINATION "resources/")
