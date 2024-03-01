cmake_minimum_required(VERSION 3.21...3.24)

# Not ideal to use this global variable, but necessary to make sure that tooling and projects use the same version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

# strongly encouraged to enable this globally to avoid conflicts between -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for
# example when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_EXTENSIONS ON)

# Add project_options https://github.com/aminya/project_options
include(cmake/CPM.cmake)
cpmaddpackage(
        NAME
        project_options
        GITHUB_REPOSITORY
        aminya/project_options
        VERSION           0.35.0
        #GIT_TAG
        #main
        DOWNLOAD_ONLY)
if(project_options_ADDED)
    include(${project_options_SOURCE_DIR}/Index.cmake)
endif()

#option(ENABLE_CROSS_COMPILING "Detect cross compiler and setup toolchain" OFF)
#if(ENABLE_CROSS_COMPILING)
#    enable_cross_compiler()
#endif()
#run_vcpkg()
## Use CPM for dependencies

# Project
project (raylib_cpp
    VERSION 5.0.1
    DESCRIPTION "raylib-cpp C++ Object Oriented Wrapper for raylib"
    HOMEPAGE_URL "https://github.com/robloach/raylib-cpp"
    LANGUAGES C CXX
)

# include(cmake/PreventInSourceBuilds.cmake)
# Options
include(cmake/CMakeOptions.cmake)
get_property(BUILDING_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(BUILDING_MULTI_CONFIG)
    if(NOT CMAKE_BUILD_TYPE)
        # Make sure that all supported configuration types have their associated conan packages available. You can reduce this list to only the
        # configuration types you use, but only if one is not forced-set on the command line for VS
        message(TRACE "Setting up multi-config build types")
        set(CMAKE_CONFIGURATION_TYPES
                Debug Release RelWithDebInfo MinSizeRel
                CACHE STRING "Enabled build types" FORCE)
    else()
        message(TRACE "User chose a specific build type, so we are using that")
        set(CMAKE_CONFIGURATION_TYPES
                ${CMAKE_BUILD_TYPE}
                CACHE STRING "Enabled build types" FORCE)
    endif()
endif()
include(cmake/ProjectOptions.cmake)
include(cmake/MoreProjectOptions.cmake)

# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info
include(cmake/CPM.cmake)
# PackageProject.cmake will be used to make our target installable https://github.com/TheLartians/PackageProject.cmake
cpmaddpackage("gh:TheLartians/PackageProject.cmake@1.11.2")
# https://github.com/TheLartians/Format.cmake
cpmaddpackage(
        NAME
        Format.cmake
        VERSION
        1.8.1
        GITHUB_REPOSITORY
        TheLartians/Format.cmake
        OPTIONS
        # set to yes skip cmake formatting
        "FORMAT_SKIP_CMAKE NO"
        # set to yes skip clang formatting
        "FORMAT_SKIP_CLANG NO"
        # path to exclude (optional, supports regular expressions)
        "CMAKE_FORMAT_EXCLUDE libs|vendor|docs")
# @TODO: write .clang-format file

# Include Directory
add_subdirectory(include)

if (BUILD_RAYLIB_CPP_EXAMPLES OR BUILD_RAYLIB_CPP_TESTS OR BUILD_RAYLIB_CPP_EXTRAS)
    add_subdirectory(libs)
endif()
add_subdirectory(src)

# Examples
if(BUILD_RAYLIB_CPP_EXAMPLES)
    add_subdirectory(examples)
endif()
# Testing
if (BUILD_RAYLIB_CPP_TESTS)
    set(BUILD_RAYLIB_CPP_STATIC ON)

    include(CTest)
    enable_testing()
    if(BUILD_TESTING)
        set(CTEST_CUSTOM_TESTS_IGNORE
                pkg-config--static
        )
        add_subdirectory(tests)
    endif()
endif()