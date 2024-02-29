#ifndef RAYLIB_CPP_INCLUDE_RECTANGLE_HPP_
#define RAYLIB_CPP_INCLUDE_RECTANGLE_HPP_

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "./Vector2.hpp"

namespace raylib {
/**
 * Rectangle type
 */
class Rectangle : public ::Rectangle {
 public:
    explicit constexpr Rectangle(const ::Rectangle& rect = {
            .x = 0, .y = 0, .width = 0, .height = 0
    }) : ::Rectangle{rect.x, rect.y, rect.width, rect.height} {}

    [[deprecated("Use Rectangle(rect), use named-parameter struct")]]
    constexpr Rectangle(float _x, float _y, float _width, float _height) : ::Rectangle{_x, _y, _width, _height} {}
    [[deprecated("Use Rectangle(rect), use named-parameter struct")]]
    constexpr Rectangle(float _x, float _y, float _width) : ::Rectangle{_x, _y, _width, 0} {}
    [[deprecated("Use Rectangle(rect), use named-parameter struct")]]
    constexpr Rectangle(float _x, float _y) : ::Rectangle{_x, _y, 0, 0} {}
    [[deprecated("Use Rectangle(rect), use named-parameter struct")]]
    explicit constexpr Rectangle(float _x) : ::Rectangle{_x, 0, 0, 0} {}

    struct RectangleVector2 {
        ::Vector2 position;
        ::Vector2 size;
    };
    explicit constexpr Rectangle(RectangleVector2 position_size)
            : ::Rectangle{position_size.position.x, position_size.position.y, position_size.size.x, position_size.size.y} {}
    explicit constexpr Rectangle(::Vector2 size) : ::Rectangle{0, 0, size.x, size.y} {}
    explicit constexpr Rectangle(::Vector4 rect) : ::Rectangle{rect.x, rect.y, rect.z, rect.w} {}

    GETTERSETTER(float, X, x)
    GETTERSETTER(float, Y, y)
    GETTERSETTER(float, Width, width)
    GETTERSETTER(float, Height, height)

    constexpr Rectangle& operator=(const ::Rectangle& rect) {
        set(rect);
        return *this;
    }

    [[nodiscard]] constexpr ::Vector4 ToVector4() {
        return {x, y, width, height};
    }

    explicit constexpr operator ::Vector4() const {
        return {x, y, width, height};
    }

    /**
     * Draw a color-filled rectangle
     */
    void Draw(::Color color) const {
        ::DrawRectangleRec(*this, color);
    }

    void Draw(::Vector2 origin, float rotation, ::Color color) const {
        ::DrawRectanglePro(*this, origin, rotation, color);
    }

    void DrawGradientV(::Color color1, ::Color color2) const {
        ::DrawRectangleGradientV(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width),
            static_cast<int>(height), color1, color2);
    }

    void DrawGradientH(::Color color1, ::Color color2) const {
        ::DrawRectangleGradientH(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width),
            static_cast<int>(height), color1, color2);
    }

    void DrawGradient(::Color col1, ::Color col2, ::Color col3, ::Color col4) const {
        ::DrawRectangleGradientEx(*this, col1, col2, col3, col4);
    }

    void DrawLines(::Color color) const {
        ::DrawRectangleLines(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width),
            static_cast<int>(height), color);
    }

    void DrawLines(::Color color, float lineThick) const {
        ::DrawRectangleLinesEx(*this, lineThick, color);
    }

    void DrawRounded(float roundness, int segments, ::Color color) const {
        ::DrawRectangleRounded(*this, roundness, segments, color);
    }

    void DrawRoundedLines(float roundness, int segments,
            float lineThick, ::Color color) const {
        ::DrawRectangleRoundedLines(*this, roundness, segments, lineThick, color);
    }

    /**
     * Check collision between two rectangles
     */
    [[nodiscard]] bool CheckCollision(::Rectangle rec2) const {
        return ::CheckCollisionRecs(*this, rec2);
    }

    /**
     * Get collision rectangle for two rectangles collision
     */
    [[nodiscard]] ::Rectangle GetCollision(::Rectangle rec2) const {
        return ::GetCollisionRec(*this, rec2);
    }

    /**
     * Check if point is inside rectangle
     */
    [[nodiscard]] bool CheckCollision(::Vector2 point) const {
        return ::CheckCollisionPointRec(point, *this);
    }

    /**
     * Check collision between circle and rectangle
     */
    [[nodiscard]] bool CheckCollision(::Vector2 center, float radius) const {
        return ::CheckCollisionCircleRec(center, radius, *this);
    }

    [[nodiscard]] constexpr raylib::Vector2 GetSize() const {
        return raylib::Vector2{{.x = width, .y = height}};
    }

    constexpr Rectangle& SetSize(float newWidth, float newHeight) {
        width = newWidth;
        height = newHeight;
        return *this;
    }

    constexpr Rectangle& SetSize(const ::Vector2& size) {
        return SetSize(size.x, size.y);
    }

    Rectangle& SetShapesTexture(const ::Texture2D& texture) {
        ::SetShapesTexture(texture, *this);
        return *this;
    }

    [[nodiscard]] constexpr raylib::Vector2 GetPosition() const {
        return raylib::Vector2{{.x = x, .y = y}};
    }

    constexpr Rectangle& SetPosition(float newX, float newY) {
        x = newX;
        y = newY;
        return *this;
    }

    constexpr Rectangle& SetPosition(const ::Vector2& position) {
        return SetPosition(position.x, position.y);
    }

 protected:
    constexpr void set(const ::Rectangle& rect) {
        x = rect.x;
        y = rect.y;
        width = rect.width;
        height = rect.height;
    }
};
}  // namespace raylib

using RRectangle = raylib::Rectangle;

#endif  // RAYLIB_CPP_INCLUDE_RECTANGLE_HPP_
