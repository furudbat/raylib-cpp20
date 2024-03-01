option(BUILD_RAYLIB_CPP_EXTRAS "Enable the raylib-cpp extra libs" OFF)
if(BUILD_RAYLIB_CPP_EXTRAS)
    add_subdirectory(extras)
endif()
