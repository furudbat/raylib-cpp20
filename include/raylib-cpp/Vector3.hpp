#ifndef RAYLIB_CPP_INCLUDE_VECTOR3_HPP_
#define RAYLIB_CPP_INCLUDE_VECTOR3_HPP_


#include "raylib.hpp"
#include "raymath.hpp"
#include "raylib-cpp-utils.hpp"

#include <string>
#ifndef RAYLIB_CPP_NO_MATH
#include <cmath>
#endif

namespace raylib {
/**
 * Vector3 type
 */
class Vector3 : public ::Vector3 {
 public:
    constexpr explicit Vector3(const ::Vector3& vec = {
            .x = 0, .y = 0, .z = 0
    }) : ::Vector3{vec.x, vec.y, vec.z} {}

    explicit Vector3(::Color color) {
        set(ColorToHSV(color));
    }

    [[deprecated("Use Vector3(vec)")]]
    constexpr Vector3(float _x, float _y, float _z) : ::Vector3{_x, _y, _z} {}
    [[deprecated("Use Vector3(vec)")]]
    constexpr Vector3(float _x, float _y) : ::Vector3{_x, _y, 0} {}
    [[deprecated("Use Vector3(vec)")]]
    /*explicit*/ constexpr Vector3(float _x) : ::Vector3{_x, 0, 0} {}

    GETTERSETTER(float, X, x)
    GETTERSETTER(float, Y, y)
    GETTERSETTER(float, Z, z)

    constexpr Vector3& operator=(const ::Vector3& vector3) & noexcept {
        set(vector3);
        return *this;
    }
    constexpr Vector3& operator=(::Vector3&& vector3) && noexcept {
        set(vector3);
        return *this;
    }

    constexpr bool operator==(const ::Vector3& other) const noexcept {
        return x == other.x
            && y == other.y
            && z == other.z;
    }

    constexpr bool operator!=(const ::Vector3& other) const noexcept {
        return !(*this == other);
    }

    [[nodiscard]] std::string ToString() const {
        return ::TextFormat("Vector3(%f, %f, %f)", static_cast<double>(x), static_cast<double>(y), static_cast<double>(z));
    }

    explicit operator std::string() const {
        return ToString();
    }

    /*
    explicit(false) operator ::Vector3() const noexcept {
        return *this;
    }
    */

#ifndef RAYLIB_CPP_NO_MATH
    /**
     * Add two vectors
     */
    [[nodiscard]] Vector3 Add(const ::Vector3& vector3) const noexcept {
        return Vector3{::Vector3Add(*this, vector3)};
    }

    /**
     * Add two vectors
     */
    Vector3 operator+(const ::Vector3& vector3) const noexcept {
        return Vector3{::Vector3Add(*this, vector3)};
    }

    Vector3& operator+=(const ::Vector3& vector3) noexcept {
        set(::Vector3Add(*this, vector3));

        return *this;
    }

    /**
     * Subtract two vectors.
     */
    [[nodiscard]] Vector3 Subtract(::Vector3 vector3) const noexcept {
        return Vector3{::Vector3Subtract(*this, vector3)};
    }

    /**
     * Subtract two vectors.
     */
    Vector3 operator-(::Vector3 vector3) const noexcept {
        return Vector3{::Vector3Subtract(*this, vector3)};
    }

    Vector3& operator-=(::Vector3 vector3) noexcept {
        set(::Vector3Subtract(*this, vector3));

        return *this;
    }

    /**
     * Negate provided vector (invert direction)
     */
    [[nodiscard]] Vector3 Negate() const noexcept {
        return Vector3{::Vector3Negate(*this)};
    }

    /**
     * Negate provided vector (invert direction)
     */
    Vector3 operator-() const noexcept {
        return Vector3{::Vector3Negate(*this)};
    }

    /**
     * Multiply vector by vector
     */
    [[nodiscard]] Vector3 Multiply(::Vector3 vector3) const noexcept {
        return Vector3{::Vector3Multiply(*this, vector3)};
    }

    /**
     * Multiply vector by vector
     */
    Vector3 operator*(const ::Vector3& vector3) const noexcept {
        return Vector3{::Vector3Multiply(*this, vector3)};
    }

    /**
     * Multiply vector by vector
     */
    Vector3& operator*=(const ::Vector3& vector3) noexcept {
        set(Vector3Multiply(*this, vector3));

        return *this;
    }

    /**
     * Multiply vector by scalar
     */
    [[nodiscard]] Vector3 Scale(const float scaler) const noexcept {
        return Vector3{::Vector3Scale(*this, scaler)};
    }

    /**
     * Multiply vector by scalar
     */
    Vector3 operator*(const float scaler) const noexcept {
        return Vector3{::Vector3Scale(*this, scaler)};
    }

    /**
     * Multiply vector by scalar
     */
    Vector3& operator*=(const float scaler) noexcept {
        set(::Vector3Scale(*this, scaler));

        return *this;
    }

    /**
     * Divide vector by vector
     */
    [[nodiscard]] Vector3 Divide(::Vector3 vector3) const {
        return Vector3{::Vector3Divide(*this, vector3)};
    }

    /**
     * Divide vector by vector
     */
    Vector3 operator/(::Vector3 vector3) const {
        return Vector3{::Vector3Divide(*this, vector3)};
    }

    /**
     * Divide vector by vector
     */
    constexpr Vector3& operator/=(::Vector3 vector3) {
        x /= vector3.x;
        y /= vector3.y;
        z /= vector3.z;

        return *this;
    }

    /**
     * Divide a vector by a value.
     */
    [[nodiscard]] constexpr Vector3 Divide(const float div) const {
        return Vector3{::Vector3{.x = x / div, .y = y / div, .z = z / div}};
    }

    /**
     * Divide a vector by a value.
     */
    [[nodiscard]] constexpr Vector3 operator/(const float div) const {
        return Divide(div);
    }

    /**
     * Divide a vector by a value.
     */
    constexpr Vector3& operator/=(const float div) {
        x /= div;
        y /= div;
        z /= div;

        return *this;
    }

    /**
     * Calculate vector length
     */
    [[nodiscard]] float Length() const noexcept {
        return ::Vector3Length(*this);
    }

    /**
     * Calculate vector square length
     */
    float LengthSqr() const noexcept {
        return ::Vector3LengthSqr(*this);
    }

    [[nodiscard]] Vector3 Normalize() const noexcept {
        return Vector3{::Vector3Normalize(*this)};
    }

    [[nodiscard]] float DotProduct(::Vector3 vector3) const noexcept {
        return ::Vector3DotProduct(*this, vector3);
    }

    [[nodiscard]] float Distance(::Vector3 vector3) const noexcept {
        return ::Vector3Distance(*this, vector3);
    }

    [[nodiscard]] Vector3 Lerp(::Vector3 vector3, float amount) const noexcept {
        return Vector3{::Vector3Lerp(*this, vector3, amount)};
    }

    [[nodiscard]] Vector3 CrossProduct(::Vector3 vector3) const noexcept {
        return Vector3{::Vector3CrossProduct(*this, vector3)};
    }

    [[nodiscard]] Vector3 Perpendicular() const noexcept {
        return Vector3{Vector3Perpendicular(*this)};
    }

    [[nodiscard]] Vector3 Project(const ::Vector3& vector3) const {
        return Vector3{Vector3Project(*this, vector3)};
    }

    [[nodiscard]] Vector3 Reject(const ::Vector3& vector3) const {
        return Vector3{Vector3Reject(*this, vector3)};
    }

    static void OrthoNormalize(::Vector3& v1, ::Vector3& v2) noexcept {
        ::Vector3OrthoNormalize(&v1, &v2);
    }

    [[nodiscard]] Vector3 Transform(const ::Matrix& matrix) const noexcept {
        return Vector3{::Vector3Transform(*this, matrix)};
    }

    [[nodiscard]] Vector3 RotateByQuaternion(::Quaternion quaternion) const noexcept {
        return Vector3{Vector3RotateByQuaternion(*this, quaternion)};
    }

    [[nodiscard]] Vector3 Reflect(::Vector3 normal) const noexcept {
        return Vector3{Vector3Reflect(*this, normal)};
    }

    [[nodiscard]] Vector3 Min(::Vector3 vector3) const noexcept {
        return Vector3{::Vector3Min(*this, vector3)};
    }

    [[nodiscard]] Vector3 Max(::Vector3 vector3) const noexcept {
        return Vector3{::Vector3Max(*this, vector3)};
    }

    [[nodiscard]] Vector3 Barycenter(::Vector3 a, ::Vector3 b, ::Vector3 c) const noexcept {
        return Vector3{::Vector3Barycenter(*this, a, b, c)};
    }

    static Vector3 Zero() noexcept {
        return Vector3{::Vector3Zero()};
    }

    static Vector3 One() noexcept {
        return Vector3{::Vector3One()};
    }
#endif

    void DrawLine3D(const ::Vector3& endPos, ::Color color) const noexcept {
        ::DrawLine3D(*this, endPos, color);
    }

    void DrawPoint3D(::Color color) const noexcept {
        ::DrawPoint3D(*this, color);
    }

    void DrawCircle3D(
            float radius,
            const ::Vector3& rotationAxis,
            float rotationAngle,
            Color color) const noexcept {
        ::DrawCircle3D(*this, radius, rotationAxis, rotationAngle, color);
    }

    void DrawCube(float width, float height, float length, ::Color color) const noexcept {
        ::DrawCube(*this, width, height, length, color);
    }

    void DrawCube(const ::Vector3& size, ::Color color) const noexcept {
        ::DrawCubeV(*this, size, color);
    }

    void DrawCubeWires(float width, float height, float length, ::Color color) const noexcept {
        ::DrawCubeWires(*this, width, height, length, color);
    }

    void DrawCubeWires(const ::Vector3& size, ::Color color) const noexcept {
        ::DrawCubeWiresV(*this, size, color);
    }

    void DrawSphere(float radius, ::Color color) const noexcept {
        ::DrawSphere(*this, radius, color);
    }

    void DrawSphere(float radius, int rings, int slices, ::Color color) const noexcept {
        ::DrawSphereEx(*this, radius, rings, slices, color);
    }

    void DrawSphereWires(float radius, int rings, int slices, ::Color color) const noexcept {
        ::DrawSphereWires(*this, radius, rings, slices, color);
    }

    void DrawCylinder(float radiusTop, float radiusBottom, float height,
            int slices, ::Color color) const noexcept {
        ::DrawCylinder(*this, radiusTop, radiusBottom, height, slices, color);
    }

    void DrawCylinderWires(float radiusTop, float radiusBottom, float height,
            int slices, ::Color color) const noexcept {
        ::DrawCylinderWires(*this, radiusTop, radiusBottom, height, slices, color);
    }

    void DrawPlane(::Vector2 size, ::Color color) const noexcept {
        ::DrawPlane(*this, size, color);
    }

    /**
     * Detect collision between two spheres
     */
    [[nodiscard]] bool CheckCollision(float radius1, const ::Vector3& center2, float radius2) const noexcept {
        return ::CheckCollisionSpheres(*this, radius1, center2, radius2);
    }

 protected:
    constexpr void set(const ::Vector3& vec) noexcept {
        x = vec.x;
        y = vec.y;
        z = vec.z;
    }
};
}  // namespace raylib

using RVector3 = raylib::Vector3;

#endif  // RAYLIB_CPP_INCLUDE_VECTOR3_HPP_
