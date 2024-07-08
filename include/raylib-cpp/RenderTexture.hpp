#ifndef RAYLIB_CPP_INCLUDE_RENDERTEXTURE_HPP_
#define RAYLIB_CPP_INCLUDE_RENDERTEXTURE_HPP_

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "./TextureUnmanaged.hpp"
#ifdef __cpp_exceptions
#include "./RaylibException.hpp"
#endif
#include "./RaylibError.hpp"

namespace raylib {
/**
 * RenderTexture type, for texture rendering
 */
class RenderTexture {
 public:
    /**
     * Default constructor to build an empty RenderTexture.
     */
    constexpr RenderTexture() : m_data({0, NullTexture, NullTexture}) {}

    constexpr explicit RenderTexture(const ::RenderTexture& renderTexture) = delete;
    constexpr explicit RenderTexture(owner<::RenderTexture&&> renderTexture) noexcept {
        set(renderTexture);

        renderTexture.id = 0;
        renderTexture.texture = NullTexture;
        renderTexture.depth = NullTexture;
    }

    /// Questionable Ownership, where is id coming from
    //constexpr RenderTexture(unsigned int id, const ::Texture& texture, const ::Texture& depth) :
    //    ::RenderTexture{id, texture, depth} {}

    /**
     * Load texture for rendering (framebuffer)
     */
    RenderTexture(int width, int height) {
        set(::LoadRenderTexture(width, height));
    }

    constexpr RenderTexture(const RenderTexture&) = delete;
    constexpr RenderTexture(RenderTexture&& other) noexcept {
        set(other.m_data);

        other.m_data.id = 0;
        other.m_data.texture = NullTexture;
        other.m_data.depth = NullTexture;
    }

    GETTER(unsigned int, Id, m_data.id)

    /**
     * Get the color buffer attachment texture.
     */
    TextureUnmanaged GetTexture() {
        return TextureUnmanaged{m_data.texture};
    }

    ::Texture& GetTextureC() {
        return m_data.texture;
    }
    const ::Texture& GetTextureC() const {
        return m_data.texture;
    }

    void SetTexture(owner<const ::Texture&> newTexture) = delete;
    void SetTexture(owner<::Texture&&> newTexture) noexcept {
        m_data.texture = newTexture;
        newTexture = NullTexture;
    }

    /**
     * Depth buffer attachment texture
     */
    TextureUnmanaged GetDepth() {
        return TextureUnmanaged{m_data.depth};
    }
    void SetDepth(const ::Texture& newDepth) noexcept {
        m_data.depth = newDepth;
    }

    RenderTexture& operator=(const ::RenderTexture& texture) = delete;
    RenderTexture& operator=(::RenderTexture&& texture) noexcept {
        set(texture);

        texture.id = 0;
        texture.texture = NullTexture;
        texture.depth = NullTexture;

        return *this;
    }

    RenderTexture& operator=(const RenderTexture&) = delete;
    RenderTexture& operator=(RenderTexture&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other.m_data);

        other.m_data.id = 0;
        other.m_data.texture = NullTexture;
        other.m_data.depth = NullTexture;

        return *this;
    }

    ~RenderTexture() {
        Unload();
    }

    explicit operator ::RenderTexture() const noexcept {
        return m_data;
    }
    [[nodiscard]] ::RenderTexture c_raylib() const & noexcept {
        return m_data;
    }

    void Unload() noexcept {
        ::UnloadRenderTexture(m_data);
        m_data.id = 0;
        m_data.texture = NullTexture;
    }

    /**
     * Initializes render texture for drawing
     */
    RenderTexture& BeginMode() noexcept {
        ::BeginTextureMode(m_data);
        return *this;
    }

    /**
     * Ends drawing to render texture
     */
    RenderTexture& EndMode() noexcept {
        ::EndTextureMode();
        return *this;
    }

    /**
     * Load texture for rendering (framebuffer)
     */
    static RenderTexture Load(int width, int height) {
        return RenderTexture{::LoadRenderTexture(width, height)};
    }

    /**
     * Retrieves whether or not the render texture is ready.
     */
    [[nodiscard]] bool IsReady() const noexcept {
        return ::IsRenderTextureReady(m_data);
    }

 protected:
    constexpr void set(const ::RenderTexture& renderTexture) noexcept {
        m_data.id = renderTexture.id;
        m_data.texture = renderTexture.texture;
        m_data.depth = renderTexture.depth;
    }

    ::RenderTexture m_data;
};
using RenderTexture2D = RenderTexture;
}  // namespace raylib

using RRenderTexture = raylib::RenderTexture;
using RRenderTexture2D = raylib::RenderTexture2D;

#endif  // RAYLIB_CPP_INCLUDE_RENDERTEXTURE_HPP_
