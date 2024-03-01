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
class Font : public ::Font {
 public:
    constexpr Font(int _baseSize,
            int _glyphCount,
            int _glyphPadding,
            owner<::Texture2D> _texture,
            owner<::Rectangle*> _recs = nullptr,
            owner<::GlyphInfo*> _glyphs = nullptr) : ::Font{_baseSize, _glyphCount, _glyphPadding, _texture, _recs, _glyphs} {
        // Nothing.
    }

    /**
     * Retrieves the default Font.
     */
    Font() {
        set(::GetFontDefault());
    }

    constexpr Font(const ::Font& font) = delete;
    constexpr Font(::Font&& font) {
        set(font);

        font.baseSize = 0;
        font.glyphCount = 0;
        font.glyphPadding = 0;
        font.texture = { .id = 0 };
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
    Font(const std::filesystem::path& fileName, int fontSize, int* codepoints, int codepointCount) RAYLIB_CPP_THROWS {
        Load(fileName, fontSize, std::span<int>{codepoints, static_cast<size_t>(codepointCount)});
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
        Load(fileType, fileData, fontSize, std::span<int>{codepoints, static_cast<size_t>(codepointCount)});
    }

    constexpr Font(const Font&) = delete;
    constexpr Font(Font&& other) {
        set(other);

        other.baseSize = 0;
        other.glyphCount = 0;
        other.glyphPadding = 0;
        other.texture = { .id = 0 };
        other.recs = nullptr;
        other.glyphs = nullptr;
    }
    ~Font() {
        Unload();
    }

    void Unload() {
        // Protect against calling UnloadFont() twice.
        if (baseSize != 0) {
            UnloadFont(*this);
            baseSize = 0;
        }
    }

    GETTERSETTER(int, BaseSize, baseSize)
    GETTERSETTER(int, GlyphCount, glyphCount)
    GETTERSETTER(int, GlyphPadding, glyphPadding)
    GETTERSETTER(::Rectangle*, Recs, recs)
    GETTERSETTER(::GlyphInfo*, Glyphs, glyphs)

    /**
     * Get the texture atlas containing the glyphs.
     */
    TextureUnmanaged GetTexture() {
        return TextureUnmanaged{texture};
    }

    /**
     * Set the texture atlas containing the glyphs.
     */
    constexpr void SetTexture(const ::Texture& newTexture) = delete;
    constexpr void SetTexture(::Texture&& newTexture) {
        texture = newTexture;

        newTexture.id = 0;
        newTexture.width = 0;
        newTexture.height = 0;
        newTexture.mipmaps = 0;
        newTexture.format = 0;
    }

    Font& operator=(const ::Font& font) = delete;
    Font& operator=(::Font&& font) {
        Unload();
        set(font);

        font.baseSize = 0;
        font.glyphCount = 0;
        font.glyphPadding = 0;
        font.texture = { .id = 0 };
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
        set(other);

        other.baseSize = 0;
        other.glyphCount = 0;
        other.glyphPadding = 0;
        other.texture = { .id = 0 };
        other.recs = nullptr;
        other.glyphs = nullptr;

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
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const std::string& fileName) RAYLIB_CPP_THROWS {
        set(::LoadFont(fileName.c_str()));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Font with from file: " + fileName));
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
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const std::filesystem::path& fileName, int fontSize, std::span<int> fontChars) RAYLIB_CPP_THROWS {
        set(::LoadFontEx(fileName.c_str(), fontSize, fontChars.data(), static_cast<int>(fontChars.size())));
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

    /**
     * Returns if the font is ready to be used.
     */
    [[nodiscard]] bool IsReady() const {
        return ::IsFontReady(*this);
    }

    /**
     * Draw text using font and additional parameters.
     */
    void DrawText(const char* text, ::Vector2 position, float fontSize,
                  float spacing, ::Color tint = WHITE) const {
        ::DrawTextEx(*this, text, position,  fontSize,  spacing,  tint);
    }

    /**
     * Draw text using font and additional parameters.
     */
    void DrawText(const std::string& text, ::Vector2 position, float fontSize,
                  float spacing, ::Color tint = WHITE) const {
        ::DrawTextEx(*this, text.c_str(), position,  fontSize,  spacing,  tint);
    }

    /**
     * Draw text using font and additional parameters.
     */
    void DrawText(const char* text, int posX, int posY, float fontSize,
                  float spacing, ::Color tint = WHITE) const {
        ::DrawTextEx(*this, text,
            { .x = static_cast<float>(posX), .y = static_cast<float>(posY) },
            fontSize, spacing, tint);
    }

    /**
     * Draw text using font and additional parameters.
     */
    void DrawText(const std::string& text, int posX, int posY, float fontSize,
                  float spacing, ::Color tint = WHITE) const {
        ::DrawTextEx(*this, text.c_str(),
            { .x = static_cast<float>(posX), .y = static_cast<float>(posY) },
            fontSize, spacing, tint);
    }

    void DrawText(
            const char* text,
            ::Vector2 position,
            ::Vector2 origin,
            float rotation,
            float fontSize,
            float spacing,
            ::Color tint = WHITE) const {
        ::DrawTextPro(*this, text,
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
            ::Color tint = WHITE) const {
        ::DrawTextPro(*this, text.c_str(),
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
            ::Color tint = WHITE) const {
        ::DrawTextCodepoint(*this, codepoint, position, fontSize, tint);
    }

    /**
     * Draw multiple character (codepoint)
     */
    void DrawText(std::span<const int> codepoints,
            ::Vector2 position,
            float fontSize, float spacing,
            ::Color tint = WHITE) const {
        ::DrawTextCodepoints(*this,
            codepoints.data(), static_cast<int>(codepoints.size()),
            position, fontSize,
            spacing, tint);
    }

    /**
     * Measure string size for Font
     */
    Vector2 MeasureText(const char* text, float fontSize, float spacing) const {
        return ::MeasureTextEx(*this, text, fontSize, spacing);
    }

    /**
     * Measure string size for Font
     */
    [[nodiscard]] Vector2 MeasureText(const std::string& text, float fontSize, float spacing) const {
        return ::MeasureTextEx(*this, text.c_str(), fontSize, spacing);
    }

    /**
     * Get index position for a unicode character on font
     */
    [[nodiscard]] int GetGlyphIndex(int character) const {
        return ::GetGlyphIndex(*this, character);
    }

    /**
     * Create an image from text (custom sprite font)
     */
    ::Image ImageText(const char* text, float fontSize,
                      float spacing, ::Color tint) const {
        return ::ImageTextEx(*this, text, fontSize, spacing, tint);
    }

    /**
     * Create an image from text (custom sprite font)
     */
    [[nodiscard]] ::Image ImageText(const std::string& text, float fontSize,
                                    float spacing, ::Color tint) const {
        return ::ImageTextEx(*this, text.c_str(), fontSize, spacing, tint);
    }

 protected:
    constexpr void set(const ::Font& font) noexcept {
        baseSize = font.baseSize;
        glyphCount = font.glyphCount;
        glyphPadding = font.glyphPadding;
        texture = font.texture;
        recs = font.recs;
        glyphs = font.glyphs;
    }
};
}  // namespace raylib

using RFont = raylib::Font;

#endif  // RAYLIB_CPP_INCLUDE_FONT_HPP_
