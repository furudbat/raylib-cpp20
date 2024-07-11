#ifndef RAYLIB_CPP_INCLUDE_TEXTUREUNMANAGED_HPP_
#define RAYLIB_CPP_INCLUDE_TEXTUREUNMANAGED_HPP_

#include "raylib.hpp"
#include "Image.hpp"
#include "Vector2.hpp"
#include "raylib-cpp-utils.hpp"
#ifdef __cpp_exceptions
#include "RaylibException.hpp"
#endif
#include "RaylibError.hpp"
#include "enums.hpp"

#include <bit>
#include <string>

namespace raylib {

class Texture;
class Model;

/**
 * A Texture that is not managed by C++ RAII.
 *
 * Make sure to Unload() this if needed, otherwise use raylib::Texture.
 *
 * @see raylib::Texture
 */
class TextureUnmanaged {
 public:
    inline static constexpr int DefaultDrawPosX = 0;
    inline static constexpr int DefaultDrawPosY = 0;
    inline static constexpr ::Color DefaultDrawTintColor = WHITE;
    inline static constexpr float DefaultDrawScale = 1.0f;
    inline static constexpr ::Vector2 DefaultDrawPosition = {.x = DefaultDrawPosX, .y = DefaultDrawPosY};
    inline static constexpr ::Vector2 DefaultDrawOrigin = {.x = 0, .y = 0};
    inline static constexpr float DefaultDrawRotation = 0;

    /**
     * Default texture constructor.
     */
    constexpr TextureUnmanaged() : m_data{NullTexture} {
        // Nothing.
    }

    /**
     * Move/Create a texture structure manually.
     */
    constexpr TextureUnmanaged(unsigned int id,
            int width, int height,
            int mipmaps = 1,
            int format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8)
            : m_data{id, width, height, mipmaps, format} {
        // Nothing.
    }
    constexpr TextureUnmanaged(unsigned int id,
                      int width, int height,
                      int mipmaps,
                      PixelFormat format)
            : m_data{id, width, height, mipmaps, static_cast<int>(format)} {
        // Nothing.
    }

    /**
     * Creates a texture object based on the given Texture struct data.
     */
    explicit constexpr TextureUnmanaged(const ::Texture& texture) :
            m_data{texture.id, texture.width, texture.height, texture.mipmaps, texture.format} {
        // Nothing.
    }

    constexpr TextureUnmanaged& operator=(const ::Texture& texture) noexcept {
        set(texture);
        return *this;
    }

    explicit operator ::Texture() const noexcept {
        return m_data;
    }
    [[nodiscard]] ::Texture c_raylib() const & noexcept {
        return m_data;
    }

    GETTER(unsigned int, Id, m_data.id)
    GETTER(int, Width, m_data.width)
    GETTER(int, Height, m_data.height)
    GETTER(int, Mipmaps, m_data.mipmaps)
    GETTER(PixelFormat, Format, static_cast<PixelFormat>(m_data.format))
    GETTER(int, FormatC, m_data.format)

    constexpr float GetWidthF() const { return static_cast<float>(m_data.width); }
    constexpr float GetHeightF() const { return static_cast<float>(m_data.height); }


    /**
     * Retrieve the width and height of the texture.
     */
    [[nodiscard]] constexpr raylib::Vector2 GetSize() const noexcept {
        return raylib::Vector2{{.x = static_cast<float>(m_data.width), .y = static_cast<float>(m_data.height)}};
    }

    /**
     * Update GPU texture with new data
     */
    template<typename T>
    /// @TODO: make T integral (?) ... short, unsigned char, ... ???
    TextureUnmanaged& Update(const T *pixels) {
        ::UpdateTexture(m_data, pixels);
        return *this;
    }

    /**
     * Update GPU texture rectangle with new data
     */
    template<typename T>
    /// @TODO: make T integral (?) ... short, unsigned char, ... ???
    TextureUnmanaged& Update(::Rectangle rec, const T *pixels) {
        UpdateTextureRec(m_data, rec, pixels);
        return *this;
    }

    /**
     * Get pixel data from GPU texture and return an Image
     */
    [[nodiscard]] raylib::Image GetData() const {
        return raylib::Image{::LoadImageFromTexture(m_data)};
    }

    /**
     * Get pixel data from GPU texture and return an Image
     */
    explicit operator raylib::Image() const {
        return GetData();
    }

    /**
     * Set texture scaling filter mode
     */
    [[deprecated("Use SetFilter(TextureFilter)")]]
    TextureUnmanaged& SetFilter(int filterMode) noexcept {
        ::SetTextureFilter(m_data, filterMode);
        return *this;
    }
    TextureUnmanaged& SetFilter(TextureFilter filterMode) noexcept {
        ::SetTextureFilter(m_data, static_cast<int>(filterMode));
        return *this;
    }

    /**
     * Set texture wrapping mode
     */
    [[deprecated("Use SetWrap(TextureWrap)")]]
    TextureUnmanaged& SetWrap(int wrapMode) noexcept {
        ::SetTextureWrap(m_data, wrapMode);
        return *this;
    }
    TextureUnmanaged& SetWrap(TextureWrap wrapMode) noexcept {
        ::SetTextureWrap(m_data, static_cast<int>(wrapMode));
        return *this;
    }

    /**
     * Draw a Texture2D
     *
     * @see ::DrawTexture()
     */
    void Draw(int posX = DefaultDrawPosX, int posY = DefaultDrawPosY, ::Color tint = DefaultDrawTintColor) const noexcept {
        ::DrawTexture(m_data, posX, posY, tint);
    }

    /**
     * Draw a Texture2D with position defined as Vector2
     *
     * @see ::DrawTextureV()
     */
    void Draw(::Vector2 position, ::Color tint = DefaultDrawTintColor) const noexcept {
        ::DrawTextureV(m_data, position, tint);
    }

    /**
     * Draw a Texture2D with extended parameters
     *
     * @see ::DrawTextureEx()
     */
    void Draw(::Vector2 position, float rotation, float scale = DefaultDrawScale,
            ::Color tint = DefaultDrawTintColor) const noexcept {
        ::DrawTextureEx(m_data, position, rotation, scale, tint);
    }

    /**
     * Draw a part of a texture defined by a rectangle
     *
     * @see ::DrawTextureRec()
     */
    void Draw(::Rectangle sourceRec, ::Vector2 position = DefaultDrawPosition,
            ::Color tint = DefaultDrawTintColor) const noexcept {
        ::DrawTextureRec(m_data, sourceRec, position, tint);
    }

    /**
     * Draw a part of a texture defined by a rectangle with 'pro' parameters
     *
     * @see ::DrawTexturePro()
     */
    void Draw(::Rectangle sourceRec, ::Rectangle destRec, ::Vector2 origin = DefaultDrawOrigin,
            float rotation = DefaultDrawRotation, ::Color tint = DefaultDrawTintColor) const noexcept {
        ::DrawTexturePro(m_data, sourceRec, destRec, origin, rotation, tint);
    }

    /**
     * Draws a texture (or part of it) that stretches or shrinks nicely
     *
     * @see ::DrawTextureNPatch()
     */
    void Draw(::NPatchInfo nPatchInfo, ::Rectangle destRec, ::Vector2 origin = DefaultDrawOrigin,
            float rotation = DefaultDrawRotation, ::Color tint = DefaultDrawTintColor) const noexcept {
        ::DrawTextureNPatch(m_data, nPatchInfo, destRec, origin, rotation, tint);
    }

    /**
     * Draw a billboard texture
     *
     * @see ::DrawBillboard()
     */
    void DrawBillboard(const ::Camera& camera,
            ::Vector3 position, float size,
            ::Color tint = DefaultDrawTintColor) const noexcept {
        ::DrawBillboard(camera, m_data, position, size, tint);
    }

    /**
     * Draw a billboard texture defined by source
     *
     * @see ::DrawBillboardRec()
     */
    void DrawBillboard(const ::Camera& camera,
            ::Rectangle source, ::Vector3 position, ::Vector2 size,
            ::Color tint = DefaultDrawTintColor) const noexcept {
        ::DrawBillboardRec(camera, m_data, source, position, size, tint);
    }

    /**
     * Draw a billboard texture defined by source and rotation
     *
     * @see ::DrawBillboardPro()
     */
    void DrawBillboard(const ::Camera& camera,
            ::Rectangle source, Vector3 position,
            ::Vector3 up, Vector2 size, Vector2 origin, float rotation = DefaultDrawRotation,
            ::Color tint = DefaultDrawTintColor) const noexcept {
        ::DrawBillboardPro(camera, m_data, source, position, up, size, origin, rotation, tint);
    }

    /**
     * Set texture for a material map type (MAP_DIFFUSE, MAP_SPECULAR...)
     */
    /*
    TextureUnmanaged& SetMaterial(::Material &material, int mapType = MATERIAL_MAP_NORMAL) {
        ::SetMaterialTexture(&material, mapType, *this);
        return *this;
    }
    */

    /**
     * Set texture and rectangle to be used on shapes drawing.
     */
    TextureUnmanaged& SetShapes(const ::Rectangle& source) noexcept {
        ::SetShapesTexture(m_data, source);
        return *this;
    }

    /**
     * Set shader uniform value for texture (sampler2d)
     */
    TextureUnmanaged& SetShaderValue(const ::Shader& shader, int locIndex) noexcept {
        ::SetShaderValueTexture(shader, locIndex, m_data);
        return *this;
    }

    /**
     * Determines whether or not the Texture has been loaded and is ready.
     *
     * @return True or false depending on whether the Texture has data.
     */
    [[nodiscard]] bool IsReady() const noexcept {
        return m_data.id != 0;
    }

 protected:
    constexpr void set(const ::Texture& texture) noexcept {
        m_data.id = texture.id;
        m_data.width = texture.width;
        m_data.height = texture.height;
        m_data.mipmaps = texture.mipmaps;
        m_data.format = texture.format;
    }

    ::Texture m_data;

    friend class Texture;
    friend class Model;
};

// Create the TextureUnmanaged aliases.
using Texture2DUnmanaged = TextureUnmanaged;
using TextureCubemapUnmanaged = TextureUnmanaged;

}  // namespace raylib

using RTextureUnmanaged = raylib::TextureUnmanaged;
using RTexture2DUnmanaged = raylib::Texture2DUnmanaged;
using RTextureCubemapUnmanaged = raylib::TextureCubemapUnmanaged;

#endif  // RAYLIB_CPP_INCLUDE_TEXTUREUNMANAGED_HPP_
