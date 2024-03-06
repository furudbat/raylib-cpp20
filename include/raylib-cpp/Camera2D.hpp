#ifndef RAYLIB_CPP_INCLUDE_CAMERA2D_HPP_
#define RAYLIB_CPP_INCLUDE_CAMERA2D_HPP_

#include "./raylib.hpp"
#include "./Vector2.hpp"
#include "./raylib-cpp-utils.hpp"

namespace raylib {
/**
 * Camera2D type, defines a 2d camera
 */
class Camera2D : public ::Camera2D {
 public:
    inline static constexpr float DefaultRotation = 0.0f;
    inline static constexpr float DefaultZoom = 1.0f;

    explicit constexpr Camera2D(const ::Camera2D& camera) {
        set(camera);
    }

    constexpr Camera2D() = default;
    constexpr Camera2D(::Vector2 pOffset, ::Vector2 pTarget,
                       float pRotation = DefaultRotation, float pZoom = DefaultZoom) : ::Camera2D{pOffset, pTarget, pRotation, pZoom} {}

    /*
    explicit(false) operator ::Camera2D() const {
        return *this;
    }
    */

    constexpr Camera2D& operator=(const ::Camera2D& camera) noexcept {
        set(camera);
        return *this;
    }


    GETTERSETTER(::Vector2, Offset, offset)
    GETTERSETTER(::Vector2, Target, target)
    GETTERSETTER(float, Rotation, rotation)
    GETTERSETTER(float, Zoom, zoom)

    Camera2D& BeginMode() noexcept {
        ::BeginMode2D(*this);
        return *this;
    }

    Camera2D& EndMode() noexcept {
        ::EndMode2D();
        return *this;
    }

    /**
     * Returns camera 2d transform matrix
     */
    [[nodiscard]] Matrix GetMatrix() const noexcept {
        return ::GetCameraMatrix2D(*this);
    }

    /**
     * Returns the world space position for a 2d camera screen space position
     */
    [[nodiscard]] Vector2 GetScreenToWorld(::Vector2 position) const noexcept {
        return ::GetScreenToWorld2D(position, *this);
    }

    /**
     * Returns the screen space position for a 3d world space position
     */
    [[nodiscard]] Vector2 GetWorldToScreen(::Vector2 position) const noexcept {
        return ::GetWorldToScreen2D(position, *this);
    }

 protected:
    constexpr void set(const ::Camera2D& camera) noexcept {
        offset = camera.offset;
        target = camera.target;
        rotation = camera.rotation;
        zoom = camera.zoom;
    }
};
}  // namespace raylib

using RCamera2D = raylib::Camera2D;

#endif  // RAYLIB_CPP_INCLUDE_CAMERA2D_HPP_
