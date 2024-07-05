target_compile_options(raylib_cpp_project_options INTERFACE $<$<C_COMPILER_ID:MSVC>:/external:anglebrackets /external:W0>)
target_compile_options(raylib_cpp_project_options INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/external:anglebrackets /external:W0>)

add_library(raylib_cpp_project_options_no_exceptions INTERFACE)
target_link_libraries(raylib_cpp_project_options_no_exceptions INTERFACE raylib_cpp_project_options)
target_compile_options(raylib_cpp_project_options_no_exceptions INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/EHs-c- /D_HAS_EXCEPTIONS=0>)
target_compile_options(raylib_cpp_project_options_no_exceptions INTERFACE $<$<CXX_COMPILER_ID:Clang,GNU>:-fno-exceptions -fno-unwind-tables>)

add_library(raylib_cpp_project_options_no_rtti INTERFACE)
target_link_libraries(raylib_cpp_project_options_no_rtti INTERFACE raylib_cpp_project_options)
target_compile_options(raylib_cpp_project_options_no_rtti INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/GR->)
target_compile_options(raylib_cpp_project_options_no_rtti INTERFACE $<$<CXX_COMPILER_ID:Clang,GNU>:-fno-rtti>)

add_library(raylib_cpp_project_options_no_stdlib INTERFACE)
target_link_libraries(raylib_cpp_project_options_no_stdlib INTERFACE project_options)
target_compile_definitions(raylib_cpp_project_options_no_stdlib INTERFACE NOSTDLIB)
target_compile_options(raylib_cpp_project_options_no_stdlib INTERFACE $<$<CXX_COMPILER_ID:Clang,GNU>:-nostdlib>)
