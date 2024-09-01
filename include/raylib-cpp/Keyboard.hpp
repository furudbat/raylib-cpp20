#ifndef RAYLIB_CPP_INCLUDE_KEYBOARD_HPP_
#define RAYLIB_CPP_INCLUDE_KEYBOARD_HPP_

#include "raylib.hpp"

namespace raylib {
/**
 * Input-related functions: keyboard
 */
namespace Keyboard {
    /**
     * Detect if a key has been pressed once
     */
    [[maybe_unused]] RLCPPAPI_INLINE bool IsKeyPressed(int key) {
        return ::IsKeyPressed(key);
    }

    /**
     * Detect if a key has been pressed again (Only PLATFORM_DESKTOP)
     */
    [[maybe_unused]] RLCPPAPI_INLINE bool IsKeyPressedRepeat(int key) {
        return ::IsKeyPressedRepeat(key);
    }

    /**
     * Detect if a key is being pressed
     */
    [[maybe_unused]] RLCPPAPI_INLINE bool IsKeyDown(int key) {
        return ::IsKeyDown(key);
    }

    /**
     * Detect if a key has been released once
     */
    [[maybe_unused]] RLCPPAPI_INLINE bool IsKeyReleased(int key) {
        return ::IsKeyReleased(key);
    }

    /**
     * Detect if a key is NOT being pressed
     */
    [[maybe_unused]] RLCPPAPI_INLINE bool IsKeyUp(int key) {
        return ::IsKeyUp(key);
    }

    /**
     * Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
     */
    [[maybe_unused]] RLCPPAPI_INLINE int GetKeyPressed() {
        return ::GetKeyPressed();
    }

    /**
     * Get char pressed (unicode), call it multiple times for chars queued, returns 0 when the queue is empty
     */
    [[maybe_unused]] RLCPPAPI_INLINE int GetCharPressed() {
        return ::GetCharPressed();
    }
}  // namespace Keyboard
}  // namespace raylib
namespace RKeyboard = raylib::Keyboard;

#endif  // RAYLIB_CPP_INCLUDE_KEYBOARD_HPP_
