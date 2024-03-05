#ifndef RAYLIB_CPP_INCLUDE_TEXTUREUNMANAGED_HPP_
#define RAYLIB_CPP_INCLUDE_TEXTUREUNMANAGED_HPP_

#include <bit>
#include <string>

#include "./raylib.hpp"
#include "./Image.hpp"
#include "./Vector2.hpp"
#include "./raylib-cpp-utils.hpp"
#ifdef __cpp_exceptions
#include "./RaylibException.hpp"
#endif
#include "./RaylibError.hpp"

namespace raylib {

class Texture;

/**
 * A Texture that is not managed by the C++ RAII.
 *
 * Make sure to Unload() this if needed, otherwise use raylib::Texture.
 *
 * @see raylib::Texture
 */
class TextureUnmanaged {
 public:
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

    /**
     * Creates a texture object based on the given Texture struct data.
     */
    explicit constexpr TextureUnmanaged(const ::Texture& texture) :
            m_data{texture.id, texture.width, texture.height, texture.mipmaps, texture.format} {
        // Nothing.
    }

    constexpr TextureUnmanaged& operator=(const ::Texture& texture) {
        set(texture);
        return *this;
    }

    explicit operator ::Texture() const {
        return m_data;
    }
    [[nodiscard]] ::Texture c_raylib() const & {
        return m_data;
    }

    GETTER(unsigned int, Id, m_data.id)
    GETTER(int, Width, m_data.width)
    GETTER(int, Height, m_data.height)
    GETTER(int, Mipmaps, m_data.mipmaps)
    GETTER(int, Format, m_data.format)

    /**
     * Retrieve the width and height of the texture.
     */
    [[nodiscard]] constexpr raylib::Vector2 GetSize() const {
        return raylib::Vector2{{.x = static_cast<float>(m_data.width), .y = static_cast<float>(m_data.height)}};
    }

    /**
     * Update GPU texture with new data
     */
    TextureUnmanaged& Update(const void *pixels) {
        ::UpdateTexture(m_data, pixels);
        return *this;
    }

    /**
     * Update GPU texture rectangle with new data
     */
    TextureUnmanaged& Update(::Rectangle rec, const void *pixels) {
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
     * Generate GPU mipmaps for a texture
     */
    TextureUnmanaged& GenMipmaps() {
        ::GenTextureMipmaps(&m_data);
        return *this;
    }

    /**
     * Set texture scaling filter mode
     */
    TextureUnmanaged& SetFilter(int filterMode) {
        ::SetTextureFilter(m_data, filterMode);
        return *this;
    }

    /**
     * Set texture wrapping mode
     */
    TextureUnmanaged& SetWrap(int wrapMode) {
        ::SetTextureWrap(m_data, wrapMode);
        return *this;
    }

    /**
     * Draw a Texture2D
     *
     * @see ::DrawTexture()
     */
    void Draw(int posX = 0, int posY = 0, ::Color tint = {255, 255, 255, 255}) const {
        ::DrawTexture(m_data, posX, posY, tint);
    }

    /**
     * Draw a Texture2D with position defined as Vector2
     *
     * @see ::DrawTextureV()
     */
    void Draw(::Vector2 position, ::Color tint = {255, 255, 255, 255}) const {
        ::DrawTextureV(m_data, position, tint);
    }

    /**
     * Draw a Texture2D with extended parameters
     *
     * @see ::DrawTextureEx()
     */
    void Draw(::Vector2 position, float rotation, float scale = 1.0F,
            ::Color tint = {255, 255, 255, 255}) const {
        ::DrawTextureEx(m_data, position, rotation, scale, tint);
    }

    /**
     * Draw a part of a texture defined by a rectangle
     *
     * @see ::DrawTextureRec()
     */
    void Draw(::Rectangle sourceRec, ::Vector2 position = {0, 0},
            ::Color tint = {255, 255, 255, 255}) const {
        ::DrawTextureRec(m_data, sourceRec, position, tint);
    }

    /**
     * Draw a part of a texture defined by a rectangle with 'pro' parameters
     *
     * @see ::DrawTexturePro()
     */
    void Draw(::Rectangle sourceRec, ::Rectangle destRec, ::Vector2 origin = {0, 0},
            float rotation = 0, ::Color tint = {255, 255, 255, 255}) const {
        ::DrawTexturePro(m_data, sourceRec, destRec, origin, rotation, tint);
    }

    /**
     * Draws a texture (or part of it) that stretches or shrinks nicely
     *
     * @see ::DrawTextureNPatch()
     */
    void Draw(::NPatchInfo nPatchInfo, ::Rectangle destRec, ::Vector2 origin = {0, 0},
            float rotation = 0, ::Color tint = {255, 255, 255, 255}) const {
        ::DrawTextureNPatch(m_data, nPatchInfo, destRec, origin, rotation, tint);
    }

    /**
     * Draw a billboard texture
     *
     * @see ::DrawBillboard()
     */
    void DrawBillboard(const ::Camera& camera,
            ::Vector3 position, float size,
            ::Color tint = WHITE) const {
        ::DrawBillboard(camera, m_data, position, size, tint);
    }

    /**
     * Draw a billboard texture defined by source
     *
     * @see ::DrawBillboardRec()
     */
    void DrawBillboard(const ::Camera& camera,
            ::Rectangle source, ::Vector3 position, ::Vector2 size,
            ::Color tint = WHITE) const {
        DrawBillboardRec(camera, m_data, source, position, size, tint);
    }

    /**
     * Draw a billboard texture defined by source and rotation
     *
     * @see ::DrawBillboardPro()
     */
    void DrawBillboard(const ::Camera& camera,
            ::Rectangle source, Vector3 position,
            ::Vector3 up, Vector2 size, Vector2 origin, float rotation = 0.0F,
            ::Color tint = WHITE) const {
        DrawBillboardPro(camera, m_data, source, position, up, size, origin, rotation, tint);
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
    TextureUnmanaged& SetShapes(const ::Rectangle& source) {
        ::SetShapesTexture(m_data, source);
        return *this;
    }

    /**
     * Set shader uniform value for texture (sampler2d)
     */
    TextureUnmanaged& SetShaderValue(const ::Shader& shader, int locIndex) {
        ::SetShaderValueTexture(shader, locIndex, m_data);
        return *this;
    }

    /**
     * Determines whether or not the Texture has been loaded and is ready.
     *
     * @return True or false depending on whether the Texture has data.
     */
    [[nodiscard]] bool IsReady() const {
        return m_data.id != 0;
    }

 protected:
    constexpr void set(const ::Texture& texture) {
        m_data.id = texture.id;
        m_data.width = texture.width;
        m_data.height = texture.height;
        m_data.mipmaps = texture.mipmaps;
        m_data.format = texture.format;
    }

    ::Texture m_data;

    friend class Texture;
};

// Create the TextureUnmanaged aliases.
using Texture2DUnmanaged = TextureUnmanaged;
using TextureCubemapUnmanaged = TextureUnmanaged;

}  // namespace raylib

using RTextureUnmanaged = raylib::TextureUnmanaged;
using RTexture2DUnmanaged = raylib::Texture2DUnmanaged;
using RTextureCubemapUnmanaged = raylib::TextureCubemapUnmanaged;

#endif  // RAYLIB_CPP_INCLUDE_TEXTUREUNMANAGED_HPP_
