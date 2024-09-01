#ifndef RAYLIB_CPP_INCLUDE_MUSIC_HPP_
#define RAYLIB_CPP_INCLUDE_MUSIC_HPP_

#include "RaylibError.hpp"
#include "raylib-cpp-utils.hpp"
#include "raylib.hpp"
#ifdef __cpp_exceptions
#include "RaylibException.hpp"
#endif

#include <chrono>
#include <filesystem>
#include <string>

namespace raylib {
/**
 * Music stream type (audio file streaming from memory)
 */
class Music {
public:
    static constexpr float DefaultSetPan = 0.5F; ///< center

    explicit Music() = default;

    //[[deprecated("Use Music(music)")]]
    explicit Music(
        ::AudioStream stream,
        unsigned int frameCount = 0,
        bool looping = false,
        int ctxType = 0,
        owner<void*> ctxData = nullptr)
        : m_data{stream, frameCount, looping, ctxType, ctxData} {}

    explicit constexpr Music(const ::Music& music) = delete;
    explicit constexpr Music(
        ::Music&& music = {
            .stream = {nullptr, nullptr, 0, 0, 0},
            .frameCount = 0,
            .looping = false,
            .ctxType = 0,
            .ctxData = nullptr}) noexcept {
        set(music);

        music.stream = {nullptr, nullptr, 0, 0, 0};
        music.frameCount = 0;
        music.looping = false;
        music.ctxType = 0;
        music.ctxData = nullptr;
    }

    /**
     * Load music stream from file
     *
     * @throws raylib::RaylibException Throws if the music failed to load.
     */
    Music(czstring fileName) RAYLIB_CPP_THROWS { Load(fileName); }
    Music(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS { Load(fileName); }

    /**
     * Load music stream from memory
     *
     * @throws raylib::RaylibException Throws if the music failed to load.
     */
    Music(const std::string& fileType, std::span<unsigned char> data) RAYLIB_CPP_THROWS {
        LoadFromMemory(fileType, data);
    }
    Music(czstring fileType, std::span<unsigned char> data) RAYLIB_CPP_THROWS { LoadFromMemory(fileType, data); }

    constexpr Music(const Music&) = delete;
    constexpr Music(Music&& other) noexcept {
        set(other.m_data);

        other.m_data.stream = {nullptr, nullptr, 0, 0, 0};
        other.m_data.frameCount = 0;
        other.m_data.looping = false;
        other.m_data.ctxType = 0;
        other.m_data.ctxData = nullptr;
    }

    /**
     * Unload music stream
     */
    ~Music() { Unload(); }

    constexpr Music& operator=(const ::Music& music) = delete;
    constexpr Music& operator=(::Music&& music) noexcept {
        set(music);

        music.ctxType = 0;
        music.ctxData = nullptr;
        music.looping = false;
        music.frameCount = 0;
        music.stream = {nullptr, nullptr, 0, 0, 0};

        return *this;
    }

    constexpr Music& operator=(const Music&) = delete;
    Music& operator=(Music&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other.m_data);

        other.m_data.ctxType = 0;
        other.m_data.ctxData = nullptr;
        other.m_data.looping = false;
        other.m_data.frameCount = 0;
        other.m_data.stream = {nullptr, nullptr, 0, 0, 0};

        return *this;
    }

    explicit operator ::Music() const { return m_data; }
    [[nodiscard]] ::Music c_raylib() const& { return m_data; }

    GETTER(::AudioStream, Stream, m_data.stream)
    GETTER(unsigned int, FrameCount, m_data.frameCount)
    GETTERSETTER(bool, Looping, m_data.looping)
    GETTER(int, CtxType, m_data.ctxType)
    CONST_GETTER(void*, CtxData, m_data.ctxData)

    /**
     * Unload music stream
     */
    void Unload() { ::UnloadMusicStream(m_data); }

    /**
     * Start music playing
     */
    Music& Play() {
        ::PlayMusicStream(m_data);
        return *this;
    }

    /**
     * Updates buffers for music streaming
     */
    Music& Update() {
        ::UpdateMusicStream(m_data);
        return *this;
    }

    /**
     * Stop music playing
     */
    Music& Stop() {
        ::StopMusicStream(m_data);
        return *this;
    }

    /**
     * Pause music playing
     */
    Music& Pause() {
        ::PauseMusicStream(m_data);
        return *this;
    }

    /**
     * Resume music playing
     */
    Music& Resume() {
        ::ResumeMusicStream(m_data);
        return *this;
    }

    /**
     * Seek music to a position (in seconds)
     */
    Music& Seek(float position) {
        ::SeekMusicStream(m_data, position);
        return *this;
    }
    Music& Seek(std::chrono::milliseconds position) {
        ::SeekMusicStream(m_data, static_cast<float>(position.count()) / 1000.0f);
        return *this;
    }

    /**
     * Check if music is playing
     */
    [[nodiscard]] bool IsPlaying() const { return ::IsMusicStreamPlaying(m_data); }

    /**
     * Set volume for music
     */
    Music& SetVolume(float volume) {
        ::SetMusicVolume(m_data, volume);
        return *this;
    }

    /**
     * Set pitch for music
     */
    Music& SetPitch(float pitch) {
        ::SetMusicPitch(m_data, pitch);
        return *this;
    }

    /**
     * Set pan for a music (0.5 is center)
     */
    Music& SetPan(float pan = DefaultSetPan) {
        ::SetMusicPan(m_data, pan);
        return *this;
    }

    /**
     * Get music time length (in seconds)
     */
    [[nodiscard]] float GetTimeLength() const { return ::GetMusicTimeLength(m_data); }
    [[nodiscard]] std::chrono::milliseconds GetTimeLengthMs() const {
        return std::chrono::milliseconds{static_cast<int64_t>(::GetMusicTimeLength(m_data) * 1000)};
    }

    /**
     * Get current music time played (in seconds)
     */
    [[nodiscard]] float GetTimePlayed() const { return ::GetMusicTimePlayed(m_data); }
    [[nodiscard]] std::chrono::milliseconds GetTimePlayedMs() const {
        return std::chrono::milliseconds{static_cast<int64_t>(::GetMusicTimePlayed(m_data) * 1000)};
    }

    /**
     * Load music stream from file
     *
     * @throws raylib::RaylibException Throws if the music failed to load.
     */
    RAYLIB_CPP_EXPECTED_RESULT_VOID Load(czstring fileName) {
        set(::LoadMusicStream(fileName));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(
                RaylibError(::TextFormat("Failed to load Music from file: %s", fileName)));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }
    RAYLIB_CPP_EXPECTED_RESULT_VOID Load(const std::filesystem::path& fileName) {
        RAYLIB_CPP_RETURN_EXPECTED_VOID_VALUE(Load(fileName.c_str()));
    }

    /**
     * Load music stream from memory
     *
     * @throws raylib::RaylibException Throws if the music failed to load.
     */
    RAYLIB_CPP_EXPECTED_RESULT_VOID LoadFromMemory(czstring fileType, std::span<unsigned char> data) {
        set(::LoadMusicStreamFromMemory(fileType, data.data(), static_cast<int>(data.size())));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(
                RaylibError(::TextFormat("Failed to load Music from %s file dat", fileType)));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }
    RAYLIB_CPP_EXPECTED_RESULT_VOID LoadFromMemory(const std::string& fileType, std::span<unsigned char> data) {
        RAYLIB_CPP_RETURN_EXPECTED_VOID_VALUE(LoadFromMemory(fileType.c_str(), data));
    }

    /**
     * Retrieve whether or not the Music has been loaded.
     *
     * @return True or false depending on whether the Music has been loaded.
     */
    [[nodiscard]] bool IsReady() const { return ::IsMusicReady(m_data); }
protected:
    constexpr void set(const ::Music& music) noexcept {
        m_data.stream = music.stream;
        m_data.frameCount = music.frameCount;
        m_data.looping = music.looping;
        m_data.ctxType = music.ctxType;
        m_data.ctxData = music.ctxData;
    }

    ::Music m_data{
        .stream =
            {
                .buffer = nullptr,
                .processor = nullptr,
                .sampleRate = 0,
                .sampleSize = 0,
                .channels = 0,
            },
        .frameCount = 0,
        .looping = false,
        .ctxType = 0,
        .ctxData = nullptr};
};
} // namespace raylib

using RMusic = raylib::Music;

#endif // RAYLIB_CPP_INCLUDE_MUSIC_HPP_
