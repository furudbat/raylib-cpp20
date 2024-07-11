#ifndef RAYLIB_CPP_ENUMS_HPP
#define RAYLIB_CPP_ENUMS_HPP

#include "raylib.hpp"
#include "strong_types.h"

#include <cstdint>

namespace raylib {

// System/Window config flags
// NOTE: Every bit registers one state (use it with bit masks)
// By default all flags are set to 0
enum class ConfigFlags : uint32_t {
    vsyncHint         = 0x00000040,   // Set to try enabling V-Sync on GPU
    FullscreenMode    = 0x00000002,   // Set to run program in fullscreen
    WindowResizable   = 0x00000004,   // Set to allow resizable window
    WindowUndecorated = 0x00000008,   // Set to disable window decoration (frame and buttons)
    WindowHidden      = 0x00000080,   // Set to hide window
    WindowMinimized   = 0x00000200,   // Set to minimize window (iconify)
    WindowMaximized   = 0x00000400,   // Set to maximize window (expanded to monitor)
    WindowUnfocused   = 0x00000800,   // Set to window non focused
    WindowTopmost     = 0x00001000,   // Set to window always on top
    WindowAlwaysRun  = 0x00000100,   // Set to allow windows running while minimized
    WindowTransparent = 0x00000010,   // Set to allow transparent framebuffer
    WindowHighdpi     = 0x00002000,   // Set to support HighDPI
    WindowMousePassthrough = 0x00004000, // Set to support mouse passthrough, only supported when FLAG_WINDOW_UNDECORATED
    BorderlessWindowedMode = 0x00008000, // Set to run program in borderless windowed mode
    Msaa4XHint       = 0x00000020,   // Set to try enabling MSAA 4X
    InterlacedHint    = 0x00010000    // Set to try enabling interlaced video format (for V3D)
};

/*
[[maybe_unused]] RLCPPAPI inline constexpr auto CombineConfigFlags(ConfigFlags... flags) {
    return (static_cast<uint32_t>(flags) | ...);
}
*/

// Trace log level
// NOTE: Organized by priority level
enum class TraceLogLevel {
    All = 0,        // Display all logs
    Trace,          // Trace logging, intended for internal use only
    Debug,          // Debug logging, used for internal debugging, it should be disabled on release builds
    Info,           // Info logging, used for program execution info
    Warning,        // Warning logging, used on recoverable failures
    Error,          // Error logging, used on unrecoverable failures
    Fatal,          // Fatal logging, used to abort program: exit(EXIT_FAILURE)
    None            // Disable logging
};

///@TODO: add Input/Keybaord, ... enums

/// use strong_type instead of enum class ... (can't use enum values with the same name/aliases)
CONVERTIBLE_STRONG_TYPE(MaterialMapIndexT, size_t);
namespace MaterialMapIndex {
    inline constexpr MaterialMapIndexT Albedo {0};            // Albedo material (same as: MATERIAL_MAP_DIFFUSE)
    inline constexpr MaterialMapIndexT Metalness {1};         // Metalness material (same as: MATERIAL_MAP_SPECULAR)
    inline constexpr MaterialMapIndexT Normal {2};            // Normal material
    inline constexpr MaterialMapIndexT Roughness {3};         // Roughness material
    inline constexpr MaterialMapIndexT Occlusion {4};         // Ambient occlusion material
    inline constexpr MaterialMapIndexT Emission {5};          // Emission material
    inline constexpr MaterialMapIndexT Height {6};            // Heightmap material
    inline constexpr MaterialMapIndexT Cubemap {7};           // Cubemap material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    inline constexpr MaterialMapIndexT Irradiance {8};        // Irradiance material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    inline constexpr MaterialMapIndexT PREFILTER {9};         // Prefilter material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    inline constexpr MaterialMapIndexT BRDF {10};             // Brdf material
    inline constexpr MaterialMapIndexT Diffuse = Albedo;
    inline constexpr MaterialMapIndexT Specular = Metalness;
}

CONVERTIBLE_STRONG_TYPE(ShaderLocationIndexT, size_t);
namespace ShaderLocationIndex {
    inline constexpr ShaderLocationIndexT VertexPosition {0}; // Shader location: vertex attribute: position
    inline constexpr ShaderLocationIndexT VertexTexcoord01{1};   // Shader location: vertex attribute: texcoord01
    inline constexpr ShaderLocationIndexT VertexTexcoord02{2};   // Shader location: vertex attribute: texcoord02
    inline constexpr ShaderLocationIndexT VertexNormal{3};       // Shader location: vertex attribute: normal
    inline constexpr ShaderLocationIndexT VertexTangent{4};      // Shader location: vertex attribute: tangent
    inline constexpr ShaderLocationIndexT VertexColor{5};        // Shader location: vertex attribute: color
    inline constexpr ShaderLocationIndexT MatrixMvp{6};          // Shader location: matrix uniform: model-view-projection
    inline constexpr ShaderLocationIndexT MatrixView{7};         // Shader location: matrix uniform: view (camera transform)
    inline constexpr ShaderLocationIndexT MatrixProjection{8};   // Shader location: matrix uniform: projection
    inline constexpr ShaderLocationIndexT MatrixModel{9};        // Shader location: matrix uniform: model (transform)
    inline constexpr ShaderLocationIndexT MatrixNormal{10};       // Shader location: matrix uniform: normal
    inline constexpr ShaderLocationIndexT VectorView{11};         // Shader location: vector uniform: view
    inline constexpr ShaderLocationIndexT ColorDiffuse{12};       // Shader location: vector uniform: diffuse color
    inline constexpr ShaderLocationIndexT ColorSpecular{13};      // Shader location: vector uniform: specular color
    inline constexpr ShaderLocationIndexT ColorAmbient{14};       // Shader location: vector uniform: ambient color
    inline constexpr ShaderLocationIndexT Albedo{15};          // Shader location: sampler2d texture: albedo (same as: MAP_DIFFUSE)
    inline constexpr ShaderLocationIndexT Metalness{16};       // Shader location: sampler2d texture: metalness (same as: MAP_SPECULAR)
    inline constexpr ShaderLocationIndexT Normal{17};          // Shader location: sampler2d texture: normal
    inline constexpr ShaderLocationIndexT Roughness{18};       // Shader location: sampler2d texture: roughness
    inline constexpr ShaderLocationIndexT Occlusion{19};       // Shader location: sampler2d texture: occlusion
    inline constexpr ShaderLocationIndexT Emission{20};        // Shader location: sampler2d texture: emission
    inline constexpr ShaderLocationIndexT Height{21};          // Shader location: sampler2d texture: height
    inline constexpr ShaderLocationIndexT Cubemap{22};         // Shader location: samplerCube texture: cubemap
    inline constexpr ShaderLocationIndexT Irradiance{23};      // Shader location: samplerCube texture: irradiance
    inline constexpr ShaderLocationIndexT Prefilter{24};       // Shader location: samplerCube texture: prefilter
    inline constexpr ShaderLocationIndexT Brdf{25};             // Shader location: sampler2d texture: brdf
    inline constexpr ShaderLocationIndexT Diffuse = Albedo;
    inline constexpr ShaderLocationIndexT Specular = Metalness;
}

// Shader uniform data type
enum class ShaderUniformDataType {
    Float = 0,       // Shader uniform type: float
    Vec2,            // Shader uniform type: vec2 (2 float)
    Vec3,            // Shader uniform type: vec3 (3 float)
    Vec4,            // Shader uniform type: vec4 (4 float)
    Int,             // Shader uniform type: int
    Ivec2,           // Shader uniform type: ivec2 (2 int)
    Ivec3,           // Shader uniform type: ivec3 (3 int)
    Ivec4,           // Shader uniform type: ivec4 (4 int)
    Sampler2D        // Shader uniform type: sampler2d
};

// Shader attribute data types
enum class ShaderAttributeDataType {
    Float = 0,       // Shader attribute type: float
    Vec2,            // Shader attribute type: vec2 (2 float)
    Vec3,            // Shader attribute type: vec3 (3 float)
    Vec4             // Shader attribute type: vec4 (4 float)
};

// Pixel formats
// NOTE: Support depends on OpenGL version and platform
enum class PixelFormat {
    UncompressedGrayscale = 1, // 8 bit per pixel (no alpha)
    UncompressedGrayAlpha,     // 8*2 bpp (2 channels)
    UncompressedR5G6B5,        // 16 bpp
    UncompressedR8G8B8,        // 24 bpp
    UncompressedR5G5B5A1,      // 16 bpp (1 bit alpha)
    UncompressedR4G4B4A4,      // 16 bpp (4 bit alpha)
    UncompressedR8G8B8A8,      // 32 bpp
    UncompressedR32,           // 32 bpp (1 channel - float)
    UncompressedR32G32B32,     // 32*3 bpp (3 channels - float)
    UncompressedR32G32B32A32,  // 32*4 bpp (4 channels - float)
    UncompressedR16,           // 16 bpp (1 channel - half float)
    UncompressedR16G16B16,     // 16*3 bpp (3 channels - half float)
    UncompressedR16G16B16A16,  // 16*4 bpp (4 channels - half float)
    CompressedDxt1Rgb,         // 4 bpp (no alpha)
    CompressedDxt1Rgba,        // 4 bpp (1 bit alpha)
    CompressedDxt3Rgba,        // 8 bpp
    CompressedDxt5Rgba,        // 8 bpp
    CompressedEtc1Rgb,         // 4 bpp
    CompressedEtc2Rgb,         // 4 bpp
    CompressedEtc2EacRgba,     // 8 bpp
    CompressedPvrtRgb,         // 4 bpp
    CompressedPvrtRgba,        // 4 bpp
    CompressedAstc4x4Rgba,     // 8 bpp
    CompressedAstc8x8Rgba      // 2 bpp
};

// Texture parameters: filter mode
// NOTE 1: Filtering considers mipmaps if available in the texture
// NOTE 2: Filter is accordingly set for minification and magnification
enum class TextureFilter {
    Point = 0,               // No filter, just pixel approximation
    Bilinear,                // Linear filtering
    Trilinear,               // Trilinear filtering (linear with mipmaps)
    Anisotropic4x,           // Anisotropic filtering 4x
    Anisotropic8x,           // Anisotropic filtering 8x
    Anisotropic16x           // Anisotropic filtering 16x
};

// Texture parameters: wrap mode
enum class TextureWrap {
    Repeat = 0,              // Repeats texture in tiled mode
    Clamp,                   // Clamps texture to edge pixel in tiled mode
    MirrorRepeat,            // Mirrors and repeats the texture in tiled mode
    MirrorClamp              // Mirrors and clamps to border the texture in tiled mode
};

// Cubemap layouts
enum class CubemapLayout {
    AutoDetect = 0,          // Automatically detect layout type
    LineVertical,            // Layout is defined by a vertical line with faces
    LineHorizontal,          // Layout is defined by a horizontal line with faces
    CrossThreeByFour,        // Layout is defined by a 3x4 cross with cubemap faces
    CrossFourByThree,        // Layout is defined by a 4x3 cross with cubemap faces
    Panorama                 // Layout is defined by a panorama image (equirrectangular map)
};

// Font type, defines generation method
enum class FontType {
    Default = 0,             // Default font generation, anti-aliased
    Bitmap,                  // Bitmap font generation, no anti-aliasing
    Sdf                      // SDF font generation, requires external shader
};

// Color blending modes (pre-defined)
enum class BlendMode {
    Alpha = 0,               // Blend textures considering alpha (default)
    Additive,                // Blend textures adding colors
    Multiplied,              // Blend textures multiplying colors
    AddColors,               // Blend textures adding colors (alternative)
    SubtractColors,          // Blend textures subtracting colors (alternative)
    AlphaPremultiply,        // Blend premultiplied textures considering alpha
    Custom,                  // Blend textures using custom src/dst factors (use rlSetBlendFactors())
    CustomSeparate           // Blend textures using custom rgb/alpha separate src/dst factors (use rlSetBlendFactorsSeparate())
};

// Gesture
// NOTE: Provided as bit-wise flags to enable only desired gestures
enum class Gesture {
    None        = 0,         // No gesture
    Tap         = 1,         // Tap gesture
    DoubleTap   = 2,         // Double tap gesture
    Hold        = 4,         // Hold gesture
    Drag        = 8,         // Drag gesture
    SwipeRight  = 16,        // Swipe right gesture
    SwipeLeft   = 32,        // Swipe left gesture
    SwipeUp     = 64,        // Swipe up gesture
    SwipeDown   = 128,       // Swipe down gesture
    PinchIn     = 256,       // Pinch in gesture
    PinchOut    = 512        // Pinch out gesture
};

// Camera system modes
enum class CameraMode {
    Custom = 0,              // Camera custom, controlled by user (UpdateCamera() does nothing)
    Free,                    // Camera free mode
    Orbital,                 // Camera orbital, around target, zoom supported
    FirstPerson,             // Camera first person
    ThirdPerson              // Camera third person
};

// Camera projection
enum class CameraProjection {
    Perspective = 0,         // Perspective projection
    Orthographic             // Orthographic projection
};

// N-patch layout
enum class NPatchLayout {
    NinePatch = 0,           // Npatch layout: 3x3 tiles
    ThreePatchVertical,      // Npatch layout: 1x3 tiles
    ThreePatchHorizontal     // Npatch layout: 3x1 tiles
};


}

#endif //RAYLIB_CPP_ENUMS_HPP
