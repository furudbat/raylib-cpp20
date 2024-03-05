#ifndef RAYLIB_CPP_INCLUDE_RAY_HPP_
#define RAYLIB_CPP_INCLUDE_RAY_HPP_

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "./RayCollision.hpp"

namespace raylib {
/**
 * Ray type (useful for raycast)
 */
class Ray : public ::Ray {
 public:
    explicit constexpr Ray(const ::Ray& ray) {
        set(ray);
    }

    explicit constexpr Ray(const ::Vector3& _position = {0.0F, 0.0F, 0.0F}, const ::Vector3& _direction = {0.0F, 0.0F, 0.0F}) :
            ::Ray{_position, _direction} {
        // Nothing.
    }

    Ray(::Vector2 mousePosition, const ::Camera& camera) {
        set(::GetMouseRay(mousePosition, camera));
    }

    constexpr Ray& operator=(const ::Ray& ray) {
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
    void Draw(::Color color) const {
        DrawRay(*this, color);
    }

    /**
     * Get collision information between ray and sphere
     */
    [[nodiscard]] RayCollision GetCollision(::Vector3 center, float radius) const {
        return RayCollision{::GetRayCollisionSphere(*this, center, radius)};
    }

    /**
     * Detect collision between ray and box
     */
    [[nodiscard]] RayCollision GetCollision(const ::BoundingBox& box) const {
        return RayCollision{::GetRayCollisionBox(*this, box)};
    }

    /**
     * Get collision information between ray and mesh
     */
    [[nodiscard]] RayCollision GetCollision(const ::Mesh& mesh, const ::Matrix& transform) const {
        return RayCollision{::GetRayCollisionMesh(*this, mesh, transform)};
    }

    /**
     * Get collision info between ray and triangle
     */
    [[nodiscard]] RayCollision GetCollision(::Vector3 p1, ::Vector3 p2, ::Vector3 p3) const {
        return RayCollision{::GetRayCollisionTriangle(*this, p1, p2, p3)};
    }

    /**
     * Get collision info between ray and quad
     */
    [[nodiscard]] RayCollision GetCollision(::Vector3 p1, ::Vector3 p2, ::Vector3 p3, ::Vector3 p4) const {
        return RayCollision{::GetRayCollisionQuad(*this, p1, p2, p3, p4)};
    }

    /**
     * Get a ray trace from mouse position
     */
    static Ray GetMouse(::Vector2 mousePosition, const ::Camera& camera) {
        return Ray{::GetMouseRay(mousePosition, camera)};
    }

    /**
     * Get a ray trace from mouse position
     */
    static Ray GetMouse(const ::Camera& camera) {
        return Ray{::GetMouseRay(::GetMousePosition(), camera)};
    }

 protected:
    constexpr void set(const ::Ray& ray) {
        position = ray.position;
        direction = ray.direction;
    }
};

}  // namespace raylib

using RRay = raylib::Ray;

#endif  // RAYLIB_CPP_INCLUDE_RAY_HPP_
