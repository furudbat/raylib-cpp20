#ifndef RAYLIB_CPP_INCLUDE_RAYCOLLISION_HPP_
#define RAYLIB_CPP_INCLUDE_RAYCOLLISION_HPP_

#include "raylib.hpp"
#include "raylib-cpp-utils.hpp"

namespace raylib {
/**
 * Raycast hit information
 */
class RayCollision : public ::RayCollision {
 public:
    explicit constexpr RayCollision(const ::RayCollision& ray) {
        set(ray);
    }

    [[deprecated("Use RayCollision(RayCollisionHit, ...)")]]
    constexpr RayCollision(bool pHit, float pDistance, ::Vector3 pPoint, ::Vector3 pNormal) : ::RayCollision{pHit, pDistance, pPoint, pNormal} {
        // Nothing.
    }

    enum class RayCollisionHit : bool { Miss = false, Hit = true };
    constexpr RayCollision(RayCollisionHit pHit, float pDistance,
        ::Vector3 pPoint, ::Vector3 pNormal) : ::RayCollision{pHit == RayCollisionHit::Hit, pDistance, pPoint, pNormal} {
        // Nothing.
    }

    /**
     * Get collision info between ray and bounding box
     */
    RayCollision(const ::Ray& ray, const ::BoundingBox& box) {
        set(::GetRayCollisionBox(ray, box));
    }

    /**
     * Get collision info between ray and mesh
     */
    RayCollision(const ::Ray& ray, const ::Mesh& mesh, const ::Matrix& transform) {
        set(::GetRayCollisionMesh(ray, mesh, transform));
    }

    /**
     * Get collision info between ray and quad
     */
    RayCollision(const ::Ray& ray, ::Vector3 p1, ::Vector3 p2, ::Vector3 p3, ::Vector3 p4) {
        set(::GetRayCollisionQuad(ray, p1, p2, p3, p4));
    }

    /**
     * Get collision info between ray and sphere
     */
    RayCollision(const ::Ray& ray, ::Vector3 center, float radius) {
        set(::GetRayCollisionSphere(ray, center, radius));
    }

    /**
     * Get collision info between ray and triangle
     */
    RayCollision(const ::Ray& ray, ::Vector3 p1, ::Vector3 p2, ::Vector3 p3) {
        set(::GetRayCollisionTriangle(ray, p1, p2, p3));
    }

    constexpr RayCollision& operator=(const ::RayCollision& ray) {
        set(ray);
        return *this;
    }

    /*
    explicit(false) operator ::RayCollision() const {
        return *this;
    }
    */

    GETTER(bool, IsHit, hit)
    /** Retrieves the Hit value for the object. @return The Hit value of the object. */
    [[nodiscard]] constexpr RayCollisionHit GetHit() const & { return hit ? RayCollisionHit::Hit : RayCollisionHit::Miss; }
    /** Sets the Hit value for the object. @param value The value of which to set Hit to. */
    constexpr void SetHit(RayCollisionHit value) { hit = value == RayCollisionHit::Hit; }
    GETTERSETTER(float, Distance, distance)
    GETTERSETTER(::Vector3, Position, point)
    GETTERSETTER(::Vector3, Normal, normal)

 protected:
    constexpr void set(const ::RayCollision& ray) noexcept {
        hit = ray.hit;
        distance = ray.distance;
        point = ray.point;
        normal = ray.normal;
    }
};
}  // namespace raylib

using RRayCollision = raylib::RayCollision;

#endif  // RAYLIB_CPP_INCLUDE_RAYCOLLISION_HPP_
