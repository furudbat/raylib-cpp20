#ifndef RAYLIB_CPP_INCLUDE_FONT_HPP_
#define RAYLIB_CPP_INCLUDE_FONT_HPP_

#include <string>
#include <span>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "./TextureUnmanaged.hpp"
#ifdef __cpp_exceptions
#include "./RaylibException.hpp"
#endif
#include "./RaylibError.hpp"

namespace raylib {
/**
 * Font type, includes texture and charSet array data
 */
class Font {
 public:
    inline static constexpr ::Color DefaultTintColor = WHITE;

    constexpr Font(int _baseSize,
            int _glyphCount,
            int _glyphPadding,
            owner<::Texture2D> _texture,
            owner<::Rectangle*> _recs = nullptr,
            owner<::GlyphInfo*> _glyphs = nullptr) : m_data{_baseSize, _glyphCount, _glyphPadding, _texture, _recs, _glyphs} {
        // Nothing.
    }

    /**
     * Retrieves the default Font.
     */
    Font() noexcept {
        set(::GetFontDefault());
    }

    explicit constexpr Font(owner<const ::Font&> font) = delete;
    explicit constexpr Font(owner<::Font&&> font) noexcept {
        set(font);

        font.baseSize = 0;
        font.glyphCount = 0;
        font.glyphPadding = 0;
        font.texture = NullTexture;
        font.recs = nullptr;
        font.glyphs = nullptr;
    }

    /**
     * Loads a Font from the given file.
     *
     * @param fileName The file name of the font to load.
     *
     * @throws raylib::RaylibException Throws if the given font failed to initialize.
     */
    explicit Font(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        Load(fileName);
    }

    /**
     * Loads a Font from the given file, with generation parameters.
     *
     * @param fileName The file name of the font to load.
     *
     * @throws raylib::RaylibException Throws if the given font failed to initialize.
     *
     * @see ::LoadFontEx
     */
    Font(const std::filesystem::path& fileName, int fontSize, std::span<int> codepoints = {}) RAYLIB_CPP_THROWS {
        Load(fileName, fontSize, codepoints);
    }
    Font(const std::filesystem::path& fileName, int fontSize, int* fontChars, int charCount) RAYLIB_CPP_THROWS {
        Load(fileName, fontSize, fontChars, charCount);
    }

    /**
     * Loads a Font from the given image with a color key.
     *
     * @param image The image to load the fond from.
     *
     * @throws raylib::RaylibException Throws if the given font failed to initialize.
     *
     * @see ::LoadFontFromImage()
     */
    Font(const ::Image& image, ::Color key, int firstChar) RAYLIB_CPP_THROWS {
        Load(image, key, firstChar);
    }
    Font(const raylib::Image& image, ::Color key, int firstChar) RAYLIB_CPP_THROWS {
        Load(image.c_raylib(), key, firstChar);
    }

    /**
     * Loads a font from memory, based on the given file type and file data.
     *
     * @throws raylib::RaylibException Throws if the given font failed to initialize.
     *
     * @see ::LoadFontFromMemory()
     */
    Font(const std::string& fileType, std::span<const unsigned char> fileData, int fontSize,
         std::span<int> codepoints) RAYLIB_CPP_THROWS {
        Load(fileType, fileData, fontSize, codepoints);
    }
    Font(const std::string& fileType, std::span<const unsigned char> fileData, int fontSize,
         int* codepoints, int codepointCount) RAYLIB_CPP_THROWS {
        Load(fileType, fileData, fontSize, codepoints, codepointCount);
    }

    Font& operator=(owner<const ::Font&> font) = delete;
    Font& operator=(owner<::Font&&> font) noexcept {
        Unload();
        set(font);

        font.baseSize = 0;
        font.glyphCount = 0;
        font.glyphPadding = 0;
        font.texture = NullTexture;
        font.recs = nullptr;
        font.glyphs = nullptr;

        return *this;
    }

    constexpr Font& operator=(const Font&) = delete;
    Font& operator=(Font&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other.m_data);

        other.m_data.baseSize = 0;
        other.m_data.glyphCount = 0;
        other.m_data.glyphPadding = 0;
        other.m_data.texture = NullTexture;
        other.m_data.recs = nullptr;
        other.m_data.glyphs = nullptr;

        return *this;
    }

    explicit operator ::Font() const noexcept {
        return m_data;
    }
    [[nodiscard]] ::Font c_raylib() const & noexcept {
        return m_data;
    }

    constexpr Font(const Font&) = delete;
    constexpr Font(Font&& other) noexcept {
        set(other.m_data);

        other.m_data.baseSize = 0;
        other.m_data.glyphCount = 0;
        other.m_data.glyphPadding = 0;
        other.m_data.texture = NullTexture;
        other.m_data.recs = nullptr;
        other.m_data.glyphs = nullptr;
    }
    ~Font() noexcept {
        Unload();
    }

    void Unload() noexcept {
        if (m_data.texture.id != 0 && m_data.texture.id != GetFontDefault().texture.id) {
            ::UnloadFont(m_data);
            m_data.texture.id = 0;
            m_data.glyphs = nullptr;
            m_data.glyphCount = 0;
            m_data.recs = nullptr;
        }
    }

    GETTERSETTER(int, BaseSize, m_data.baseSize)
    GETTERSETTER(int, GlyphCount, m_data.glyphCount)
    GETTERSETTER(int, GlyphPadding, m_data.glyphPadding)
    SPAN_GETTER(::Rectangle, Recs, m_data.recs, m_data.glyphCount)
    SPAN_GETTER(::GlyphInfo, Glyphs, m_data.glyphs, m_data.glyphCount)

    /**
     * Get the texture atlas containing the glyphs.
     */
    [[nodiscard]] TextureUnmanaged GetTexture() const noexcept {
        return TextureUnmanaged{m_data.texture};
    }

    /**
     * Set the texture atlas containing the glyphs.
     */
    constexpr void SetTexture(const ::Texture& newTexture) = delete;
    constexpr void SetTexture(::Texture&& newTexture) noexcept {
        m_data.texture = newTexture;
        newTexture = NullTexture;
    }

    Font& TextureGenMipmaps() noexcept {
        ::GenTextureMipmaps(&m_data.texture);

        return *this;
    }

    /**
     * Loads a font from a given file.
     *
     * @param fileName The filename of the font to load.
     *
     * @throws raylib::RaylibException Throws if the given font failed to initialize.
     *
     * @see ::LoadFont()
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        set(::LoadFont(fileName.c_str()));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Font with from file: " + fileName.string()));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Loads a font from a given file with generation parameters.
     *
     * @param fileName The filename of the font to load.
     * @param fontSize The desired size of the font.
     *
     * @throws raylib::RaylibException Throws if the given font failed to initialize.
     *
     * @see ::LoadFontEx()
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const std::filesystem::path& fileName, int fontSize, std::span<int> codepoints = {}) RAYLIB_CPP_THROWS {
        set(::LoadFontEx(fileName.c_str(), fontSize, codepoints.data(), static_cast<int>(codepoints.size())));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Font with from file with font size: " + fileName.string()));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const std::filesystem::path& fileName, int fontSize, int* codepoints, int codepointCount) RAYLIB_CPP_THROWS {
        set(::LoadFontEx(fileName.c_str(), fontSize, codepoints, codepointCount));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Font with from file with font size: " + fileName.string()));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const ::Image& image, ::Color key, int firstChar) RAYLIB_CPP_THROWS {
        set(::LoadFontFromImage(image, key, firstChar));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Font with from image"));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const std::string& fileType, std::span<const unsigned char> fileData, int fontSize,
                                          std::span<int> codepoints) RAYLIB_CPP_THROWS {
        set(::LoadFontFromMemory(fileType.c_str(), fileData.data(), static_cast<int>(fileData.size()), fontSize, codepoints.data(),
                                 static_cast<int>(codepoints.size())));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Font " + fileType + " with from file data"));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const std::string& fileType, std::span<const unsigned char> fileData, int fontSize,
                                          int* codepoints, int codepointCount) RAYLIB_CPP_THROWS {
        set(::LoadFontFromMemory(fileType.c_str(), fileData.data(), static_cast<int>(fileData.size()), fontSize, codepoints,
                                 codepointCount));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Font " + fileType + " with from file data"));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Returns if the font is ready to be used.
     */
    [[nodiscard]] bool IsReady() const noexcept {
        return ::IsFontReady(m_data);
    }

    /**
     * Draw text using font and additional parameters.
     */
    void DrawText(const char* text, ::Vector2 position, float fontSize,
                  float spacing, ::Color tint = DefaultTintColor) const noexcept {
        ::DrawTextEx(m_data, text, position,  fontSize,  spacing,  tint);
    }

    /**
     * Draw text using font and additional parameters.
     */
    void DrawText(const std::string& text, ::Vector2 position, float fontSize,
                  float spacing, ::Color tint = DefaultTintColor) const {
        ::DrawTextEx(m_data, text.c_str(), position,  fontSize,  spacing,  tint);
    }
    void DrawTextWithBaseSize(const std::string& text, ::Vector2 position,
                              float spacing, ::Color tint = DefaultTintColor) const {
        DrawText(text, position, static_cast<float>(GetBaseSize()), spacing, tint);
    }

    /**
     * Draw text using font and additional parameters.
     */
    void DrawText(const char* text, int posX, int posY, float fontSize,
                  float spacing, ::Color tint = DefaultTintColor) const noexcept {
        ::DrawTextEx(m_data, text,
            { .x = static_cast<float>(posX), .y = static_cast<float>(posY) },
            fontSize, spacing, tint);
    }

    /**
     * Draw text using font and additional parameters.
     */
    void DrawText(const std::string& text, int posX, int posY, float fontSize,
                  float spacing, ::Color tint = DefaultTintColor) const {
        ::DrawTextEx(m_data, text.c_str(),
            { .x = static_cast<float>(posX), .y = static_cast<float>(posY) },
            fontSize, spacing, tint);
    }
    void DrawTextWithBaseSize(const std::string& text, int posX, int posY,
                  float spacing, ::Color tint = DefaultTintColor) const {
        DrawText(text, posX, posY, static_cast<float>(GetBaseSize()), spacing, tint);
    }

    void DrawText(
            const char* text,
            ::Vector2 position,
            ::Vector2 origin,
            float rotation,
            float fontSize,
            float spacing,
            ::Color tint = DefaultTintColor) const noexcept {
        ::DrawTextPro(m_data, text,
            position, origin,
            rotation, fontSize,
            spacing, tint);
    }

    void DrawText(
            const std::string& text,
            ::Vector2 position,
            ::Vector2 origin,
            float rotation,
            float fontSize,
            float spacing,
            ::Color tint = DefaultTintColor) const {
        ::DrawTextPro(m_data, text.c_str(),
            position, origin,
            rotation, fontSize,
            spacing, tint);
    }

    /**
     * Draw one character (codepoint)
     */
    void DrawText(int codepoint,
            ::Vector2 position,
            float fontSize,
            ::Color tint = DefaultTintColor) const noexcept {
        ::DrawTextCodepoint(m_data, codepoint, position, fontSize, tint);
    }

    /**
     * Draw multiple character (codepoint)
     */
    void DrawText(std::span<const int> codepoints,
            ::Vector2 position,
            float fontSize, float spacing,
            ::Color tint = WHITE) const noexcept {
        ::DrawTextCodepoints(m_data,
            codepoints.data(), static_cast<int>(codepoints.size()),
            position, fontSize,
            spacing, tint);
    }

    /**
     * Measure string size for Font
     */
    Vector2 MeasureText(const char* text, float fontSize, float spacing) const noexcept {
        return ::MeasureTextEx(m_data, text, fontSize, spacing);
    }

    /**
     * Measure string size for Font
     */
    [[nodiscard]] Vector2 MeasureText(const std::string& text, float fontSize, float spacing) const {
        return ::MeasureTextEx(m_data, text.c_str(), fontSize, spacing);
    }
    [[nodiscard]] Vector2 MeasureTextWithBaseSize(const std::string& text, float spacing) const {
        return ::MeasureTextEx(m_data, text.c_str(), static_cast<float>(GetBaseSize()), spacing);
    }

    /**
     * Get index position for a unicode character on font
     */
    [[nodiscard]] int GetGlyphIndex(int character) const noexcept {
        return ::GetGlyphIndex(m_data, character);
    }

    /**
     * Create an image from text (custom sprite font)
     */
    raylib::Image ImageText(const char* text, float fontSize,
                      float spacing, ::Color tint) const {
        return raylib::Image{::ImageTextEx(m_data, text, fontSize, spacing, tint)};
    }

    /**
     * Create an image from text (custom sprite font)
     */
    [[nodiscard]] raylib::Image ImageText(const std::string& text, float fontSize,
                                    float spacing, ::Color tint) const {
        return raylib::Image{::ImageTextEx(m_data, text.c_str(), fontSize, spacing, tint)};
    }

 protected:
    constexpr void set(const ::Font& font) noexcept {
        m_data.baseSize = font.baseSize;
        m_data.glyphCount = font.glyphCount;
        m_data.glyphPadding = font.glyphPadding;
        m_data.texture = font.texture;
        m_data.recs = font.recs;
        m_data.glyphs = font.glyphs;
    }

    ::Font m_data;
};
}  // namespace raylib

using RFont = raylib::Font;

#endif  // RAYLIB_CPP_INCLUDE_FONT_HPP_
