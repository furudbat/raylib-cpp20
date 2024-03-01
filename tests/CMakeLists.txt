CPMAddPackage("gh:catchorg/Catch2@3.5.2")
#find_package(Catch2 CONFIG REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)

add_library(raylib_cpp_test_helper INTERFACE)
target_include_directories(raylib_cpp_test_helper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)
target_compile_features(raylib_cpp_test_helper INTERFACE cxx_std_20)

add_subdirectory(unit)

option(BUILD_RAYLIB_CPP_INTEGRATION_TESTS "Enable the integration tests (similar to examples)" OFF)
if(BUILD_RAYLIB_CPP_INTEGRATION_TESTS)
    add_subdirectory(integration)
endif()
