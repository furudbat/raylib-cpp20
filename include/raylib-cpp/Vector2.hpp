#ifndef RAYLIB_CPP_INCLUDE_VECTOR2_HPP_
#define RAYLIB_CPP_INCLUDE_VECTOR2_HPP_

#ifndef RAYLIB_CPP_NO_MATH
#include <cmath>
#endif

#include <string>

#include "./raylib.hpp"
#include "./raymath.hpp"
#include "./raylib-cpp-utils.hpp"

namespace raylib {
/**
 * Vector2 type
 */
class Vector2 : public ::Vector2 {
 public:
    inline static constexpr ::Color DefaultDrawColor = BLACK;
    inline static constexpr int DefaultCheckCollisionPointLineThreshold = 1;

    constexpr Vector2(const ::Vector2& vec = {
            .x = 0, .y = 0,
    }) : ::Vector2{vec.x, vec.y} {}

    [[deprecated("Use Vector2(vec)")]]
    constexpr Vector2(float _x, float _y) : ::Vector2{_x, _y} {}
    [[deprecated("Use Vector2(vec)")]]
    explicit constexpr Vector2(float _x) : ::Vector2{_x, 0} {}

    GETTERSETTER(float, X, x)
    GETTERSETTER(float, Y, y)

    /**
     * Set the Vector2 to the same as the given Vector2.
     */
    constexpr Vector2& operator=(const ::Vector2& vector2) noexcept {
        set(vector2);
        return *this;
    }

    /**
     * Determine whether or not the vectors are equal.
     */
    constexpr bool operator==(const ::Vector2& other) const noexcept {
        return x == other.x
            && y == other.y;
    }

    /**
     * Determines if the vectors are not equal.
     */
    constexpr bool operator!=(const ::Vector2& other) const noexcept {
        return !(*this == other);
    }

    [[nodiscard]] std::string ToString() const {
        return ::TextFormat("Vector2(%f, %f)", static_cast<double>(x), static_cast<double>(y));
    }

    explicit operator std::string() const {
        return ToString();
    }

    /*
    explicit(false) operator ::Vector2() const {
        return *this;
    }
    */

#ifndef RAYLIB_CPP_NO_MATH
    /**
     * Add two vectors (v1 + v2)
     */
    [[nodiscard]] Vector2 Add(::Vector2 vector2) const noexcept {
        return ::Vector2Add(*this, vector2);
    }

    /**
     * Add two vectors (v1 + v2)
     */
    Vector2 operator+(const ::Vector2& vector2) const noexcept {
        return ::Vector2Add(*this, vector2);
    }

    /**
     * Add two vectors (v1 + v2)
     */
    Vector2& operator+=(const ::Vector2& vector2) noexcept {
        set(::Vector2Add(*this, vector2));

        return *this;
    }

    /**
     * Subtract two vectors (v1 - v2)
     */
    [[nodiscard]] Vector2 Subtract(::Vector2 vector2) const noexcept {
        return ::Vector2Subtract(*this, vector2);
    }

    /**
     * Subtract two vectors (v1 - v2)
     */
    Vector2 operator-(const ::Vector2& vector2) const noexcept {
        return ::Vector2Subtract(*this, vector2);
    }

    /**
     * Add two vectors (v1 + v2)
     */
    Vector2& operator-=(const ::Vector2& vector2) noexcept {
        set(::Vector2Subtract(*this, vector2));

        return *this;
    }

    /**
     * Negate vector
     */
    [[nodiscard]] Vector2 Negate() const noexcept {
        return ::Vector2Negate(*this);
    }

    /**
     * Negate vector
     */
    Vector2 operator-() const noexcept {
        return ::Vector2Negate(*this);
    }

    /**
     * Multiply vector by vector
     */
    [[nodiscard]] Vector2 Multiply(::Vector2 vector2) const noexcept {
        return ::Vector2Multiply(*this, vector2);
    }

    /**
     * Multiply vector by vector
     */
    Vector2 operator*(const ::Vector2& vector2) const noexcept {
        return ::Vector2Multiply(*this, vector2);
    }

    /**
     * Multiply vector by vector
     */
    Vector2& operator*=(const ::Vector2& vector2) noexcept {
        set(::Vector2Multiply(*this, vector2));

        return *this;
    }

    /**
     * Scale vector (multiply by value)
     */
    [[nodiscard]] Vector2 Scale(float scale) const noexcept {
        return ::Vector2Scale(*this, scale);
    }

    /**
     * Scale vector (multiply by value)
     */
    Vector2 operator*(float scale) const noexcept {
        return ::Vector2Scale(*this, scale);
    }

    /**
     * Scale vector (multiply by value)
     */
    Vector2& operator*=(float scale) noexcept {
        set(::Vector2Scale(*this, scale));

        return *this;
    }

    /**
     * Divide vector by vector
     */
    [[nodiscard]] Vector2 Divide(::Vector2 vector2) const {
        return ::Vector2Divide(*this, vector2);
    }

    /**
     * Divide vector by vector
     */
    Vector2 operator/(const ::Vector2& vector2) const {
        return ::Vector2Divide(*this, vector2);
    }

    /**
     * Divide vector by vector
     */
    Vector2& operator/=(const ::Vector2& vector2) {
        set(::Vector2Divide(*this, vector2));

        return *this;
    }

    /**
     * Divide vector by value
     */
    [[nodiscard]] constexpr raylib::Vector2 Divide(float div) const {
        return raylib::Vector2{::Vector2{x / div, y / div}};
    }

    /**
     * Divide vector by value
     */
    constexpr Vector2 operator/(float div) const {
        return Divide(div);
    }

    /**
     * Divide vector by value
     */
    constexpr Vector2& operator/=(float div) {
        x /= div;
        y /= div;

        return *this;
    }

    /**
     * Normalize provided vector
     */
    [[nodiscard]] Vector2 Normalize() const noexcept {
        return ::Vector2Normalize(*this);
    }

    /**
     * Transforms a Vector2 by a given Matrix
     */
    [[nodiscard]] Vector2 Transform(const ::Matrix& mat) const noexcept {
        return ::Vector2Transform(*this, mat);
    }

    /**
     * Calculate linear interpolation between two vectors
     */
    [[nodiscard]] Vector2 Lerp(::Vector2 vector2, float amount) const noexcept {
        return ::Vector2Lerp(*this, vector2, amount);
    }

    /**
     * Calculate reflected vector to normal
     */
    [[nodiscard]] Vector2 Reflect(::Vector2 normal) const noexcept {
        return ::Vector2Reflect(*this, normal);
    }

    /**
     * Rotate Vector by float in radians
     */
    [[nodiscard]] Vector2 Rotate(float angle) const noexcept {
        return ::Vector2Rotate(*this, angle);
    }

    /**
     * Move Vector towards target
     */
    [[nodiscard]] Vector2 MoveTowards(::Vector2 target, float maxDistance) const noexcept {
        return ::Vector2MoveTowards(*this, target, maxDistance);
    }

    /**
     * Invert the given vector
     */
    [[nodiscard]] Vector2 Invert() const noexcept {
        return ::Vector2Invert(*this);
    }

    /**
     * Clamp the components of the vector between
     */
    [[nodiscard]] Vector2 Clamp(::Vector2 min, ::Vector2 max) const noexcept {
        return ::Vector2Clamp(*this, min, max);
    }

    /**
     * // Clamp the magnitude of the vector between two min and max values
     */
    [[nodiscard]] Vector2 Clamp(float min, float max) const noexcept {
        return ::Vector2ClampValue(*this, min, max);
    }

    /**
     * Check whether two given vectors are almost equal
     */
    [[nodiscard]] int Equals(::Vector2 q) const noexcept {
        return ::Vector2Equals(*this, q);
    }

    /**
     * Calculate vector length
     */
    [[nodiscard]] float Length() const noexcept {
        return ::Vector2Length(*this);
    }

    /**
     * Calculate vector square length
     */
    [[nodiscard]] float LengthSqr() const noexcept {
        return ::Vector2LengthSqr(*this);
    }

    /**
     * Calculate two vectors dot product
     */
    [[nodiscard]] float DotProduct(::Vector2 vector2) const noexcept {
        return Vector2DotProduct(*this, vector2);
    }

    /**
     * Calculate distance between two vectors
     */
    [[nodiscard]] float Distance(::Vector2 vector2) const noexcept {
        return Vector2Distance(*this, vector2);
    }

    /**
     * Calculate square distance between two vectors
     */
    [[nodiscard]] float DistanceSqr(::Vector2 v2) const {
        return ::Vector2DistanceSqr(*this, v2);
    }

    /**
     * Calculate angle from two vectors in X-axis
     */
    [[nodiscard]] float Angle(::Vector2 vector2) const noexcept {
        return Vector2Angle(*this, vector2);
    }

    /**
     * Vector with components value 0.0f
     */
    static Vector2 Zero() noexcept {
        return ::Vector2Zero();
    }

    /**
     * Vector with components value 1.0f
     */
    static Vector2 One() noexcept {
        return ::Vector2One();
    }
#endif

    void DrawPixel(::Color color = DefaultDrawColor) const noexcept {
        ::DrawPixelV(*this, color);
    }

    void DrawLine(::Vector2 endPos, ::Color color = DefaultDrawColor) const noexcept {
        ::DrawLineV(*this, endPos, color);
    }

    void DrawLine(::Vector2 endPos, float thick, ::Color color = DefaultDrawColor) const noexcept {
        ::DrawLineEx(*this, endPos, thick, color);
    }

    void DrawLineBezier(::Vector2 endPos, float thick, ::Color color = DefaultDrawColor) const noexcept {
        ::DrawLineBezier(*this, endPos, thick, color);
    }

    /**
     * Draw a color-filled circle (Vector version)
     */
    void DrawCircle(float radius, ::Color color = DefaultDrawColor) const noexcept {
        ::DrawCircleV(*this, radius, color);
    }

    void DrawRectangle(::Vector2 size, ::Color color = DefaultDrawColor) const noexcept {
        ::DrawRectangleV(*this, size, color);
    }

    void DrawPoly(int sides, float radius, float rotation, ::Color color = DefaultDrawColor) const noexcept {
        ::DrawPoly(*this, sides, radius, rotation, color);
    }

    /**
     * Check collision between two circles
     */
    [[nodiscard]] bool CheckCollisionCircle(float radius1, ::Vector2 center2, float radius2) const noexcept {
        return ::CheckCollisionCircles(*this, radius1, center2, radius2);
    }

    /**
     * Check collision between circle and rectangle
     */
    [[nodiscard]] bool CheckCollisionCircle(float radius, const ::Rectangle& rec) const noexcept {
        return ::CheckCollisionCircleRec(*this, radius, rec);
    }

    /**
     * Check if point is inside rectangle
     */
    [[nodiscard]] bool CheckCollision(const ::Rectangle& rec) const noexcept {
        return ::CheckCollisionPointRec(*this, rec);
    }

    /**
     * Check if point is inside circle
     */
    [[nodiscard]] bool CheckCollision(::Vector2 center, float radius) const noexcept {
        return ::CheckCollisionPointCircle(*this, center, radius);
    }

    /**
     * Check if point is inside a triangle
     */
    [[nodiscard]] bool CheckCollision(::Vector2 p1, ::Vector2 p2, ::Vector2 p3) const noexcept {
        return ::CheckCollisionPointTriangle(*this, p1, p2, p3);
    }

    /**
     * Check the collision between two lines defined by two points each, returns collision point by reference
     */
    [[nodiscard]] bool CheckCollisionLines(
            ::Vector2 endPos1,
            ::Vector2 startPos2, ::Vector2 endPos2,
            ::Vector2 &collisionPoint) const noexcept {
        return ::CheckCollisionLines(*this, endPos1, startPos2, endPos2, &collisionPoint);
    }
    [[nodiscard]] bool CheckCollisionLines(
            ::Vector2 endPos1,
            ::Vector2 startPos2, ::Vector2 endPos2) const noexcept {
        return ::CheckCollisionLines(*this, endPos1, startPos2, endPos2, nullptr);
    }

    /**
     * Check if point belongs to line created between two points [p1] and [p2] with defined margin in pixels [threshold]
     */
    [[nodiscard]] bool CheckCollisionPointLine(::Vector2 p1, ::Vector2 p2, int threshold = DefaultCheckCollisionPointLineThreshold) const noexcept {
        return ::CheckCollisionPointLine(*this, p1, p2, threshold);
    }

 protected:
    constexpr void set(const ::Vector2& vec) noexcept {
        x = vec.x;
        y = vec.y;
    }
};

}  // namespace raylib

using RVector2 = raylib::Vector2;

#endif  // RAYLIB_CPP_INCLUDE_VECTOR2_HPP_
