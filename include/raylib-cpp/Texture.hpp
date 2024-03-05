#ifndef RAYLIB_CPP_INCLUDE_TEXTURE_HPP_
#define RAYLIB_CPP_INCLUDE_TEXTURE_HPP_

#include "./Image.hpp"
#include "./TextureUnmanaged.hpp"

namespace raylib {

/**
 * Texture type
 *
 * The texture will be unloaded on object destruction. Use raylib::TextureUnmanaged if you're looking to not unload.
 *
 * @see raylib::TextureUnmanaged
 */
class Texture {
 public:
    /**
     * Default texture constructor.
     */
    constexpr Texture() = default;

    /**
     * Explicitly forbid the copy constructor.
     */
    Texture(const Texture&) = delete;

    /**
     * Explicitly forbid copy assignment.
     */
    Texture& operator=(const Texture&) = delete;


    /**
     * Move/Create a texture structure manually.
     */
    constexpr Texture(owner<unsigned int> id,
                               int width, int height,
                               int mipmaps = 1,
                               int format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8)
            : m_texture{id, width, height, mipmaps, format} {
        // Nothing.
    }

    explicit constexpr Texture(owner<const ::Texture&> other) = delete;
    explicit constexpr Texture(owner<::Texture&&> other) {
        m_texture.set(other);

        other.id = 0;
        other.width = 0;
        other.height = 0;
        other.mipmaps = 0;
        other.format = 0;
    }

    /**
     * Move constructor.
     */
    Texture(Texture&& other) {
        m_texture.set(other.m_texture.m_data);

        other.m_texture.m_data.id = 0;
        other.m_texture.m_data.width = 0;
        other.m_texture.m_data.height = 0;
        other.m_texture.m_data.mipmaps = 0;
        other.m_texture.m_data.format = 0;
    }
    /**
     * Move assignment.
     */
    Texture& operator=(Texture&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        m_texture.set(other.m_texture.m_data);

        other.m_texture.m_data.id = 0;
        other.m_texture.m_data.width = 0;
        other.m_texture.m_data.height = 0;
        other.m_texture.m_data.mipmaps = 0;
        other.m_texture.m_data.format = 0;

        return *this;
    }

    /**
     * On destruction, unload the Texture.
     */
    ~Texture() {
        Unload();
    }

    //explicit operator ::Texture() const {
    //    return m_texture.m_data;
    //}
    [[nodiscard]] ::Texture c_raylib() const & {
        return m_texture.c_raylib();
    }

    GETTER(unsigned int, Id, m_texture.m_data.id)
    GETTER(int, Width, m_texture.m_data.width)
    GETTER(int, Height, m_texture.m_data.height)
    GETTER(int, Mipmaps, m_texture.m_data.mipmaps)
    GETTER(int, Format, m_texture.m_data.format)

    /**
     * Creates a texture from the given Image.
     *
     * @throws raylib::RaylibException Throws if failed to create the texture from the given image.
     */
    Texture(const ::Image& image) RAYLIB_CPP_THROWS {
        Load(image);
    }
    Texture(const raylib::Image& image) RAYLIB_CPP_THROWS {
        Load(image);
    }

    /**
     * Load cubemap from image, multiple image cubemap layouts supported.
     *
     * @throws raylib::RaylibException Throws if failed to create the texture from the given cubemap.
     *
     * @see LoadTextureCubemap()
     */
    Texture(const ::Image& image, int layout) RAYLIB_CPP_THROWS {
        Load(image, layout);
    }
    Texture(const raylib::Image& image, int layout) RAYLIB_CPP_THROWS {
        Load(image, layout);
    }

    /**
     * Load texture from file into GPU memory (VRAM)
     *
     * @throws raylib::RaylibException Throws if failed to create the texture from the given file.
     */
    explicit Texture(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        Load(fileName);
    }

    /**
     * Unload texture from GPU memory (VRAM)
     */
    void Unload() {
        // Protect against calling UnloadTexture() twice.
        if (m_texture.m_data.id != 0) {
            ::UnloadTexture(m_texture.m_data);
            m_texture.m_data.id = 0;
        }
    }

    /**
     * Load texture from image data
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const ::Image& image) RAYLIB_CPP_THROWS {
        m_texture.set(::LoadTextureFromImage(image));
        if (!m_texture.IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Texture from Image"));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }
    inline RAYLIB_CPP_EXPECTED_RESULT(void) Load(const raylib::Image& image) RAYLIB_CPP_THROWS {
        return Load(image.c_raylib());
    }

    /**
     * Load cubemap from image, multiple image cubemap layouts supported
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const ::Image& image, int layoutType) RAYLIB_CPP_THROWS {
        m_texture.set(::LoadTextureCubemap(image, layoutType));
        if (!m_texture.IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Texture from Cubemap"));
        }
    }
    inline RAYLIB_CPP_EXPECTED_RESULT(void) Load(const raylib::Image& image, int layoutType) RAYLIB_CPP_THROWS {
        return Load(image.c_raylib(), layoutType);
    }

    /**
     * Load texture from file into GPU memory (VRAM)
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        m_texture.set(::LoadTexture(fileName.c_str()));
        if (!m_texture.IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Texture from file: " + fileName.string()));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    CONST_COMPOSITION_METHODE_CALL(GetSize, m_texture)

    COMPOSITION_METHODE_CALL_RETURN_THIS(Update, m_texture)

    CONST_COMPOSITION_METHODE_CALL(GetData, m_texture)
    /**
     * Get pixel data from GPU texture and return an Image
     */
    explicit operator raylib::Image() const {
        return m_texture.GetData();
    }

    COMPOSITION_METHODE_CALL_RETURN_THIS(GenMipmaps, m_texture)
    COMPOSITION_METHODE_CALL_RETURN_THIS(SetFilter, m_texture)
    COMPOSITION_METHODE_CALL_RETURN_THIS(SetWrap, m_texture)

    CONST_COMPOSITION_METHODE_CALL(Draw, m_texture)
    CONST_COMPOSITION_METHODE_CALL(DrawBillboard, m_texture)

    COMPOSITION_METHODE_CALL_RETURN_THIS(SetShapes, m_texture)
    COMPOSITION_METHODE_CALL_RETURN_THIS(SetShaderValue, m_texture)

    CONST_COMPOSITION_METHODE_CALL(IsReady, m_texture)

private:
    TextureUnmanaged m_texture;
};

// Create the Texture aliases.
using Texture2D = Texture;
using TextureCubemap = Texture;

}  // namespace raylib

using RTexture = raylib::Texture;
using RTexture2D = raylib::Texture2D;
using RTextureCubemap = raylib::TextureCubemap;

#endif  // RAYLIB_CPP_INCLUDE_TEXTURE_HPP_
