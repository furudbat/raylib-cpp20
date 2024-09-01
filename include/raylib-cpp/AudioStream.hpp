#ifndef RAYLIB_CPP_INCLUDE_AUDIOSTREAM_HPP_
#define RAYLIB_CPP_INCLUDE_AUDIOSTREAM_HPP_

#include "raylib.hpp"

#include "RaylibError.hpp"
#include "raylib-cpp-utils.hpp"
#ifdef __cpp_exceptions
#include "RaylibException.hpp"
#endif

namespace raylib {

/**
 * AudioStream management functions
 */
class AudioStream {
 public:
    static constexpr uint32_t LoadDefaultChannels = 2;
    static constexpr float SetDefaultVolume = 1.0F;
    static constexpr float SetDefaultPan = 0.5F;

    explicit constexpr AudioStream() = default;
    explicit constexpr AudioStream(const ::AudioStream& music) = delete;
    explicit constexpr AudioStream(::AudioStream&& music) noexcept {
        set(music);

        music.buffer = nullptr;
        music.processor = nullptr;
        music.sampleRate = 0;
        music.sampleSize = 0;
        music.channels = 0;
    }

    [[deprecated("Use AudioStream(music)")]]
    explicit constexpr AudioStream(owner<rAudioBuffer*> buffer,
            owner<rAudioProcessor*> processor = nullptr,
            unsigned int sampleRate = 0,
            unsigned int sampleSize = 0,
            unsigned int channels = 0) : m_data{buffer, processor, sampleRate, sampleSize, channels} {
        // Nothing.
    }

    /**
     * Init audio stream (to stream raw audio pcm data)
     *
     * @throws raylib::RaylibException Throws if the AudioStream failed to load.
     */
    AudioStream(unsigned int sampleRate, unsigned int sampleSize, unsigned int channels = LoadDefaultChannels) RAYLIB_CPP_THROWS {
        Load(sampleRate, sampleSize, channels);
    }

    constexpr AudioStream(const AudioStream&) = delete;
    constexpr AudioStream(AudioStream&& other) noexcept {
        set(other.m_data);

        other.m_data.buffer = nullptr;
        other.m_data.processor = nullptr;
        other.m_data.sampleRate = 0;
        other.m_data.sampleSize = 0;
        other.m_data.channels = 0;
    }
    ~AudioStream() {
        Unload();
    }

    explicit operator ::AudioStream() const noexcept {
        return m_data;
    }
    [[nodiscard]] ::AudioStream c_raylib() const & noexcept {
        return m_data;
    }

    GETTER(rAudioBuffer *, Buffer, m_data.buffer)
    GETTER(rAudioProcessor *, Processor, m_data.processor)
    GETTER(unsigned int, SampleRate, m_data.sampleRate)
    GETTER(unsigned int, SampleSize, m_data.sampleSize)
    GETTER(unsigned int, Channels, m_data.channels)

    constexpr AudioStream& operator=(const ::AudioStream& stream) = delete;
    constexpr AudioStream& operator=(::AudioStream&& stream) noexcept {
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
        set(other.m_data);

        other.m_data.buffer = nullptr;
        other.m_data.processor = nullptr;
        other.m_data.sampleRate = 0;
        other.m_data.sampleSize = 0;
        other.m_data.channels = 0;

        return *this;
    }

    /**
     * Update audio stream buffers with data
     */
    template<typename T>
    /// @TODO: require data must be integral (?), short, unsigned char, ... ???
    AudioStream& Update(const T *data, int samplesCount) {
        ::UpdateAudioStream(m_data, data, samplesCount);
        return *this;
    }

    /**
     * Unload audio stream and free memory
     */
    void Unload() {
        if (IsReady()) {
            ::UnloadAudioStream(m_data);
            m_data.buffer = nullptr;
        }
    }

    /**
     * Check if any audio stream buffers requires refill
     */
    [[nodiscard]] bool IsProcessed() const {
        return ::IsAudioStreamProcessed(m_data);
    }

    /**
     * Play audio stream
     */
    AudioStream& Play() {
        ::PlayAudioStream(m_data);

        return *this;
    }

    /**
     * Pause audio stream
     */
    AudioStream& Pause() {
        ::PauseAudioStream(m_data);

        return *this;
    }

    /**
     * Resume audio stream
     */
    AudioStream& Resume() {
        ::ResumeAudioStream(m_data);

        return *this;
    }

    /**
     * Check if audio stream is playing
     */
    [[nodiscard]] bool IsPlaying() const {
        return ::IsAudioStreamPlaying(m_data);
    }

    /**
     * Stop audio stream
     */
    AudioStream& Stop() {
        ::StopAudioStream(m_data);

        return *this;
    }

    /**
     * Set volume for audio stream (1.0 is max level)
     */
    AudioStream& SetVolume(float volume = SetDefaultVolume) {
        ::SetAudioStreamVolume(m_data, volume);

        return *this;
    }

    /**
     * Set pitch for audio stream (1.0 is base level)
     */
    AudioStream& SetPitch(float pitch) {
        ::SetAudioStreamPitch(m_data, pitch);

        return *this;
    }

    /**
     * Set pan for audio stream (0.5 is centered)
     */
    AudioStream& SetPan(float pan = SetDefaultPan) {
        ::SetAudioStreamPan(m_data, pan);

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
        ::SetAudioStreamCallback(m_data, callback);
    }

    /**
     * Attach audio stream processor to stream
     */
    void AttachProcessor(::AudioCallback processor) {
        ::AttachAudioStreamProcessor(m_data, processor);
    }

    /**
     * Detach audio stream processor from stream
     */
    void DetachProcessor(::AudioCallback processor) {
        ::DetachAudioStreamProcessor(m_data, processor);
    }

    /**
     * Retrieve whether or not the audio stream is ready.
     */
    [[nodiscard]] bool IsReady() const {
        return ::IsAudioStreamReady(m_data);
    }

    /**
     * Load audio stream (to stream raw audio pcm data)
     *
     * @throws raylib::RaylibException Throws if the AudioStream failed to load.
     */
    RAYLIB_CPP_EXPECTED_RESULT_VOID Load(unsigned int sampleRate, unsigned int sampleSize, unsigned int channels = LoadDefaultChannels) {
        Unload();
        set(::LoadAudioStream(sampleRate, sampleSize, channels));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load audio stream"));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

 protected:
    constexpr void set(const ::AudioStream& stream) noexcept {
        m_data.buffer = stream.buffer;
        m_data.processor = stream.processor;
        m_data.sampleRate = stream.sampleRate;
        m_data.sampleSize = stream.sampleSize;
        m_data.channels = stream.channels;
    }

    ::AudioStream m_data {
        .buffer = nullptr,
        .processor = nullptr,
        .sampleRate = 0,
        .sampleSize = 0,
        .channels = 0,
    };
};
}  // namespace raylib

using RAudioStream = raylib::AudioStream;

#endif  // RAYLIB_CPP_INCLUDE_AUDIOSTREAM_HPP_
