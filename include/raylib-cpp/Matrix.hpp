#ifndef RAYLIB_CPP_INCLUDE_MATRIX_HPP_
#define RAYLIB_CPP_INCLUDE_MATRIX_HPP_

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "./raymath.hpp"

#ifndef RAYLIB_CPP_NO_MATH
#include <cmath>
#endif

namespace raylib {
/**
 * Matrix type (OpenGL style 4x4 - right handed, column major)
 */
class Matrix : public ::Matrix {
 public:
    constexpr Matrix(const ::Matrix& mat = {
            .m0 = 0, .m4 = 0, .m8 = 0, .m12 = 0,
            .m1 = 0, .m5 = 0, .m9 = 0, .m13 = 0,
            .m2 = 0, .m6 = 0, .m10 = 0, .m14 = 0,
            .m3 = 0, .m7 = 0, .m11 = 0, .m15 = 0
    }) : ::Matrix{
            mat.m0, mat.m4, mat.m8, mat.m12,
            mat.m1, mat.m5, mat.m9, mat.m13,
            mat.m2, mat.m6, mat.m10, mat.m14,
            mat.m3, mat.m7, mat.m11, mat.m15} {
        // Nothing.
    }

    [[deprecated("Use Matrix(mat), use named-parameters with struct")]]
    explicit constexpr Matrix(
            float _m0 = 0, float _m4 = 0, float _m8 = 0, float _m12 = 0,
            float _m1 = 0, float _m5 = 0, float _m9 = 0, float _m13 = 0,
            float _m2 = 0, float _m6 = 0, float _m10 = 0, float _m14 = 0,
            float _m3 = 0, float _m7 = 0, float _m11 = 0, float _m15 = 0) :
        ::Matrix{
            _m0, _m4, _m8, _m12,
            _m1, _m5, _m9, _m13,
            _m2, _m6, _m10, _m14,
            _m3, _m7, _m11, _m15} {
        // Nothing.
    }

    constexpr Matrix& operator=(const ::Matrix& matrix) {
        set(matrix);
        return *this;
    }
    constexpr Matrix& operator=(const Matrix& matrix) {
        set(matrix);
        return *this;
    }

    constexpr bool operator==(const ::Matrix& other) {
        return m0 == other.m0
            && m1 == other.m1
            && m2 == other.m2
            && m3 == other.m3
            && m4 == other.m4
            && m5 == other.m5
            && m6 == other.m6
            && m7 == other.m7
            && m8 == other.m8
            && m9 == other.m9
            && m10 == other.m10
            && m11 == other.m11
            && m12 == other.m12
            && m13 == other.m13
            && m14 == other.m14
            && m15 == other.m15;
    }

    constexpr bool operator!=(const ::Matrix& other) {
        return !(*this == other);
    }

    /*
    explicit(false) operator ::Matrix() const {
        return m_data;
    }
    */

    GETTERSETTER(float, M0, m0)
    GETTERSETTER(float, M1, m1)
    GETTERSETTER(float, M2, m2)
    GETTERSETTER(float, M3, m3)
    GETTERSETTER(float, M4, m4)
    GETTERSETTER(float, M5, m5)
    GETTERSETTER(float, M6, m6)
    GETTERSETTER(float, M7, m7)
    GETTERSETTER(float, M8, m8)
    GETTERSETTER(float, M9, m9)
    GETTERSETTER(float, M10, m10)
    GETTERSETTER(float, M11, m11)
    GETTERSETTER(float, M12, m12)
    GETTERSETTER(float, M13, m13)
    GETTERSETTER(float, M14, m14)
    GETTERSETTER(float, M15, m15)

#ifndef RAYLIB_CPP_NO_MATH
    /**
     * Returns the trace of the matrix (sum of the values along the diagonal)
     */
    [[nodiscard]] float Trace() const {
        return ::MatrixTrace(*this);
    }

    /**
     * Transposes provided matrix
     */
    [[nodiscard]] Matrix Transpose() const {
        return ::MatrixTranspose(*this);
    }

    [[nodiscard]] Matrix Invert() const {
        return ::MatrixInvert(*this);
    }

    static Matrix Identity() {
        return ::MatrixIdentity();
    }

    Matrix Add(const ::Matrix& right) {
        return ::MatrixAdd(*this, right);
    }

    Matrix operator+(const ::Matrix& matrix) {
            return ::MatrixAdd(*this, matrix);
    }

    Matrix Subtract(const ::Matrix& right) {
        return ::MatrixSubtract(*this, right);
    }

    Matrix operator-(const ::Matrix& matrix) {
        return ::MatrixSubtract(*this, matrix);
    }

    static Matrix Translate(float x, float y, float z) {
        return ::MatrixTranslate(x, y, z);
    }

    static Matrix Rotate(Vector3 axis, float angle) {
        return ::MatrixRotate(axis, angle);
    }

    static Matrix RotateXYZ(Vector3 angle) {
        return ::MatrixRotateXYZ(angle);
    }

    static Matrix RotateX(float angle) {
        return ::MatrixRotateX(angle);
    }

    static Matrix RotateY(float angle) {
        return ::MatrixRotateY(angle);
    }

    static Matrix RotateZ(float angle) {
        return ::MatrixRotateZ(angle);
    }

    static Matrix Scale(float x, float y, float z) {
        return ::MatrixScale(x, y, z);
    }

    [[nodiscard]] Matrix Multiply(const ::Matrix& right) const {
        return ::MatrixMultiply(*this, right);
    }

    Matrix operator*(const ::Matrix& matrix) {
        return ::MatrixMultiply(*this, matrix);
    }

    static Matrix Frustum(double left, double right, double bottom, double top,
            double near, double far) {
        return ::MatrixFrustum(left, right, bottom, top, near, far);
    }

    static Matrix Perspective(double fovy, double aspect, double near, double far) {
        return ::MatrixPerspective(fovy, aspect, near, far);
    }

    static Matrix Ortho(double left, double right, double bottom, double top,
            double near, double far) {
        return ::MatrixOrtho(left, right, bottom, top, near, far);
    }

    static Matrix LookAt(Vector3 eye, Vector3 target, Vector3 up) {
        return ::MatrixLookAt(eye, target, up);
    }

    [[nodiscard]] float16 ToFloatV() const {
        return ::MatrixToFloatV(*this);
    }

    explicit operator float16() const {
        return ToFloatV();
    }

    /**
     * Set shader uniform value (matrix 4x4)
     */
    /*
    Matrix& SetShaderValue(const ::Shader& shader, int uniformLoc) {
        ::SetShaderValueMatrix(shader, uniformLoc, *this);
        return *this;
    }
    Matrix& SetShaderValue(const raylib::Shader& shader, int uniformLoc) {
        ::SetShaderValueMatrix(shader.c_raylib(), uniformLoc, *this);
        return *this;
    }
    */

    static Matrix FromCamera(const ::Camera& camera) {
        return ::GetCameraMatrix(camera);
    }

    static Matrix FromCamera(const ::Camera2D& camera) {
        return ::GetCameraMatrix2D(camera);
    }

#endif

 protected:
    constexpr void set(const ::Matrix& mat) noexcept {
        m0 = mat.m0;
        m1 = mat.m1;
        m2 = mat.m2;
        m3 = mat.m3;
        m4 = mat.m4;
        m5 = mat.m5;
        m6 = mat.m6;
        m7 = mat.m7;
        m8 = mat.m8;
        m9 = mat.m9;
        m10 = mat.m10;
        m11 = mat.m11;
        m12 = mat.m12;
        m13 = mat.m13;
        m14 = mat.m14;
        m15 = mat.m15;
    }
};

}  // namespace raylib

using RMatrix = raylib::Matrix;

#endif  // RAYLIB_CPP_INCLUDE_MATRIX_HPP_
