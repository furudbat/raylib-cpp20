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
    inline static constexpr float DefaultSetPan = 0.5f; ///< center

    constexpr Sound() {
        m_data.stream = { nullptr, nullptr, 0, 0, 0 };
        m_data.frameCount = 0;
    }

    constexpr Sound(owner<::AudioStream> _stream, unsigned int _frameCount) : m_data{_stream, _frameCount} {
        // Nothing.
    }

    constexpr Sound(owner<const ::Sound&>) = delete;
    constexpr Sound(owner<::Sound&&> other) noexcept {
        set(other);

        other.stream = { nullptr, nullptr, 0, 0, 0 };
        other.frameCount = 0;
    }

    constexpr Sound(const Sound&) = delete;
    constexpr Sound(Sound&& other) noexcept {
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

    ~Sound() noexcept {
        Unload();
    }

    explicit operator ::Sound() const noexcept {
        return m_data;
    }
    [[nodiscard]] ::Sound c_raylib() const & noexcept {
        return m_data;
    }

    GETTER(unsigned int, FrameCount, m_data.frameCount)
    GETTER(::AudioStream, Stream, m_data.stream)

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
    template<typename T>
    /// @TODO: make T integral (?) ... short, unsigned char, ... ???
    Sound& Update(const T *data, int samplesCount) {
        ::UpdateSound(m_data, data, samplesCount);
        return *this;
    }

    /**
     * Update sound buffer with new data, assuming it's the same sample count.
     */
    template<typename T>
    /// @TODO: make T integral (?) ... short, unsigned char, ... ???
    Sound& Update(const T *data) {
        ::UpdateSound(m_data, data, static_cast<int>(m_data.frameCount));
        return *this;
    }

    /**
     * Unload sound
     */
    void Unload() noexcept {
        if (m_data.stream.buffer != nullptr) {
            ::UnloadSound(m_data);
            m_data.stream.buffer = nullptr;
        }
    }

    /**
     * Play a sound
     */
    Sound& Play() noexcept {
        ::PlaySound(m_data);
        return *this;
    }

    /**
     * Stop playing a sound
     */
    Sound& Stop() noexcept {
        ::StopSound(m_data);
        return *this;
    }

    /**
     * Pause a sound
     */
    Sound& Pause() noexcept {
        ::PauseSound(m_data);
        return *this;
    }

    /**
     * Resume a paused sound
     */
    Sound& Resume() noexcept {
        ::ResumeSound(m_data);
        return *this;
    }

    /**
     * Check if a sound is currently playing
     */
    [[nodiscard]] bool IsPlaying() const noexcept {
        return ::IsSoundPlaying(m_data);
    }

    /**
     * Set volume for a sound (1.0 is max level)
     */
    Sound& SetVolume(float volume) noexcept {
        ::SetSoundVolume(m_data, volume);
        return *this;
    }

    /**
     * Set pitch for a sound (1.0 is base level)
     */
    Sound& SetPitch(float pitch) noexcept {
        ::SetSoundPitch(m_data, pitch);
        return *this;
    }

    /**
     * Set pan for a sound (0.5 is center)
     */
    Sound& SetPan(float pan = DefaultSetPan) noexcept {
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
    [[nodiscard]] bool IsReady() const noexcept {
        return ::IsSoundReady(m_data);
    }

 protected:
    constexpr void set(const ::Sound& sound) noexcept {
        m_data.frameCount = sound.frameCount;
        m_data.stream = sound.stream;
    }

    ::Sound m_data;
};
}  // namespace raylib

using RSound = raylib::Sound;

#endif  // RAYLIB_CPP_INCLUDE_SOUND_HPP_
