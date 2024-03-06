/**
 * C++ wrapper Begin()/End() (guards) for raylib.
 */
#ifndef RAYLIB_CPP_INCLUDE_GUARDS_HPP_
#define RAYLIB_CPP_INCLUDE_GUARDS_HPP_

#include <filesystem>
#include <string>
#include <vector>

#include "./raylib.hpp"
#include "./AudioDevice.hpp"
#include "./AudioStream.hpp"
#include "./AutomationEventList.hpp"
#include "./Image.hpp"
#include "./Texture.hpp"
#include "./Wave.hpp"
#include "./raylib-cpp-utils.hpp"

namespace raylib {

class DrawingGuard {
  public:
    DrawingGuard() { ::BeginDrawing(); }
    explicit DrawingGuard(::Color clearBackground) { ::BeginDrawing(); ::ClearBackground(clearBackground); }
    ~DrawingGuard() { ::EndDrawing(); }
};

class CameraDrawingGuard {
  public:
    explicit CameraDrawingGuard(const ::Camera3D& camera) { ::BeginMode3D(camera); }
    ~CameraDrawingGuard() { ::EndMode3D(); }
};

class RenderTextureDrawingGuard {
  public:
    explicit RenderTextureDrawingGuard(const ::RenderTexture& renderTexture) { ::BeginTextureMode(renderTexture); }
    explicit RenderTextureDrawingGuard(const raylib::RenderTexture& renderTexture) { ::BeginTextureMode(renderTexture.c_raylib()); }
    ~RenderTextureDrawingGuard() { ::EndTextureMode(); }
};

class ShaderDrawingGuard {
  public:
    explicit ShaderDrawingGuard(const ::Shader& shader) { ::BeginShaderMode(shader); }
    explicit ShaderDrawingGuard(const raylib::Shader& shader) { ::BeginShaderMode(shader.c_raylib()); }
    ~ShaderDrawingGuard() { ::EndShaderMode(); }
};

}  // namespace raylib

#endif  // RAYLIB_CPP_INCLUDE_GUARDS_HPP_
