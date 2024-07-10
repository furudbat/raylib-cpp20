#ifndef RAYLIB_CPP_INCLUDE_AUDIODEVICE_HPP_
#define RAYLIB_CPP_INCLUDE_AUDIODEVICE_HPP_

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#ifdef __cpp_exceptions
#include "./RaylibException.hpp"
#endif
#include "./RaylibError.hpp"

namespace raylib {

enum class AudioDeviceInitOption : bool {
    LateInit = false,
    CallInit
};

/**
 * Audio device management functions.
 */
class AudioDevice {
 public:
    /**
     * Initialize audio device and context.
     *
     * @param lateInit Whether or not to post-pone initializing the context.
     *
     * @throws raylib::RaylibException Throws if the AudioDevice failed to initialize.
     */
    [[deprecated("Use AudioDevice(options)")]] explicit AudioDevice(bool lateInit) RAYLIB_CPP_THROWS {
        if (!lateInit) {
            Init();
        }
    }

#ifdef RAYLIB_CPP_EXPECTED
    [[deprecated("Use AudioDevice() and Init() to handle error")]]
#endif
    explicit AudioDevice(AudioDeviceInitOption options) RAYLIB_CPP_THROWS {
        if (options == AudioDeviceInitOption::CallInit) {
            Init();
        }
    }

    constexpr AudioDevice() = default;

    /**
     * Close the audio device and context.
     */
    ~AudioDevice() noexcept {
        if (IsReady()) {
            Close();
        }
    }

    /**
     * Initialize audio device and context.
     *
     * @throws raylib::RaylibException Throws if the AudioDevice failed to initialize.
     */
        RAYLIB_CPP_EXPECTED_RESULT_VOID Init() RAYLIB_CPP_THROWS {
        ::InitAudioDevice();
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to initialize AudioDevice"));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Close the audio device and context.
     */
    void Close() noexcept {
        ::CloseAudioDevice();
    }

    /**
     * Check if audio device has been initialized successfully.
     */
    [[nodiscard]] bool IsReady() const noexcept {
        return ::IsAudioDeviceReady();
    }

    /**
     * Set master volume (listener).
     *
     * @param volume The desired volume to set.
     */
    AudioDevice& SetVolume(float volume) noexcept {
        ::SetMasterVolume(volume);
        return *this;
    }
};

}  // namespace raylib

using RAudioDevice = raylib::AudioDevice;

#endif  // RAYLIB_CPP_INCLUDE_AUDIODEVICE_HPP_
