#ifndef RAYLIB_CPP_INCLUDE_CAMERA3D_HPP_
#define RAYLIB_CPP_INCLUDE_CAMERA3D_HPP_

#include "./raylib.hpp"
#include "./Vector3.hpp"
#include "./Ray.hpp"
#include "./raylib-cpp-utils.hpp"

namespace raylib {
/**
 * Camera type, defines a camera position/orientation in 3d space
 */
class Camera3D : public ::Camera3D {
 public:
    explicit constexpr Camera3D(const ::Camera3D& camera) {
        set(camera);
    }

    /**
     * Create a new Camera3D.
     *
     * @param position Camera position
     * @param target Camera target it looks-at
     * @param up Camera up vector (rotation over its axis)
     * @param fovy Camera field-of-view apperture in Y (degrees) in perspective, used as near plane width in orthographic
     * @param projection Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC
     */
    explicit constexpr Camera3D(const ::Vector3& _position,
            const ::Vector3& _target = ::Vector3{0.0F, 0.0F, 0.0F},
            const ::Vector3& _up = ::Vector3{0.0F, 1.0F, 0.0F},
            float _fovy = 0.0F,
            int _projection = CAMERA_PERSPECTIVE) : ::Camera3D{_position, _target, _up, _fovy, _projection} {}

    Camera3D() = default;

    /*
    explicit(false) operator ::Camera3D() const {
        return *this;
    }
    */

    constexpr Camera3D& operator=(const ::Camera3D& camera) {
        set(camera);
        return *this;
    }

    GETTERSETTER(::Vector3, Position, position)
    GETTERSETTER(::Vector3, Target, target)
    GETTERSETTER(::Vector3, Up, up)
    GETTERSETTER(float, Fovy, fovy)
    GETTERSETTER(int, Projection, projection)

    /**
     * Initializes 3D mode with custom camera (3D)
     */
    Camera3D& BeginMode() {
        ::BeginMode3D(*this);
        return *this;
    }

    /**
     * Ends 3D mode and returns to default 2D orthographic mode
     */
    Camera3D& EndMode() {
        ::EndMode3D();
        return *this;
    }

    /**
     * Get camera transform matrix (view matrix)
     */
    [[nodiscard]] Matrix GetMatrix() const {
        return ::GetCameraMatrix(*this);
    }

    /**
     * Update camera position for selected mode
     */
    Camera3D& Update(int mode) {
        ::UpdateCamera(this, mode);
        return *this;
    }

    /**
     * Update camera movement/rotation
     */
    Camera3D& Update(const ::Vector3& movement, const ::Vector3& rotation, float zoom = 1.0F) {
        ::UpdateCameraPro(this, movement, rotation, zoom);
        return *this;
    }

    /**
     * Returns a ray trace from mouse position
     */
    [[nodiscard]] raylib::Ray GetMouseRay(::Vector2 mousePosition) const {
        return raylib::Ray{::GetMouseRay(mousePosition, *this)};
    }

    /**
     * Returns the screen space position for a 3d world space position
     */
    [[nodiscard]] raylib::Vector2 GetWorldToScreen(const ::Vector3& _position) const {
        return raylib::Vector2{::GetWorldToScreen(_position, *this)};
    }

    /**
     * Draw a billboard texture.
     */
    void DrawBillboard(
            const ::Texture2D& texture,
            const ::Vector3& center,
            float size,
            ::Color tint = {255, 255, 255, 255}) const {
        ::DrawBillboard(*this, texture, center, size, tint);
    }

    /**
     * Draw a billboard texture defined by source.
     */
    void DrawBillboard(
            const ::Texture2D& texture,
            ::Rectangle sourceRec,
            ::Vector3 center,
            ::Vector2 size,
            ::Color tint = {255, 255, 255, 255}) const {
        ::DrawBillboardRec(*this, texture, sourceRec, center, size, tint);
    }

 protected:
    constexpr void set(const ::Camera3D& camera) noexcept {
        position = camera.position;
        target = camera.target;
        up = camera.up;
        fovy = camera.fovy;
        projection = camera.projection;
    }
};

using Camera = Camera3D;

}  // namespace raylib

using RCamera = raylib::Camera;
using RCamera3D = raylib::Camera3D;

#endif  // RAYLIB_CPP_INCLUDE_CAMERA3D_HPP_
