add_library(raylib_cpp INTERFACE)

set(RAYLIB_CPP_HEADERS
    raylib-cpp/AudioDevice.hpp
    raylib-cpp/AudioStream.hpp
    raylib-cpp/AutomationEventList.hpp
    raylib-cpp/BoundingBox.hpp
    raylib-cpp/Camera2D.hpp
    raylib-cpp/Camera3D.hpp
    raylib-cpp/Color.hpp
    raylib-cpp/FileData.hpp
    raylib-cpp/FileText.hpp
    raylib-cpp/Font.hpp
    raylib-cpp/Functions.hpp
    raylib-cpp/Gamepad.hpp
    raylib-cpp/Image.hpp
    raylib-cpp/Keyboard.hpp
    raylib-cpp/Material.hpp
    raylib-cpp/Matrix.hpp
    raylib-cpp/Mesh.hpp
    raylib-cpp/MeshUnmanaged.hpp
    raylib-cpp/Model.hpp
    raylib-cpp/ModelAnimation.hpp
    raylib-cpp/Mouse.hpp
    raylib-cpp/Music.hpp
    raylib-cpp/Ray.hpp
    raylib-cpp/RayCollision.hpp
    raylib-cpp/RaylibException.hpp
    raylib-cpp/raylib-cpp-utils.hpp
    raylib-cpp.hpp
    raylib-cpp/raylib.hpp
    raylib-cpp/raymath.hpp
    raylib-cpp/Rectangle.hpp
    raylib-cpp/RenderTexture.hpp
    raylib-cpp/Shader.hpp
    raylib-cpp/ShaderUnmanaged.hpp
    raylib-cpp/Sound.hpp
    raylib-cpp/Text.hpp
    raylib-cpp/Texture.hpp
    raylib-cpp/TextureUnmanaged.hpp
    raylib-cpp/Touch.hpp
    raylib-cpp/Vector2.hpp
    raylib-cpp/Vector3.hpp
    raylib-cpp/Vector4.hpp
    raylib-cpp/VrStereoConfig.hpp
    raylib-cpp/Wave.hpp
    raylib-cpp/Window.hpp
)

# Include Directory
target_include_directories(raylib_cpp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)
target_compile_features(raylib_cpp INTERFACE cxx_std_20)

# Set the header files as install files.
install(FILES
  ${RAYLIB_CPP_HEADERS}
  DESTINATION include
)
