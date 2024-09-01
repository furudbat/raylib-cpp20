#ifndef RAYLIB_CPP_INCLUDE_MOUSE_HPP_
#define RAYLIB_CPP_INCLUDE_MOUSE_HPP_

#include "raylib.hpp"
#include "Vector2.hpp"

namespace raylib {
/**
 * Input-related functions: mouse
 */
namespace Mouse {
    /**
     * Detect if a mouse button has been pressed once
     */
    [[maybe_unused]] RLCPPAPI_INLINE bool IsButtonPressed(int button) noexcept {
        return ::IsMouseButtonPressed(button);
    }

    /**
     * Detect if a mouse button is being pressed
     */
    [[maybe_unused]] RLCPPAPI_INLINE bool IsButtonDown(int button) noexcept {
        return ::IsMouseButtonDown(button);
    }

    /**
     * Detect if a mouse button has been released once
     */
    [[maybe_unused]] RLCPPAPI_INLINE bool IsButtonReleased(int button) noexcept {
        return ::IsMouseButtonReleased(button);
    }

    [[maybe_unused]] RLCPPAPI_INLINE bool IsButtonUp(int button) noexcept {
        return ::IsMouseButtonUp(button);
    }

    [[maybe_unused]] RLCPPAPI_INLINE int GetX() noexcept {
        return ::GetMouseX();
    }

    [[maybe_unused]] RLCPPAPI_INLINE int GetY() noexcept {
        return ::GetMouseY();
    }

    [[maybe_unused]] RLCPPAPI_INLINE void SetX(int x) noexcept {
        ::SetMousePosition(x, GetY());
    }

    [[maybe_unused]] RLCPPAPI_INLINE void SetY(int y) noexcept {
        ::SetMousePosition(GetX(), y);
    }

    [[maybe_unused]] RLCPPAPI_INLINE Vector2 GetPosition() noexcept {
        return raylib::Vector2{::GetMousePosition()};
    }

    [[maybe_unused]] RLCPPAPI_INLINE void SetPosition(int x, int y) noexcept {
        ::SetMousePosition(x, y);
    }

    [[maybe_unused]] RLCPPAPI_INLINE void SetPosition(::Vector2 position) noexcept {
        ::SetMousePosition(static_cast<int>(position.x), static_cast<int>(position.y));
    }

    /**
     * Get mouse delta between frames
     */
    [[maybe_unused]] RLCPPAPI_INLINE Vector2 GetDelta() noexcept {
        return raylib::Vector2{::GetMouseDelta()};
    }

    inline static constexpr int DefaultSetOffsetX = 0;
    inline static constexpr int DefaultSetOffsetY = 0;
    [[maybe_unused]] RLCPPAPI_INLINE void SetOffset(int offsetX = DefaultSetOffsetX, int offsetY = DefaultSetOffsetY) noexcept {
        ::SetMouseOffset(offsetX, offsetY);
    }

    [[maybe_unused]] RLCPPAPI_INLINE void SetOffset(::Vector2 offset) noexcept {
        ::SetMouseOffset(static_cast<int>(offset.x), static_cast<int>(offset.y));
    }

    inline static constexpr float DefaultSetScaleX = 1.0f;
    inline static constexpr float DefaultSetScaleY = 1.0f;
    [[maybe_unused]] RLCPPAPI_INLINE void SetScale(float scaleX = DefaultSetScaleX, float scaleY = DefaultSetScaleY) noexcept {
        ::SetMouseScale(scaleX, scaleY);
    }

    [[maybe_unused]] RLCPPAPI_INLINE void SetScale(::Vector2 scale) noexcept {
        ::SetMouseScale(scale.x, scale.y);
    }

    /**
     * Get mouse wheel movement for X or Y, whichever is larger
     */
    [[maybe_unused]] RLCPPAPI_INLINE float GetWheelMove() noexcept {
        return ::GetMouseWheelMove();
    }

    /**
     * Get mouse wheel movement for both X and Y
     *
     * @see ::GetMouseWheelMoveV()
     */
    [[maybe_unused]] RLCPPAPI_INLINE Vector2 GetWheelMoveV() noexcept {
        return Vector2{::GetMouseWheelMoveV()};
    }

    /**
     * Sets the current mouse cursor icon.
     *
     * @see ::MouseCursor
     */
    inline static constexpr int DefaultSetCursor = MOUSE_CURSOR_DEFAULT;
    [[maybe_unused]] RLCPPAPI_INLINE void SetCursor(int cursor = DefaultSetCursor) noexcept {
        ::SetMouseCursor(cursor);
    }

    /**
     * Get touch position X for touch point 0 (relative to screen size)
     */
    [[maybe_unused]] RLCPPAPI_INLINE int GetTouchX() noexcept {
        return ::GetTouchX();
    }

    /**
     * Get touch position Y for touch point 0 (relative to screen size)
     */
    [[maybe_unused]] RLCPPAPI_INLINE int GetTouchY() noexcept {
        return ::GetTouchY();
    }

    /**
     * Get touch position XY for a touch point index (relative to screen size)
     */
    [[maybe_unused]] RLCPPAPI_INLINE Vector2 GetTouchPosition(int index) noexcept {
        return Vector2{::GetTouchPosition(index)};
    }

    /**
     * Get a ray trace from mouse position
     */
    [[maybe_unused]] RLCPPAPI_INLINE Ray GetRay(::Vector2 mousePosition, const ::Camera& camera) noexcept {
        return Ray{::GetMouseRay(mousePosition, camera)};
    }

    /**
     * Get a ray trace from mouse position
     */
    [[maybe_unused]] RLCPPAPI_INLINE Ray GetRay(const ::Camera& camera) noexcept {
        return Ray{::GetMouseRay(::GetMousePosition(), camera)};
    }
}  // namespace Mouse
}  // namespace raylib

namespace RMouse = raylib::Mouse;

#endif  // RAYLIB_CPP_INCLUDE_MOUSE_HPP_
