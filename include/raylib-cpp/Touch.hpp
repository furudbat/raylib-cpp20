#ifndef RAYLIB_CPP_INCLUDE_TOUCH_HPP_
#define RAYLIB_CPP_INCLUDE_TOUCH_HPP_

#include "raylib.hpp"

namespace raylib {
/**
 * Input-related functions: touch
 */
namespace Touch {
    /**
     * Get touch position X for touch point 0 (relative to screen size)
     */
    [[maybe_unused]] RLCPPAPI_INLINE int GetX() noexcept {
        return ::GetTouchX();
    }

    /**
     * Get touch position Y for touch point 0 (relative to screen size)
     */
    [[maybe_unused]] RLCPPAPI_INLINE int GetY() noexcept {
        return ::GetTouchY();
    }

    /**
     * Get touch position XY for a touch point index (relative to screen size)
     */
    [[maybe_unused]] RLCPPAPI_INLINE Vector2 GetPosition(int index) noexcept {
        return ::GetTouchPosition(index);
    }

    /**
     * Get touch point identifier for given index
     */
    [[maybe_unused]] RLCPPAPI_INLINE int GetPointId(int index) noexcept {
        return ::GetTouchPointId(index);
    }

    /**
     * Get number of touch points
     */
    [[maybe_unused]] RLCPPAPI_INLINE int GetPointCount() noexcept {
        return ::GetTouchPointCount();
    }
}  // namespace Touch
}  // namespace raylib

namespace RTouch = raylib::Touch;

#endif  // RAYLIB_CPP_INCLUDE_TOUCH_HPP_
