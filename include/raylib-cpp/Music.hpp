#ifndef RAYLIB_CPP_INCLUDE_MUSIC_HPP_
#define RAYLIB_CPP_INCLUDE_MUSIC_HPP_

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
 * Music stream type (audio file streaming from memory)
 */
class Music : public ::Music {
 public:
    //[[deprecated("Use Music(music)")]]
    explicit Music(::AudioStream _stream,
            unsigned int _frameCount = 0,
            bool _looping = false,
            int _ctxType = 0,
            void *_ctxData = nullptr) : ::Music{_stream, _frameCount, _looping, _ctxType, _ctxData} {}

    explicit constexpr Music(const ::Music& music = {
            .stream = {nullptr, nullptr, 0, 0, 0},
            .frameCount = 0,
            .looping = false,
            .ctxType = 0,
            .ctxData = nullptr
    }) {
        set(music);
    }

    /**
     * Load music stream from file
     *
     * @throws raylib::RaylibException Throws if the music failed to load.
     */
    Music(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        Load(fileName);
    }

    /**
     * Load music stream from memory
     *
     * @throws raylib::RaylibException Throws if the music failed to load.
     */
    Music(const std::string& fileType, std::span<unsigned char> data) RAYLIB_CPP_THROWS {
        Load(fileType, data);
    }

    constexpr Music(const Music&) = delete;

    constexpr Music(Music&& other) {
        set(other);

        other.stream = {};
        other.frameCount = 0;
        other.looping = false;
        other.ctxType = 0;
        other.ctxData = nullptr;
    }

    /**
     * Unload music stream
     */
    ~Music() {
        Unload();
    }

    GETTER(::AudioStream, Stream, stream)
    GETTER(unsigned int, FrameCount, frameCount)
    GETTERSETTER(bool, Looping, looping)
    GETTER(int, CtxType, ctxType)
    GETTER(void*, CtxData, ctxData)

    constexpr Music& operator=(const ::Music& music) {
        set(music);
        return *this;
    }

    constexpr Music& operator=(const Music&) = delete;
    Music& operator=(Music&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other);

        other.ctxType = 0;
        other.ctxData = nullptr;
        other.looping = false;
        other.frameCount = 0;
        other.stream = {};

        return *this;
    }

    /**
     * Unload music stream
     */
    void Unload() {
        ::UnloadMusicStream(*this);
    }

    /**
     * Start music playing
     */
    Music& Play() {
        ::PlayMusicStream(*this);
        return *this;
    }

    /**
     * Updates buffers for music streaming
     */
    Music& Update() {
        ::UpdateMusicStream(*this);
        return *this;
    }

    /**
     * Stop music playing
     */
    Music& Stop() {
        ::StopMusicStream(*this);
        return *this;
    }

    /**
     * Pause music playing
     */
    Music& Pause() {
        ::PauseMusicStream(*this);
        return *this;
    }

    /**
     * Resume music playing
     */
    Music& Resume() {
        ::ResumeMusicStream(*this);
        return *this;
    }

    /**
     * Seek music to a position (in seconds)
     */
    Music& Seek(float position) {
        SeekMusicStream(*this, position);
        return *this;
    }

    /**
     * Check if music is playing
     */
    [[nodiscard]] bool IsPlaying() const {
        return ::IsMusicStreamPlaying(*this);
    }

    /**
     * Set volume for music
     */
    Music& SetVolume(float volume) {
        ::SetMusicVolume(*this, volume);
        return *this;
    }

    /**
     * Set pitch for music
     */
    Music& SetPitch(float pitch) {
        ::SetMusicPitch(*this, pitch);
        return *this;
    }

    /**
     * Set pan for a music (0.5 is center)
     */
    Music& SetPan(float pan = 0.5F) {
        ::SetMusicPan(*this, pan);
        return *this;
    }

    /**
     * Get music time length (in seconds)
     */
    [[nodiscard]] float GetTimeLength() const {
        return ::GetMusicTimeLength(*this);
    }

    /**
     * Get current music time played (in seconds)
     */
    [[nodiscard]] float GetTimePlayed() const {
        return ::GetMusicTimePlayed(*this);
    }

    /**
     * Load music stream from file
     *
     * @throws raylib::RaylibException Throws if the music failed to load.
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const std::filesystem::path& fileName) {
        set(::LoadMusicStream(fileName.c_str()));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Music from file: " + fileName.string()));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Load music stream from memory
     *
     * @throws raylib::RaylibException Throws if the music failed to load.
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const std::string& fileType, std::span<unsigned char> data) {
        set(::LoadMusicStreamFromMemory(fileType.c_str(), data.data(), static_cast<int>(data.size())));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError(TextFormat("Failed to load Music from %s file dat", fileType.c_str())));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Retrieve whether or not the Music has been loaded.
     *
     * @return True or false depending on whether the Music has been loaded.
     */
    [[nodiscard]] bool IsReady() const {
        return ::IsMusicReady(*this);
    }

 protected:
    constexpr void set(const ::Music& music) {
        stream = music.stream;
        frameCount = music.frameCount;
        looping = music.looping;
        ctxType = music.ctxType;
        ctxData = music.ctxData;
    }
};
}  // namespace raylib

using RMusic = raylib::Music;

#endif  // RAYLIB_CPP_INCLUDE_MUSIC_HPP_
