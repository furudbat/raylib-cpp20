#ifndef RAYLIB_CPP_INCLUDE_COLOR_HPP_
#define RAYLIB_CPP_INCLUDE_COLOR_HPP_

#include <string>
#include <cstdint>

#include "./raylib.hpp"
#include "./Vector4.hpp"
#include "./raylib-cpp-utils.hpp"

namespace raylib {

struct ColorHSV {
    float hue;
    float saturation;
    float value;
};

/**
 * Color type, RGBA (32bit)
 */
class Color : public ::Color {
 public:
    inline static constexpr float DefaultDrawLineBezierThick = 1.0f;
    inline static constexpr int DefaultDrawTextFontSize = 10;

    /**
     * Black.
     */
    constexpr Color() : ::Color{0, 0, 0, 255} {}

    explicit constexpr Color(::Color color) : ::Color{color.r, color.g, color.b, color.a} {}

    constexpr Color(
            uint8_t red,
            uint8_t green,
            uint8_t blue,
            uint8_t alpha = 255) : ::Color{red, green, blue, alpha} {}

    /**
     * Returns a Color from HSV values
     */
    //[[deprecated("Use Color(ColorHSV) for better structured name")]]
    explicit Color(const ::Vector3& hsv) noexcept {
        set(::ColorFromHSV(hsv.x, hsv.y, hsv.z));
    }
    explicit Color(ColorHSV hsv) noexcept {
        set(::ColorFromHSV(hsv.hue, hsv.saturation, hsv.value));
    }

    /**
     * Returns a Color from HSV values
     */
    [[deprecated("Use FromHSV(ColorHSV)")]]
    static ::Color FromHSV(float hue, float saturation, float value) noexcept {
        return ::ColorFromHSV(hue, saturation, value);
    }
    //[[deprecated("Use FromHSV(ColorHSV) for better structured name")]]
    static ::Color FromHSV(const ::Vector3& hsv) noexcept {
        return ::ColorFromHSV(hsv.x, hsv.y, hsv.z);
    }
    static ::Color FromHSV(const ColorHSV& hsv) noexcept {
        return ::ColorFromHSV(hsv.hue, hsv.saturation, hsv.value);
    }

    /**
     * Get Color structure from hexadecimal value
     */
    explicit Color(uint32_t hexValue) noexcept {
        set(::GetColor(hexValue));
    }

    Color(void *srcPtr, int format) noexcept {
        set(::GetPixelColor(srcPtr, format));
    }

    /**
     * Returns hexadecimal value for a Color
     */
    [[nodiscard]] int ToInt() const noexcept {
        return ::ColorToInt(*this);
    }

    /**
     * Returns hexadecimal value for a Color
     */
    explicit operator int() const noexcept {
        return ::ColorToInt(*this);
    }

    [[nodiscard]] std::string ToString() const {
        return TextFormat("Color(%d, %d, %d, %d)", r, g, b, a);
    }

    explicit operator std::string() const {
        return ToString();
    }

    /*
    explicit(false) operator ::Color() const {
        return *this;
    }
    */

    /**
     * Returns color with alpha applied, alpha goes from 0.0f to 1.0f
     */
    [[nodiscard]] Color Fade(float alpha) const noexcept {
        return Color{::Fade(*this, alpha)};
    }

    /**
     * Returns Color normalized as float [0..1]
     */
    [[nodiscard]] raylib::Vector4 Normalize() const noexcept {
        return raylib::Vector4{::ColorNormalize(*this)};
    }

    /**
     * Returns Color from normalized values [0..1]
     */
    explicit Color(const ::Vector4& normalized) noexcept {
        set(::ColorFromNormalized(normalized));
    }

    /**
     * Returns HSV values for a Color
     */
    [[deprecated("Use ToColorHSV() for better named struct")]]
    [[nodiscard]] raylib::Vector3 ToHSV() const noexcept {
        return raylib::Vector3{::ColorToHSV(*this)};
    }
    [[nodiscard]] ColorHSV ToColorHSV() const noexcept {
        auto hsv = ::ColorToHSV(*this);
        return {.hue = hsv.x, .saturation = hsv.y, .value = hsv.z};
    }

    GETTERSETTER(uint8_t, R, r)
    GETTERSETTER(uint8_t, G, g)
    GETTERSETTER(uint8_t, B, b)
    GETTERSETTER(uint8_t, A, a)

    Color& operator=(const ::Color& color) noexcept {
        set(color);
        return *this;
    }

    /**
     * Set background color (framebuffer clear color)
     */
    Color& ClearBackground() noexcept {
        ::ClearBackground(*this);
        return *this;
    }

    void DrawPixel(int x, int y) const noexcept {
        ::DrawPixel(x, y, *this);
    }

    /**
     * Draw a pixel
     */
    void DrawPixel(::Vector2 pos) const noexcept {
        ::DrawPixelV(pos, *this);
    }

    /**
     * Draw a line
     */
    void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY) const noexcept {
        ::DrawLine(startPosX, startPosY, endPosX, endPosY, *this);
    }

    /**
     * Draw a line using Vector points
     */
    void DrawLine(::Vector2 startPos, ::Vector2 endPos) const noexcept {
        ::DrawLineV(startPos, endPos, *this);
    }

    /**
     * Draw a line using Vector points, with a given thickness
     */
    void DrawLine(::Vector2 startPos, ::Vector2 endPos, float thick) const noexcept {
        ::DrawLineEx(startPos, endPos, thick, *this);
    }

    void DrawLineBezier(::Vector2 startPos, ::Vector2 endPos, float thick = DefaultDrawLineBezierThick) const noexcept {
        ::DrawLineBezier(startPos, endPos, thick, *this);
    }

    void DrawLineStrip(::Vector2 *points, int numPoints) const {
        ::DrawLineStrip(points, numPoints, *this);
    }

    void DrawText(const char* text, int posX = 0, int posY = 0, int fontSize = DefaultDrawTextFontSize) const noexcept {
        ::DrawText(text, posX, posY, fontSize, *this);
    }

    void DrawText(const std::string& text, int posX = 0, int posY = 0, int fontSize = DefaultDrawTextFontSize) const noexcept {
        ::DrawText(text.c_str(), posX, posY, fontSize, *this);
    }

    void DrawText(const ::Font& font, const char* text, ::Vector2 position,
            float fontSize, float spacing) const noexcept {
        ::DrawTextEx(font, text, position, fontSize, spacing, *this);
    }

    void DrawText(const ::Font& font, const std::string& text, ::Vector2 position,
            float fontSize, float spacing) const noexcept {
        ::DrawTextEx(font, text.c_str(), position, fontSize, spacing, *this);
    }

    void DrawText(
            const ::Font& font,
            const char* text,
            ::Vector2 position,
            ::Vector2 origin,
            float rotation,
            float fontSize,
            float spacing) const noexcept {
        ::DrawTextPro(font, text, position, origin, rotation, fontSize, spacing, *this);
    }

    void DrawText(
            const ::Font& font,
            const std::string& text,
            ::Vector2 position,
            ::Vector2 origin,
            float rotation,
            float fontSize,
            float spacing) const noexcept {
        ::DrawTextPro(font, text.c_str(), position, origin, rotation, fontSize, spacing, *this);
    }

    void DrawRectangle(int posX, int posY, int width, int height) const noexcept {
        ::DrawRectangle(posX, posY, width, height, *this);
    }

    void DrawRectangle(::Vector2 position, ::Vector2 size) const noexcept {
        ::DrawRectangleV(position, size, *this);
    }

    void DrawRectangle(::Rectangle rec) const noexcept {
        ::DrawRectangleRec(rec, *this);
    }

    void DrawRectangle(::Rectangle rec, ::Vector2 origin, float rotation) const noexcept {
        ::DrawRectanglePro(rec, origin, rotation, *this);
    }

    void DrawRectangleLines(int posX, int posY, int width, int height) const noexcept {
        ::DrawRectangleLines(posX, posY, width, height, *this);
    }

    void DrawRectangleLines(::Rectangle rec, float lineThick) const noexcept {
        ::DrawRectangleLinesEx(rec, lineThick, *this);
    }

    /**
     * Get color multiplied with another color
     */
    [[nodiscard]] Color Tint(::Color tint) noexcept {
        return Color{::ColorTint(*this, tint)};
    }

    /**
     * Get color with brightness correction, brightness factor goes from -1.0f to 1.0f
     */
    [[nodiscard]] Color Brightness(float factor) noexcept {
        return Color{::ColorBrightness(*this, factor)};
    }

    /**
     * Get color with contrast correction, contrast values between -1.0f and 1.0f
     */
    [[nodiscard]] Color Contrast(float contrast) noexcept {
        return Color{::ColorContrast(*this, contrast)};
    }

    /**
     * Returns color with alpha applied, alpha goes from 0.0f to 1.0f
     */
    [[nodiscard]] Color Alpha(float alpha) const noexcept {
        return Color{::ColorAlpha(*this, alpha)};
    }

    /**
     * Returns src alpha-blended into dst color with tint
     */
    [[nodiscard]] Color AlphaBlend(::Color dst, ::Color tint) const noexcept {
        return Color{::ColorAlphaBlend(dst, *this, tint)};
    }

    inline static constexpr Color LightGray() { return Color{LIGHTGRAY}; }
    inline static constexpr Color Gray() { return Color{GRAY}; }
    inline static constexpr Color DarkGray() { return Color{DARKGRAY}; }
    inline static constexpr Color Yellow() { return Color{YELLOW}; }
    inline static constexpr Color Gold() { return Color{GOLD}; }
    inline static constexpr Color Orange() { return Color{ORANGE}; }
    inline static constexpr Color Pink() { return Color{PINK}; }
    inline static constexpr Color Red() { return Color{RED}; }
    inline static constexpr Color Maroon() { return Color{MAROON}; }
    inline static constexpr Color Green() { return Color{GREEN}; }
    inline static constexpr Color Lime() { return Color{LIME}; }
    inline static constexpr Color DarkGreen() { return Color{DARKGREEN}; }
    inline static constexpr Color SkyBlue() { return Color{SKYBLUE}; }
    inline static constexpr Color Blue() { return Color{BLUE}; }
    inline static constexpr Color DarkBlue() { return Color{DARKBLUE}; }
    inline static constexpr Color Purple() { return Color{PURPLE}; }
    inline static constexpr Color Violet() { return Color{VIOLET}; }
    inline static constexpr Color DarkPurple() { return Color{DARKPURPLE}; }
    inline static constexpr Color Beige() { return Color{BEIGE}; }
    inline static constexpr Color Brown() { return Color{BROWN}; }
    inline static constexpr Color DarkBrown() { return Color{DARKBROWN}; }
    inline static constexpr Color White() { return Color{WHITE}; }
    inline static constexpr Color Black() { return Color{BLACK}; }
    inline static constexpr Color Blank() { return Color{BLANK}; }
    inline static constexpr Color Magenta() { return Color{MAGENTA}; }
    inline static constexpr Color RayWhite() { return Color{RAYWHITE}; }

 protected:
    constexpr void set(const ::Color& color) noexcept {
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
    }
};

}  // namespace raylib

using RColor = raylib::Color;

#endif  // RAYLIB_CPP_INCLUDE_COLOR_HPP_
