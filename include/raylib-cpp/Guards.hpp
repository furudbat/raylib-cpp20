/**
 * C++ wrapper Begin()/End() (guards) for raylib.
 */
#ifndef RAYLIB_CPP_INCLUDE_GUARDS_HPP_
#define RAYLIB_CPP_INCLUDE_GUARDS_HPP_

#include "raylib.hpp"
#include "AudioDevice.hpp"
#include "AudioStream.hpp"
#include "AutomationEventList.hpp"
#include "Image.hpp"
#include "Texture.hpp"
#include "Wave.hpp"
#include "raylib-cpp-utils.hpp"

#include <filesystem>
#include <string>
#include <vector>

namespace raylib {

class DrawingGuard {
  public:
    DrawingGuard() { ::BeginDrawing(); }
    explicit DrawingGuard(::Color clearBackground) { ::BeginDrawing(); ::ClearBackground(clearBackground); }
    ~DrawingGuard() { ::EndDrawing(); }
};

class Camera3DDrawingGuard {
  public:
    explicit Camera3DDrawingGuard(const ::Camera3D& camera) { ::BeginMode3D(camera); }
    ~Camera3DDrawingGuard() { ::EndMode3D(); }
};

class Camera2DDrawingGuard {
public:
    explicit Camera2DDrawingGuard(const ::Camera2D& camera) { ::BeginMode2D(camera); }
    ~Camera2DDrawingGuard() { ::EndMode2D(); }
};

class RenderTextureDrawingGuard {
  public:
    explicit RenderTextureDrawingGuard(const ::RenderTexture& renderTexture) { ::BeginTextureMode(renderTexture); }
    explicit RenderTextureDrawingGuard(const raylib::RenderTexture& renderTexture) { ::BeginTextureMode(renderTexture.c_raylib()); }
    ~RenderTextureDrawingGuard() noexcept { ::EndTextureMode(); }
};

class ShaderDrawingGuard {
  public:
    explicit ShaderDrawingGuard(const ::Shader& shader) { ::BeginShaderMode(shader); }
    explicit ShaderDrawingGuard(const raylib::Shader& shader) { ::BeginShaderMode(shader.c_raylib()); }
    ~ShaderDrawingGuard() noexcept { ::EndShaderMode(); }
};

}  // namespace raylib

#endif  // RAYLIB_CPP_INCLUDE_GUARDS_HPP_
