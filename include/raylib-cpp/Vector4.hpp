#ifndef RAYLIB_CPP_INCLUDE_VECTOR4_HPP_
#define RAYLIB_CPP_INCLUDE_VECTOR4_HPP_

#include "raylib.hpp"

#include "Matrix.hpp"
#include "raylib-cpp-utils.hpp"
#include "raymath.hpp"

#include <string>
#include <utility>
#ifndef RAYLIB_CPP_NO_MATH
#include <cmath>
#endif

namespace raylib {

class Color;

/**
 * Vector4 type
 */
class Vector4 : public ::Vector4 {
public:
    explicit constexpr Vector4() : ::Vector4{0, 0, 0, 0} {}

    explicit constexpr Vector4(const ::Vector4& vec) : ::Vector4{vec.x, vec.y, vec.z, vec.w} {}

    [[deprecated("Use Vector4(vec)")]]
    constexpr Vector4(float _x, float _y, float _z, float _w)
        : ::Vector4{_x, _y, _z, _w} {}
    [[deprecated("Use Vector4(vec)")]]
    constexpr Vector4(float _x, float _y, float _z)
        : ::Vector4{_x, _y, _z, 0} {}
    [[deprecated("Use Vector4(vec)")]]
    constexpr Vector4(float _x, float _y)
        : ::Vector4{_x, _y, 0, 0} {}
    [[deprecated("Use Vector4(vec)")]]
    explicit constexpr Vector4(float _x)
        : ::Vector4{_x, 0, 0, 0} {}

    explicit constexpr Vector4(const ::Rectangle& rectangle) noexcept
        : ::Vector4{rectangle.x, rectangle.y, rectangle.width, rectangle.height} {}

    explicit Vector4(::Color color) noexcept { set(::ColorNormalize(color)); }

    GETTERSETTER(float, X, x)
    GETTERSETTER(float, Y, y)
    GETTERSETTER(float, Z, z)
    GETTERSETTER(float, W, w)

    constexpr Vector4& operator=(const ::Vector4& vector4) & {
        set(vector4);
        return *this;
    }
    constexpr Vector4& operator=(::Vector4&& vector4) && noexcept {
        set(vector4);
        return *this;
    }

    constexpr bool operator==(const ::Vector4& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    constexpr bool operator!=(const ::Vector4& other) const { return !(*this == other); }

    /*
    [[nodiscard]] raylib::Rectangle ToRectangle() const {
        return raylib::Rectangle{::Rectangle{.x = m_data.x, .y = m_data.y, .width = m_data.z, .height = m_data.w}};
    }

    explicit operator raylib::Rectangle() const {
        return raylib::Rectangle{::Rectangle{.x = m_data.x, .y = m_data.y, .width = m_data.z, .height = m_data.w}};
    }
    */

    [[nodiscard]] std::string ToString() const {
        return ::TextFormat(
            "Vector4(%f, %f, %f, %f)",
            static_cast<double>(x),
            static_cast<double>(y),
            static_cast<double>(z),
            static_cast<double>(w));
    }

    explicit operator std::string() const { return ToString(); }

    /*
    explicit(false) operator ::Vector4() const {
        return m_data;
    }
    */

#ifndef RAYLIB_CPP_NO_MATH
    [[nodiscard]] Vector4 Multiply(::Vector4 vector4) const { return Vector4{::QuaternionMultiply(*this, vector4)}; }

    Vector4 operator*(const ::Vector4& vector4) const { return Vector4{::QuaternionMultiply(*this, vector4)}; }

    [[nodiscard]] Vector4 Lerp(::Vector4 vector4, float amount) const {
        return Vector4{::QuaternionLerp(*this, vector4, amount)};
    }

    [[nodiscard]] Vector4 Nlerp(::Vector4 vector4, float amount) const {
        return Vector4{::QuaternionNlerp(*this, vector4, amount)};
    }

    [[nodiscard]] Vector4 Slerp(::Vector4 vector4, float amount) const {
        return Vector4{::QuaternionSlerp(*this, vector4, amount)};
    }

    [[nodiscard]] raylib::Matrix ToMatrix() const { return raylib::Matrix{::QuaternionToMatrix(*this)}; }

    [[nodiscard]] float Length() const { return ::QuaternionLength(*this); }

    [[nodiscard]] Vector4 Normalize() const { return Vector4{::QuaternionNormalize(*this)}; }

    [[nodiscard]] Vector4 Invert() const { return Vector4{::QuaternionInvert(*this)}; }

    [[deprecated("Use ToAxisAngle(): AxisAngle")]]
    void ToAxisAngle(::Vector3& outAxis, float& outAngle) const {
        ::QuaternionToAxisAngle(*this, &outAxis, &outAngle);
    }

    /**
     * Get the rotation angle and axis for a given quaternion
     */
    struct AxisAngle {
        ::Vector3 outAxis;
        float outAngle;
    };
    [[nodiscard]] AxisAngle ToAxisAngle() const {
        AxisAngle ret;
        ::QuaternionToAxisAngle(*this, &ret.outAxis, &ret.outAngle);

        return ret;
    }

    [[nodiscard]] Vector4 Transform(const ::Matrix& matrix) const {
        return Vector4{::QuaternionTransform(*this, matrix)};
    }

    static Vector4 Identity() { return Vector4{::QuaternionIdentity()}; }

    static Vector4 FromVector3ToVector3(::Vector3 from, ::Vector3 to) {
        return Vector4{::QuaternionFromVector3ToVector3(from, to)};
    }

    static Vector4 FromMatrix(const ::Matrix& matrix) { return Vector4{::QuaternionFromMatrix(matrix)}; }

    static Vector4 FromAxisAngle(::Vector3 axis, float angle) {
        return Vector4{::QuaternionFromAxisAngle(axis, angle)};
    }

    static Vector4 FromEuler(float pitch, float yaw, float roll) {
        return Vector4{::QuaternionFromEuler(pitch, yaw, roll)};
    }

    static Vector4 FromEuler(::Vector3 vector3) {
        return Vector4{::QuaternionFromEuler(vector3.x, vector3.y, vector3.z)};
    }

    [[nodiscard]] Vector3 ToEuler() const { return Vector3{::QuaternionToEuler(*this)}; }
#endif

    /// @FIXME: Incomplete result type 'raylib::Color' in function definition
    /*
        [[nodiscard]] raylib::Color ColorFromNormalized() const noexcept {
            return raylib::Color{::ColorFromNormalized(*this)};
        }

        explicit operator raylib::Color() const noexcept {
            return raylib::Color{::ColorFromNormalized()};
        }
    */
protected:
    constexpr void set(const ::Vector4& vec4) noexcept {
        x = vec4.x;
        y = vec4.y;
        z = vec4.z;
        w = vec4.w;
    }
};

// Alias the Vector4 as Quaternion.
using Quaternion = Vector4;

} // namespace raylib

using RVector4 = raylib::Vector4;
using RQuaternion = raylib::Quaternion;

#endif // RAYLIB_CPP_INCLUDE_VECTOR4_HPP_
