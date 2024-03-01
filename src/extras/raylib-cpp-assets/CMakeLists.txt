# Based on https://github.com/JeffM2501/raylibExtras/tree/index/rlExtrasCPP RLAssets
# https://github.com/JeffM2501/raylibExtras

option(DISABLE_RAYLIB_ASSETS_EXCEPTIONS "Disable exception handling for raylib-assets/miniz-cpp" OFF)

add_library(raylib-cpp-assets STATIC)
add_library(raylib-cpp::assets ALIAS raylib-cpp-assets)
set(RAYLIB_ASSETS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/include/RLAssets.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/zip_file.hpp
          ${CMAKE_CURRENT_SOURCE_DIR}/src/RLAssets.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/src/zip_file.cpp)
if(WIN32)
  list(APPEND RAYLIB_ASSETS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/RLAssets_Windows.cpp)
else()
  if(UNIX)
    if(APPLE)
      list(APPEND RAYLIB_ASSETS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/RLAssets_Apple.cpp)
    else()
      list(APPEND RAYLIB_ASSETS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/RLAssets_Linux.cpp)
    endif()
  endif()
endif()
target_sources(raylib-cpp-assets
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/RLAssets.hpp>
        PRIVATE ${RAYLIB_ASSETS_SOURCES})
target_include_directories(raylib-cpp-assets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(raylib-cpp-assets PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_system_library(raylib-cpp-assets PUBLIC raylib)
target_compile_features(raylib-cpp-assets PUBLIC cxx_std_20)

if(DISABLE_RAYLIB_ASSETS_EXCEPTIONS)
  target_compile_definitions(raylib-cpp-assets PUBLIC MINIZ_NO_EXCEPTIONS)
endif()
