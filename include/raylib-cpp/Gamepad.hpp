#ifndef RAYLIB_CPP_INCLUDE_GAMEPAD_HPP_
#define RAYLIB_CPP_INCLUDE_GAMEPAD_HPP_

#include "raylib.hpp"
#include "raylib-cpp-utils.hpp"

#include <string>

namespace raylib {
/**
 * Input-related functions: gamepads
 */
class Gamepad {
 public:
    inline static constexpr int DefaultGamepadNumber = 0;

    int number;

    explicit constexpr Gamepad(int gamepadNumber = DefaultGamepadNumber) {
        set(gamepadNumber);
    }

    GETTERSETTER(int, Number, number)

    constexpr Gamepad& operator=(const Gamepad& gamepad) noexcept {
        set(gamepad.number);
        return *this;
    }

    constexpr Gamepad& operator=(int gamepadNumber) noexcept {
        set(gamepadNumber);
        return *this;
    }

    explicit constexpr operator int() const { return number; }

    /**
     * Detect if a gamepad is available
     */
    [[nodiscard]] bool IsAvailable() const noexcept {
        return ::IsGamepadAvailable(number);
    }

    /**
     * Detect if a gamepad is available
     */
    static bool IsAvailable(int number) noexcept {
        return ::IsGamepadAvailable(number);
    }

    /**
     * Return gamepad internal name id
     */
    [[nodiscard]] std::string GetName() const {
        return ::GetGamepadName(number);
    }

    /**
     * Return gamepad internal name id
     */
    explicit operator std::string() const {
        return GetName();
    }

    /**
     * Detect if a gamepad button has been pressed once
     */
    [[nodiscard]] bool IsButtonPressed(int button) const noexcept {
        return ::IsGamepadButtonPressed(number, button);
    }

    /**
     * Detect if a gamepad button is being pressed
     */
    [[nodiscard]] bool IsButtonDown(int button) const noexcept {
        return ::IsGamepadButtonDown(number, button);
    }

    /**
     * Detect if a gamepad button has been released once
     */
    [[nodiscard]] bool IsButtonReleased(int button) const noexcept {
        return ::IsGamepadButtonReleased(number, button);
    }

    /**
     * Detect if a gamepad button is NOT being pressed
     */
    [[nodiscard]] bool IsButtonUp(int button) const noexcept {
        return ::IsGamepadButtonUp(number, button);
    }

    /**
     * Get the last gamepad button pressed
     */
    [[nodiscard]] static int GetButtonPressed() noexcept {
        return ::GetGamepadButtonPressed();
    }

    /**
     * Return gamepad axis count for a gamepad
     */
    [[nodiscard]] int GetAxisCount() const noexcept {
        return ::GetGamepadAxisCount(number);
    }

    /**
     * Return axis movement value for a gamepad axis
     */
    [[nodiscard]] float GetAxisMovement(int axis) const noexcept {
        return ::GetGamepadAxisMovement(number, axis);
    }

    static int SetMappings(const std::string& mappings) noexcept {
        return ::SetGamepadMappings(mappings.c_str());
    }

 protected:
    constexpr void set(int gamepadNumber) noexcept {
        number = gamepadNumber;
    }
};
}  // namespace raylib

using RGamepad = raylib::Gamepad;

#endif  // RAYLIB_CPP_INCLUDE_GAMEPAD_HPP_
