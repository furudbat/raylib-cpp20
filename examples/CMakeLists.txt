# Get the sources together
set(example_dirs audio core models shaders shapes text textures)
set(example_sources)
set(example_resources)

# Find all examples
foreach(example_dir ${example_dirs})
    file(GLOB sources ${example_dir}/*.cpp)
    list(APPEND example_sources ${sources})

    # Any any resources
    file(GLOB resources ${example_dir}/resources/*)
    list(APPEND example_resources ${resources})
endforeach()

# Compile all examples
foreach(example_source ${example_sources})
    # Create the basename for the example
    get_filename_component(example_name ${example_source} NAME)
    string(REPLACE ".cpp" "${OUTPUT_EXT}" example_name ${example_name})

    # Setup the example
    add_executable(${example_name} ${example_source})
    target_link_libraries(${example_name} PRIVATE raylib_cpp_project_warnings raylib_cpp_project_options)

    # Link raylib and raylib-cpp
    target_link_libraries(${example_name} PUBLIC raylib_cpp)
    target_link_system_libraries(${example_name} PUBLIC raylib)
    target_compile_options(${example_name} PRIVATE -Wno-shadow)

    string(REGEX MATCH ".*/.*/" resources_dir ${example_source})
    string(APPEND resources_dir "resources")
endforeach()

# Multiple Files Example
add_executable("multiple" multiple/main.cpp multiple/Player.cpp)
target_link_libraries("multiple" PUBLIC raylib_cpp raylib)

# Copy all the resources
file(COPY ${example_resources} DESTINATION "resources/")
