#ifndef RAYLIB_CPP_INCLUDE_AUDIOSTREAM_HPP_
#define RAYLIB_CPP_INCLUDE_AUDIOSTREAM_HPP_

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#ifdef __cpp_exceptions
#include "./RaylibException.hpp"
#endif

namespace raylib {
/**
 * AudioStream management functions
 */
class AudioStream : public ::AudioStream {
 public:
    AudioStream(const ::AudioStream& music) {
        set(music);
    }

    explicit AudioStream(rAudioBuffer* pBuffer = nullptr,
            rAudioProcessor *pProcessor = nullptr,
            unsigned int pSampleRate = 0,
            unsigned int pSampleSize = 0,
            unsigned int pChannels = 0) : ::AudioStream{pBuffer, pProcessor, pSampleRate, pSampleSize, pChannels} {
        // Nothing.
    }

    /**
     * Init audio stream (to stream raw audio pcm data)
     *
     * @throws raylib::RaylibException Throws if the AudioStream failed to load.
     */
    AudioStream(unsigned int pSampleRate, unsigned int pSampleSize, unsigned int pChannels = 2) {
        Load(pSampleRate, pSampleSize, pChannels);
    }

    AudioStream(const AudioStream&) = delete;
    AudioStream(AudioStream&& other) {
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

    GETTERSETTER(rAudioBuffer *, Buffer, buffer)
    GETTERSETTER(rAudioProcessor *, Processor, processor)
    GETTERSETTER(unsigned int, SampleRate, sampleRate)
    GETTERSETTER(unsigned int, SampleSize, sampleSize)
    GETTERSETTER(unsigned int, Channels, channels)

    AudioStream& operator=(const ::AudioStream& stream) {
        set(stream);
        return *this;
    }

    AudioStream& operator=(const AudioStream&) = delete;

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
    AudioStream& SetVolume(float volume = 1.0f) {
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
    AudioStream& SetPan(float pan = 0.5f) {
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
    void Load(unsigned int SampleRate, unsigned int SampleSize, unsigned int Channels = 2) {
        Unload();
        set(::LoadAudioStream(SampleRate, SampleSize, Channels));
        if (!IsReady()) {
            throw RaylibException("Failed to load audio stream");
        }
    }

 protected:
    void set(const ::AudioStream& stream) {
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
