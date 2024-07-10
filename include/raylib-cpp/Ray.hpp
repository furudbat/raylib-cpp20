#ifndef RAYLIB_CPP_INCLUDE_RAY_HPP_
#define RAYLIB_CPP_INCLUDE_RAY_HPP_

#include "raylib.hpp"
#include "raylib-cpp-utils.hpp"
#include "RayCollision.hpp"

namespace raylib {
/**
 * Ray type (useful for raycast)
 */
class Ray : public ::Ray {
 public:
    explicit constexpr Ray(const ::Ray& ray) {
        set(ray);
    }

    explicit constexpr Ray(const ::Vector3& _position = {0.0f, 0.0f, 0.0f}, const ::Vector3& _direction = {0.0f, 0.0f, 0.0f}) :
            ::Ray{_position, _direction} {
        // Nothing.
    }

    Ray(::Vector2 mousePosition, const ::Camera& camera) noexcept {
        set(::GetMouseRay(mousePosition, camera));
    }

    constexpr Ray& operator=(const ::Ray& ray) noexcept {
        set(ray);
        return *this;
    }

    /*
    explicit(false) operator ::Ray() const {
        return m_data;
    }
    */

    GETTERSETTER(::Vector3, Position, position)
    GETTERSETTER(::Vector3, Direction, direction)

    /**
     * Draw a ray line
     */
    void Draw(::Color color) const noexcept {
        ::DrawRay(*this, color);
    }

    /**
     * Get collision information between ray and sphere
     */
    [[nodiscard]] RayCollision GetCollision(::Vector3 center, float radius) const noexcept {
        return RayCollision{::GetRayCollisionSphere(*this, center, radius)};
    }

    /**
     * Detect collision between ray and box
     */
    [[nodiscard]] RayCollision GetCollision(const ::BoundingBox& box) const noexcept {
        return RayCollision{::GetRayCollisionBox(*this, box)};
    }

    /**
     * Get collision information between ray and mesh
     */
    [[nodiscard]] RayCollision GetCollision(const ::Mesh& mesh, const ::Matrix& transform) const noexcept {
        return RayCollision{::GetRayCollisionMesh(*this, mesh, transform)};
    }

    /**
     * Get collision info between ray and triangle
     */
    [[nodiscard]] RayCollision GetCollision(const ::Vector3& p1, const ::Vector3& p2, const ::Vector3& p3) const noexcept {
        return RayCollision{::GetRayCollisionTriangle(*this, p1, p2, p3)};
    }

    /**
     * Get collision info between ray and quad
     */
    [[nodiscard]] RayCollision GetCollision(const ::Vector3& p1, const ::Vector3& p2, const ::Vector3& p3, const ::Vector3& p4) const noexcept {
        return RayCollision{::GetRayCollisionQuad(*this, p1, p2, p3, p4)};
    }

    /**
     * Get a ray trace from mouse position
     */
    static Ray GetMouse(::Vector2 mousePosition, const ::Camera& camera) noexcept {
        return Ray{::GetMouseRay(mousePosition, camera)};
    }

    /**
     * Get a ray trace from mouse position
     */
    static Ray GetMouse(const ::Camera& camera) noexcept {
        return Ray{::GetMouseRay(::GetMousePosition(), camera)};
    }

 protected:
    constexpr void set(const ::Ray& ray) noexcept {
        position = ray.position;
        direction = ray.direction;
    }
};

}  // namespace raylib

using RRay = raylib::Ray;

#endif  // RAYLIB_CPP_INCLUDE_RAY_HPP_
