#ifndef RAYLIB_CPP_INCLUDE_RENDERTEXTURE_HPP_
#define RAYLIB_CPP_INCLUDE_RENDERTEXTURE_HPP_

#include "raylib.hpp"
#include "raylib-cpp-utils.hpp"
#include "TextureUnmanaged.hpp"
#include "RaylibError.hpp"
#ifdef __cpp_exceptions
#include "RaylibException.hpp"
#endif

#include <rlgl.h>

#include <utility>

namespace raylib {

enum class RenderTextureOptions : uint8_t {
    UnloadRenderTexture,      ///< UnloadRenderTexture (default)
    UnloadFramebuffer,        ///< rlUnloadFramebuffer (Same as UnloadRenderTexture when using LoadRenderTexture(width, height))
};

enum class RenderTextureTextureOptions : uint8_t {
    NoUnload,         ///< Manage Texture on your own (NO UnloadTexture for texture)
    UnloadTexture,    ///< UnloadTexture
};

struct RenderTextureCOptions {
    RenderTextureOptions id {RenderTextureOptions::UnloadRenderTexture};
    RenderTextureTextureOptions texture {RenderTextureTextureOptions::NoUnload};
    RenderTextureTextureOptions depth {RenderTextureTextureOptions::NoUnload};
};

/**
 * RenderTexture type, for texture rendering
 */
class RenderTexture {
 public:
    /**
     * Default constructor to build an empty RenderTexture.
     */
    constexpr RenderTexture() : m_data({0, NullTexture, NullTexture}) {}

    constexpr explicit RenderTexture(unsigned int id, RenderTextureOptions option)
        : m_data({id, NullTexture, NullTexture})
        , m_renderTextureOption(option)
    {}

    constexpr explicit RenderTexture(const ::RenderTexture& renderTexture) = delete;
    constexpr RenderTexture(const ::RenderTexture& renderTexture, RenderTextureCOptions options) = delete;
    constexpr RenderTexture(::RenderTexture&& renderTexture, RenderTextureCOptions options) noexcept : m_renderTextureOption(options.id),
                                                                                                              m_renderTextureTextureOption(options.texture),
                                                                                                              m_renderTextureDepthOption(options.depth) {
        set(renderTexture);

        renderTexture.id = 0;
        renderTexture.texture = NullTexture;
        renderTexture.depth = NullTexture;
    }

    /**
     * Load texture for rendering (framebuffer)
     */
    RenderTexture(int width, int height) {
        set(::LoadRenderTexture(width, height));
        m_renderTextureOption = RenderTextureOptions::UnloadRenderTexture;
    }

    constexpr RenderTexture(const RenderTexture&) = delete;
    constexpr RenderTexture(RenderTexture&& other) noexcept {
        set(other.m_data);

        other.m_data.id = 0;
        other.m_data.texture = NullTexture;
        other.m_data.depth = NullTexture;
    }

    GETTER(unsigned int, Id, m_data.id)
    void SetId(unsigned int id, RenderTextureOptions option) {
        m_data.id = id;
        m_renderTextureOption = option;
    }

    /**
     * Get the color buffer attachment texture.
     */
    [[nodiscard]] TextureUnmanaged GetTexture() const {
        return TextureUnmanaged{m_data.texture};
    }

    [[nodiscard]] ::Texture& GetTextureC() {
        return m_data.texture;
    }
    [[nodiscard]] const ::Texture& GetTextureC() const {
        return m_data.texture;
    }

    void SetTexture(const ::Texture& newTexture) = delete;
    void SetTexture(const ::Texture& newTexture, RenderTextureTextureOptions option) = delete;
    void SetTexture(::Texture&& newTexture, RenderTextureTextureOptions option) noexcept {
        m_data.texture = newTexture;
        m_renderTextureTextureOption = option;
        newTexture = NullTexture;
    }

    /**
     * Depth buffer attachment texture
     */
    [[nodiscard]] TextureUnmanaged GetDepth() const {
        return TextureUnmanaged{m_data.depth};
    }

    void SetDepth(const ::Texture& newTexture) = delete;
    void SetDepth(const ::Texture& newTexture, RenderTextureTextureOptions option) = delete;
    void SetDepth(::Texture&& newTexture, RenderTextureTextureOptions option) noexcept {
        m_data.depth = newTexture;
        m_renderTextureDepthOption = option;
        newTexture = NullTexture;
    }

    [[nodiscard]] ::Texture& GetDepthC() {
        return m_data.depth;
    }
    [[nodiscard]] const ::Texture& GetDepthC() const {
        return m_data.depth;
    }

    RenderTexture& operator=(const ::RenderTexture& texture) = delete;
    RenderTexture& operator=(::RenderTexture&& texture) = delete;

    RenderTexture& operator=(const RenderTexture&) = delete;
    RenderTexture& operator=(RenderTexture&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other.m_data);
        std::exchange(m_renderTextureOption, other.m_renderTextureOption);
        std::exchange(m_renderTextureTextureOption, other.m_renderTextureTextureOption);
        std::exchange(m_renderTextureDepthOption, other.m_renderTextureDepthOption);

        other.m_data.id = 0;
        other.m_data.texture = NullTexture;
        other.m_data.depth = NullTexture;
        other.m_renderTextureOption = RenderTextureOptions::UnloadRenderTexture;
        other.m_renderTextureTextureOption = RenderTextureTextureOptions::NoUnload;
        other.m_renderTextureDepthOption = RenderTextureTextureOptions::NoUnload;

        return *this;
    }

    //GETTERSETTER(RenderTextureOptions, RenderTextureOption, m_renderTextureOption)
    //GETTERSETTER(RenderTextureTextureOptions, RenderTextureTextureOption, m_renderTextureTextureOption)
    //GETTERSETTER(RenderTextureTextureOptions, RenderTextureDepthOption, m_renderTextureDepthOption)

    ~RenderTexture() {
        Unload();
    }

    explicit operator ::RenderTexture() const {
        return m_data;
    }
    [[nodiscard]] ::RenderTexture c_raylib() const & {
        return m_data;
    }

    void Unload() noexcept {
        switch(m_renderTextureTextureOption) {
            case RenderTextureTextureOptions::NoUnload:
                // do nothing
                break;
            case RenderTextureTextureOptions::UnloadTexture:
                rlUnloadTexture(m_data.texture.id);
                break;
        }
        switch(m_renderTextureDepthOption) {
            case RenderTextureTextureOptions::NoUnload:
                // do nothing
                break;
            case RenderTextureTextureOptions::UnloadTexture:
                rlUnloadTexture(m_data.depth.id);
                break;
        }
        switch (m_renderTextureOption) {
            case RenderTextureOptions::UnloadRenderTexture:
                ::UnloadRenderTexture(m_data);
                break;
            case RenderTextureOptions::UnloadFramebuffer:
                rlUnloadFramebuffer(m_data.id);
                break;
        }
        m_data.id = 0;
        m_data.texture = NullTexture;
        m_data.depth = NullTexture;
    }

    /**
     * Initializes render texture for drawing
     */
    RenderTexture& BeginMode() {
        ::BeginTextureMode(m_data);
        return *this;
    }

    /**
     * Ends drawing to render texture
     */
    RenderTexture& EndMode() {
        ::EndTextureMode();
        return *this;
    }

    /**
     * Load texture for rendering (framebuffer)
     */
    static RenderTexture Load(int width, int height) {
        return RenderTexture{::LoadRenderTexture(width, height), RenderTextureCOptions{
            .id = RenderTextureOptions::UnloadRenderTexture,
            .texture = RenderTextureTextureOptions::NoUnload,
            .depth = RenderTextureTextureOptions::NoUnload,
        }};
    }

    /**
     * Retrieves whether or not the render texture is ready.
     */
    [[nodiscard]] bool IsReady() const {
        return ::IsRenderTextureReady(m_data);
    }

    static RenderTexture LoadFramebuffer(int width, int height) {
        return RenderTexture(rlLoadFramebuffer(width, height), RenderTextureOptions::UnloadFramebuffer);
    }

 protected:
    constexpr void set(const ::RenderTexture& renderTexture) noexcept {
        m_data.id = renderTexture.id;
        m_data.texture = renderTexture.texture;
        m_data.depth = renderTexture.depth;
    }

    ::RenderTexture m_data;
    RenderTextureOptions m_renderTextureOption {RenderTextureOptions::UnloadRenderTexture};
    RenderTextureTextureOptions m_renderTextureTextureOption {RenderTextureTextureOptions::NoUnload};
    RenderTextureTextureOptions m_renderTextureDepthOption {RenderTextureTextureOptions::NoUnload};
};
using RenderTexture2D = RenderTexture;
}  // namespace raylib

using RRenderTexture = raylib::RenderTexture;
using RRenderTexture2D = raylib::RenderTexture2D;

#endif  // RAYLIB_CPP_INCLUDE_RENDERTEXTURE_HPP_
