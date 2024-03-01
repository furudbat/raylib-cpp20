#ifndef RAYLIB_CPP_INCLUDE_BOUNDINGBOX_HPP_
#define RAYLIB_CPP_INCLUDE_BOUNDINGBOX_HPP_

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"

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

    explicit constexpr BoundingBox(::Vector3 minMax = ::Vector3{0.0F, 0.0F, 0.0F}) : ::BoundingBox{minMax, minMax} {}
    constexpr BoundingBox(::Vector3 pMin, ::Vector3 pMax) : ::BoundingBox{pMin, pMax} {}

    GETTERSETTER(::Vector3, Min, min)
    GETTERSETTER(::Vector3, Max, max)

    constexpr BoundingBox& operator=(const ::BoundingBox& box) {
        set(box);
        return *this;
    }

    /**
     * Draw a bounding box with wires
     */
    void Draw(::Color color = WHITE) const {
        ::DrawBoundingBox(*this, color);
    }

    /**
     * Detect collision between two boxes
     */
    [[nodiscard]] bool CheckCollision(const ::BoundingBox& box2) const {
        return ::CheckCollisionBoxes(*this, box2);
    }

    /**
     * Detect collision between box and sphere
     */
    [[nodiscard]] bool CheckCollision(::Vector3 center, float radius) const {
        return ::CheckCollisionBoxSphere(*this, center, radius);
    }

    /**
     * Detect collision between ray and bounding box
     */
    [[nodiscard]] bool CheckCollision(const ::Ray& ray) const {
        return ::GetRayCollisionBox(ray, *this).hit;
    }

    /**
     * Get collision information between ray and bounding box
     */
    [[nodiscard]] RayCollision GetCollision(const ::Ray& ray) const {
        return ::GetRayCollisionBox(ray, *this);
    }

 protected:
    constexpr void set(const ::BoundingBox& box) {
        min = box.min;
        max = box.max;
    }
    constexpr void set(const ::Vector3& _min, const ::Vector3& _max) {
        min = _min;
        max = _max;
    }
};
}  // namespace raylib

using RBoundingBox = raylib::BoundingBox;

#endif  // RAYLIB_CPP_INCLUDE_BOUNDINGBOX_HPP_
