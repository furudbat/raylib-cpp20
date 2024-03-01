#ifndef RAYLIB_CPP_INCLUDE_VECTOR4_HPP_
#define RAYLIB_CPP_INCLUDE_VECTOR4_HPP_

#ifndef RAYLIB_CPP_NO_MATH
#include <cmath>
#include <utility>
#endif
#include <string>

#include "./raylib.hpp"
#include "./raymath.hpp"
#include "./Rectangle.hpp"
#include "./Matrix.hpp"
#include "./raylib-cpp-utils.hpp"

namespace raylib {

class Color;

/**
 * Vector4 type
 */
class Vector4 : public ::Vector4 {
 public:
    explicit constexpr Vector4(const ::Vector4& vec = {
            .x = 0, .y = 0, .z = 0, .w = 0,
    }) : ::Vector4{vec.x, vec.y, vec.z, vec.w} {}

    [[deprecated("Use Vector4(vec)")]]
    constexpr Vector4(float _x, float _y, float _z, float _w) : ::Vector4{_x, _y, _z, _w} {}
    [[deprecated("Use Vector4(vec)")]]
    constexpr Vector4(float _x, float _y, float _z) : ::Vector4{_x, _y, _z, 0} {}
    [[deprecated("Use Vector4(vec)")]]
    constexpr Vector4(float _x, float _y) : ::Vector4{_x, _y, 0, 0} {}
    [[deprecated("Use Vector4(vec)")]]
    explicit constexpr Vector4(float _x) : ::Vector4{_x, 0, 0, 0} {}

    explicit constexpr Vector4(::Rectangle rectangle) : ::Vector4{rectangle.x, rectangle.y, rectangle.width, rectangle.height} {}

    explicit Vector4(::Color color) {
        set(::ColorNormalize(color));
    }

    GETTERSETTER(float, X, x)
    GETTERSETTER(float, Y, y)
    GETTERSETTER(float, Z, z)
    GETTERSETTER(float, W, w)

    constexpr Vector4& operator=(const ::Vector4& vector4) {
        set(vector4);
        return *this;
    }

    constexpr bool operator==(const ::Vector4& other) const {
        return x == other.x
            && y == other.y
            && z == other.z
            && w == other.w;
    }

    constexpr bool operator!=(const ::Vector4& other) const {
        return !(*this == other);
    }

    [[nodiscard]] raylib::Rectangle ToRectangle() const {
        return raylib::Rectangle(::Rectangle{.x = x, .y = y, .width = z, .height = w});
    }

    explicit operator raylib::Rectangle() const {
        return raylib::Rectangle(::Rectangle{.x = x, .y = y, .width = z, .height = w});
    }

    std::string ToString() const {
        return TextFormat("Vector4(%f, %f, %f, %f)", x, y, z, w);
    }

    operator std::string() const {
        return ToString();
    }

#ifndef RAYLIB_CPP_NO_MATH
    Vector4 Multiply(::Vector4 vector4) const {
        return Vector4{::QuaternionMultiply(*this, vector4)};
    }

    Vector4 operator*(::Vector4 vector4) const {
        return Vector4{::QuaternionMultiply(*this, vector4)};
    }

    Vector4 Lerp(::Vector4 vector4, float amount) const {
        return Vector4{::QuaternionLerp(*this, vector4, amount)};
    }

    Vector4 Nlerp(::Vector4 vector4, float amount) const {
        return Vector4{::QuaternionNlerp(*this, vector4, amount)};
    }

    Vector4 Slerp(::Vector4 vector4, float amount) const {
        return Vector4{::QuaternionSlerp(*this, vector4, amount)};
    }

    raylib::Matrix ToMatrix() const {
        return raylib::Matrix{::QuaternionToMatrix(*this)};
    }

    float Length() const {
        return ::QuaternionLength(*this);
    }

    Vector4 Normalize() const {
        return Vector4{::QuaternionNormalize(*this)};
    }

    Vector4 Invert() const {
        return Vector4{::QuaternionInvert(*this)};
    }

    void ToAxisAngle(::Vector3 &outAxis, float &outAngle) const {
        ::QuaternionToAxisAngle(*this, &outAxis, &outAngle);
    }

    /**
     * Get the rotation angle and axis for a given quaternion
     */
     struct AxisAngle {
         Vector3 outAxis;
         float outAngle;
     };
     AxisAngle ToAxisAngle() const {
        AxisAngle ret;
        QuaternionToAxisAngle(*this, &ret.outAxis, &ret.outAngle);

        return ret;
    }

    Vector4 Transform(::Matrix matrix) const {
        return Vector4{::QuaternionTransform(*this, matrix)};
    }

    static Vector4 Identity() {
        return Vector4{::QuaternionIdentity()};
    }

    static Vector4 FromVector3ToVector3(::Vector3 from , ::Vector3 to) {
        return Vector4{::QuaternionFromVector3ToVector3(from , to)};
    }

    static Vector4 FromMatrix(::Matrix matrix) {
        return Vector4{::QuaternionFromMatrix(matrix)};
    }

    static Vector4 FromAxisAngle(::Vector3 axis, const float angle) {
        return Vector4{::QuaternionFromAxisAngle(axis, angle)};
    }

    static Vector4 FromEuler(const float pitch, const float yaw, const float roll) {
        return Vector4{::QuaternionFromEuler(pitch, yaw, roll)};
    }

    static Vector4 FromEuler(::Vector3 vector3) {
        return Vector4{::QuaternionFromEuler(vector3.x, vector3.y, vector3.z)};
    }

    raylib::Vector3 ToEuler() const {
        return raylib::Vector3{::QuaternionToEuler(*this)};
    }
#endif

/// @FIXME: Incomplete result type 'raylib::Color' in function definition
/*
    [[nodiscard]] raylib::Color ColorFromNormalized() const {
        return raylib::Color{::ColorFromNormalized(*this)};
    }

    explicit operator raylib::Color() const {
        return raylib::Color{::ColorFromNormalized()};
    }
*/

 protected:
    constexpr void set(const ::Vector4& vec4) {
        x = vec4.x;
        y = vec4.y;
        z = vec4.z;
        w = vec4.w;
    }
};

// Alias the Vector4 as Quaternion.
using Quaternion = Vector4;

}  // namespace raylib

using RVector4 = raylib::Vector4;
using RQuaternion = raylib::Quaternion;

#endif  // RAYLIB_CPP_INCLUDE_VECTOR4_HPP_
