#ifndef RAYLIB_CPP_INCLUDE_SOUND_HPP_
#define RAYLIB_CPP_INCLUDE_SOUND_HPP_

#include <string>
#include <filesystem>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#ifdef __cpp_exceptions
#include "./RaylibException.hpp"
#endif
#include "./RaylibError.hpp"

namespace raylib {

/**
 * Wave/Sound management functions
 *
 * @code
 * raylib::Sound boom("boom.wav");
 * boom.Play();
 * @endcode
 */
class Sound {
 public:
    constexpr Sound() {
        m_data.stream = { nullptr, nullptr, 0, 0, 0 };
        m_data.frameCount = 0;
    }

    constexpr Sound(owner<::AudioStream> _stream, unsigned int _frameCount) : m_data{_stream, _frameCount} {
        // Nothing.
    }

    constexpr Sound(owner<const ::Sound&>) = delete;
    constexpr Sound(owner<::Sound&&> other) {
        set(other);

        other.stream = { nullptr, nullptr, 0, 0, 0 };
        other.frameCount = 0;
    }

    constexpr Sound(const Sound&) = delete;
    constexpr Sound(Sound&& other) {
        set(other.m_data);

        other.m_data.stream = { nullptr, nullptr, 0, 0, 0 };
        other.m_data.frameCount = 0;
    }

    /**
     * Loads a sound from the given file.
     *
     * @throws raylib::RaylibException Throws if the Sound failed to load.
     */
    explicit Sound(const std::filesystem::path& fileName) {
        Load(fileName);
    }

    /**
     * Loads a sound from the given Wave.
     *
     * @throws raylib::RaylibException Throws if the Sound failed to load.
     */
    explicit Sound(const ::Wave& wave) {
        LoadFromWave(wave);
    }

    ~Sound() {
        Unload();
    }

    explicit operator ::Sound() const {
        return m_data;
    }
    [[nodiscard]] ::Sound c_raylib() const & {
        return m_data;
    }

    GETTERSETTER(unsigned int, FrameCount, m_data.frameCount)
    GETTERSETTER(::AudioStream, Stream, m_data.stream)

    constexpr Sound& operator=(const Sound&) = delete;
    Sound& operator=(Sound&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other.m_data);
        other.m_data.frameCount = 0;
        other.m_data.stream = { nullptr, nullptr, 0, 0, 0 };

        return *this;
    }

    /**
     * Update sound buffer with new data
     */
    Sound& Update(const void *data, int samplesCount) {
        ::UpdateSound(m_data, data, samplesCount);
        return *this;
    }

    /**
     * Update sound buffer with new data, assuming it's the same sample count.
     */
    Sound& Update(const void *data) {
        ::UpdateSound(m_data, data, static_cast<int>(m_data.frameCount));
        return *this;
    }

    /**
     * Unload sound
     */
    void Unload() {
        if (m_data.stream.buffer != nullptr) {
            ::UnloadSound(m_data);
            m_data.stream.buffer = nullptr;
        }
    }

    /**
     * Play a sound
     */
    Sound& Play() {
        ::PlaySound(m_data);
        return *this;
    }

    /**
     * Stop playing a sound
     */
    Sound& Stop() {
        ::StopSound(m_data);
        return *this;
    }

    /**
     * Pause a sound
     */
    Sound& Pause() {
        ::PauseSound(m_data);
        return *this;
    }

    /**
     * Resume a paused sound
     */
    Sound& Resume() {
        ::ResumeSound(m_data);
        return *this;
    }

    /**
     * Check if a sound is currently playing
     */
    [[nodiscard]] bool IsPlaying() const {
        return ::IsSoundPlaying(m_data);
    }

    /**
     * Set volume for a sound (1.0 is max level)
     */
    Sound& SetVolume(float volume) {
        ::SetSoundVolume(m_data, volume);
        return *this;
    }

    /**
     * Set pitch for a sound (1.0 is base level)
     */
    Sound& SetPitch(float pitch) {
        ::SetSoundPitch(m_data, pitch);
        return *this;
    }

    /**
     * Set pan for a sound (0.5 is center)
     */
    Sound& SetPan(float pan = 0.5F) {
        ::SetSoundPan(m_data, pan);
        return *this;
    }

    /**
     * Load a sound from the given file.
     *
     * @throws raylib::RaylibException Throws if the Sound failed to load.
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        set(::LoadSound(fileName.c_str()));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Sound from file"));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Loads the given Wave object into the Sound.
     *
     * @throws raylib::RaylibException Throws if the Sound failed to load.
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) LoadFromWave(const ::Wave& wave) RAYLIB_CPP_THROWS {
        set(::LoadSoundFromWave(wave));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Wave"));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Retrieve whether or not the Sound buffer is loaded.
     *
     * @return True or false depending on whether the Sound buffer is loaded.
     */
    [[nodiscard]] bool IsReady() const {
        return ::IsSoundReady(m_data);
    }

 protected:
    constexpr void set(const ::Sound& sound) {
        m_data.frameCount = sound.frameCount;
        m_data.stream = sound.stream;
    }

    ::Sound m_data;
};
}  // namespace raylib

using RSound = raylib::Sound;

#endif  // RAYLIB_CPP_INCLUDE_SOUND_HPP_
