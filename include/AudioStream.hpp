#ifndef RAYLIB_CPP_INCLUDE_AUDIOSTREAM_HPP_
#define RAYLIB_CPP_INCLUDE_AUDIOSTREAM_HPP_

#include <cstdint>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#ifdef __cpp_exceptions
#include "./RaylibException.hpp"
#endif
#include "./RaylibError.hpp"

namespace raylib {

/**
 * AudioStream management functions
 */
class AudioStream : public ::AudioStream {
 public:
    inline static constexpr uint32_t LoadDefaultChannels = 2;
    inline static constexpr float SetDefaultVolume = 1.0F;
    inline static constexpr float SetDefaultPan = 0.5F;

    explicit constexpr AudioStream(const ::AudioStream& music) = delete;
    explicit constexpr AudioStream(::AudioStream&& music = {
        .buffer = nullptr,
        .processor = nullptr,
        .sampleRate = 0,
        .sampleSize = 0,
        .channels = 0,
    }) {
        set(music);

        music.buffer = nullptr;
        music.processor = nullptr;
        music.sampleRate = 0;
        music.sampleSize = 0;
        music.channels = 0;
    }

    [[deprecated("Use AudioStream(music)")]] explicit constexpr AudioStream(owner<rAudioBuffer*> _buffer,
            owner<rAudioProcessor*> _processor = nullptr,
            unsigned int _sampleRate = 0,
            unsigned int _sampleSize = 0,
            unsigned int _channels = 0) : ::AudioStream{_buffer, _processor, _sampleRate, _sampleSize, _channels} {
        // Nothing.
    }

    /**
     * Init audio stream (to stream raw audio pcm data)
     *
     * @throws raylib::RaylibException Throws if the AudioStream failed to load.
     */
    AudioStream(unsigned int _sampleRate, unsigned int _sampleSize, unsigned int _channels = LoadDefaultChannels) RAYLIB_CPP_THROWS {
        Load(_sampleRate, _sampleSize, _channels);
    }

    constexpr AudioStream(const AudioStream&) = delete;
    constexpr AudioStream(AudioStream&& other) {
        set(other);

        other.buffer = nullptr;
        other.processor = nullptr;
        other.sampleRate = 0;
        other.sampleSize = 0;
        other.channels = 0;
    }
    ~AudioStream() {
        Unload();
    }

    GETTER(rAudioBuffer *, Buffer, buffer)
    GETTER(rAudioProcessor *, Processor, processor)
    GETTER(unsigned int, SampleRate, sampleRate)
    GETTER(unsigned int, SampleSize, sampleSize)
    GETTER(unsigned int, Channels, channels)

    constexpr AudioStream& operator=(const ::AudioStream& stream) = delete;
    constexpr AudioStream& operator=(::AudioStream&& stream) {
        set(stream);

        stream.buffer = nullptr;
        stream.processor = nullptr;
        stream.sampleRate = 0;
        stream.sampleSize = 0;
        stream.channels = 0;

        return *this;
    }
    constexpr AudioStream& operator=(const AudioStream&) = delete;
    AudioStream& operator=(AudioStream&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other);

        other.buffer = nullptr;
        other.processor = nullptr;
        other.sampleRate = 0;
        other.sampleSize = 0;
        other.channels = 0;

        return *this;
    }

    /**
     * Update audio stream buffers with data
     */
    AudioStream& Update(const void *data, int samplesCount) {
        ::UpdateAudioStream(*this, data, samplesCount);
        return *this;
    }

    /**
     * Unload audio stream and free memory
     */
    void Unload() {
        if (IsReady()) {
            ::UnloadAudioStream(*this);
        }
    }

    /**
     * Check if any audio stream buffers requires refill
     */
    [[nodiscard]] bool IsProcessed() const {
        return ::IsAudioStreamProcessed(*this);
    }

    /**
     * Play audio stream
     */
    AudioStream& Play() {
        ::PlayAudioStream(*this);
        return *this;
    }

    /**
     * Pause audio stream
     */
    AudioStream& Pause() {
        ::PauseAudioStream(*this);
        return *this;
    }

    /**
     * Resume audio stream
     */
    AudioStream& Resume() {
        ::ResumeAudioStream(*this);
        return *this;
    }

    /**
     * Check if audio stream is playing
     */
    [[nodiscard]] bool IsPlaying() const {
        return ::IsAudioStreamPlaying(*this);
    }

    /**
     * Stop audio stream
     */
    AudioStream& Stop() {
        ::StopAudioStream(*this);
        return *this;
    }

    /**
     * Set volume for audio stream (1.0 is max level)
     */
    AudioStream& SetVolume(float volume = SetDefaultVolume) {
        ::SetAudioStreamVolume(*this, volume);
        return *this;
    }

    /**
     * Set pitch for audio stream (1.0 is base level)
     */
    AudioStream& SetPitch(float pitch) {
        ::SetAudioStreamPitch(*this, pitch);
        return *this;
    }

    /**
     * Set pan for audio stream (0.5 is centered)
     */
    AudioStream& SetPan(float pan = SetDefaultPan) {
        ::SetAudioStreamPan(*this, pan);
        return *this;
    }

    /**
     * Default size for new audio streams
     */
    static void SetBufferSizeDefault(int size) {
        ::SetAudioStreamBufferSizeDefault(size);
    }

    /**
     * Audio thread callback to request new data
     */
    void SetCallback(::AudioCallback callback) {
        ::SetAudioStreamCallback(*this, callback);
    }

    /**
     * Attach audio stream processor to stream
     */
    void AttachProcessor(::AudioCallback pProcessor) {
        ::AttachAudioStreamProcessor(*this, pProcessor);
    }

    /**
     * Detach audio stream processor from stream
     */
    void DetachProcessor(::AudioCallback pProcessor) {
        ::DetachAudioStreamProcessor(*this, pProcessor);
    }

    /**
     * Retrieve whether or not the audio stream is ready.
     */
    [[nodiscard]] bool IsReady() const {
        return ::IsAudioStreamReady(*this);
    }

    /**
     * Load audio stream (to stream raw audio pcm data)
     *
     * @throws raylib::RaylibException Throws if the AudioStream failed to load.
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(unsigned int SampleRate, unsigned int SampleSize, unsigned int Channels = LoadDefaultChannels) {
        Unload();
        set(::LoadAudioStream(SampleRate, SampleSize, Channels));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load audio stream"));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

 protected:
    constexpr void set(const ::AudioStream& stream) noexcept {
        buffer = stream.buffer;
        processor = stream.processor;
        sampleRate = stream.sampleRate;
        sampleSize = stream.sampleSize;
        channels = stream.channels;
    }
};
}  // namespace raylib

using RAudioStream = raylib::AudioStream;

#endif  // RAYLIB_CPP_INCLUDE_AUDIOSTREAM_HPP_
