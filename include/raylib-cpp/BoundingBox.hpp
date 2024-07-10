#ifndef RAYLIB_CPP_INCLUDE_BOUNDINGBOX_HPP_
#define RAYLIB_CPP_INCLUDE_BOUNDINGBOX_HPP_

#include "raylib.hpp"
#include "raylib-cpp-utils.hpp"

namespace raylib {
/**
 * Bounding box type
 */
class BoundingBox : public ::BoundingBox {
 public:
    /*
     * Copy a bounding box from another bounding box.
     */
    explicit constexpr BoundingBox(const ::BoundingBox& box) : ::BoundingBox{box.min, box.max} {
        // Nothing.
    }

    /**
     * Compute mesh bounding box limits
     */
    explicit BoundingBox(const ::Mesh& mesh) : BoundingBox() {
        set(::GetMeshBoundingBox(mesh));
    }

    explicit constexpr BoundingBox(::Vector3 minMax = ::Vector3{0.0f, 0.0f, 0.0f}) : ::BoundingBox{minMax, minMax} {}
    constexpr BoundingBox(::Vector3 pMin, ::Vector3 pMax) : ::BoundingBox{pMin, pMax} {}

    /*
    explicit(false) operator ::BoundingBox() const {
        return *this;
    }
    */

    constexpr BoundingBox& operator=(const ::BoundingBox& box) noexcept {
        set(box);
        return *this;
    }

    GETTERSETTER(::Vector3, Min, min)
    GETTERSETTER(::Vector3, Max, max)

    /**
     * Draw a bounding box with wires
     */
    void Draw(::Color color = WHITE) const noexcept {
        ::DrawBoundingBox(*this, color);
    }

    /**
     * Detect collision between two boxes
     */
    [[nodiscard]] bool CheckCollision(const ::BoundingBox& box2) const noexcept {
        return ::CheckCollisionBoxes(*this, box2);
    }

    /**
     * Detect collision between box and sphere
     */
    [[nodiscard]] bool CheckCollision(::Vector3 center, float radius) const noexcept {
        return ::CheckCollisionBoxSphere(*this, center, radius);
    }

    /**
     * Detect collision between ray and bounding box
     */
    [[nodiscard]] bool CheckCollision(const ::Ray& ray) const noexcept {
        return ::GetRayCollisionBox(ray, *this).hit;
    }

    /**
     * Get collision information between ray and bounding box
     */
    [[nodiscard]] RayCollision GetCollision(const ::Ray& ray) const noexcept {
        return ::GetRayCollisionBox(ray, *this);
    }

 protected:
    constexpr void set(const ::BoundingBox& box) noexcept {
        this->min = box.min;
        this->max = box.max;
    }
    constexpr void set(const ::Vector3& _min, const ::Vector3& _max) noexcept {
        this->min = _min;
        this->max = _max;
    }
};
}  // namespace raylib

using RBoundingBox = raylib::BoundingBox;

#endif  // RAYLIB_CPP_INCLUDE_BOUNDINGBOX_HPP_
