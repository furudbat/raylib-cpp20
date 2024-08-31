add_library(raylib_cpp INTERFACE)

set(RAYLIB_CPP_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/AudioDevice.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/AudioStream.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/AutomationEventList.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/BoundingBox.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Camera2D.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Camera3D.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Color.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/FileData.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/FileText.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Font.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Functions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Gamepad.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Image.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Keyboard.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Material.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Matrix.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Mesh.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/MeshUnmanaged.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Model.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/ModelAnimation.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Mouse.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Music.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Ray.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/RayCollision.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/RaylibException.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/raylib-cpp-utils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/raylib.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/raymath.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Rectangle.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/RenderTexture.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/ShaderUnmanaged.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Shader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Sound.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Text.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Texture.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/TextureUnmanaged.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Touch.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Vector2.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Vector3.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Vector4.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/VrStereoConfig.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Wave.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib-cpp/Window.hpp
)

# Include Directory
target_include_directories(raylib_cpp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)
target_compile_features(raylib_cpp INTERFACE cxx_std_20)

# Set the header files as install files.
install(FILES
  ${RAYLIB_CPP_HEADERS}
  DESTINATION include
)
