#ifndef RAYLIB_CPP_INCLUDE_WAVE_HPP_
#define RAYLIB_CPP_INCLUDE_WAVE_HPP_

#include <string>
#include <filesystem>
#include <span>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#ifdef __cpp_exceptions
#include "./RaylibException.hpp"
#endif
#include "./RaylibError.hpp"

namespace raylib {

struct RayWaveSamplesDeleter {
    void operator()(float* arg) const {
        UnloadWaveSamples(arg);
    }
};
using RayWaveSamples = RayArrayHolder<float, RayWaveSamplesDeleter>;

/**
 * Wave type, defines audio wave data
 */
class Wave : public ::Wave {
 public:
    constexpr explicit Wave(const ::Wave& wave) = delete;
    constexpr explicit Wave(::Wave&& wave = {
            .frameCount = 0,
            .sampleRate = 0,
            .sampleSize = 0,
            .channels = 0,
            .data = nullptr,
    }) {
        set(wave);

        wave.frameCount = 0;
        wave.sampleRate = 0;
        wave.sampleSize = 0;
        wave.channels = 0;
        wave.data = nullptr;
    }

    [[deprecated("Use Wave(wave)")]]
    constexpr explicit Wave(
            unsigned int _frameCount,
            unsigned int _sampleRate = 0,
            unsigned int _sampleSize = 0,
            unsigned int _channels = 0,
            owner<void*> _data = nullptr) : ::Wave{_frameCount, _sampleRate, _sampleSize, _channels, _data} {
        // Nothing.
    }

    /**
     * Load wave data from file
     *
     * @throws raylib::RaylibException Throws if the Wave failed to load.
     */
    Wave(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        Load(fileName);
    }

    /**
     * Load wave from memory buffer, fileType refers to extension: i.e. "wav"
     *
     * @throws raylib::RaylibException Throws if the Wave failed to load.
     */
    Wave(const std::string& fileType, std::span<const unsigned char> fileData) RAYLIB_CPP_THROWS {
        Load(fileType, fileData);
    }

    Wave(const Wave& other) {
        auto copy = other.Copy();
        set(copy);

        copy.frameCount = 0;
        copy.sampleRate = 0;
        copy.sampleSize = 0;
        copy.channels = 0;
        copy.data = nullptr;
    }

    Wave(Wave&& other) {
        set(other);

        other.frameCount = 0;
        other.sampleRate = 0;
        other.sampleSize = 0;
        other.channels = 0;
        other.data = nullptr;
    }

    /**
     * Unload wave data
     */
    ~Wave() {
        Unload();
    }

    GETTERSETTER(unsigned int, FrameCount, frameCount)
    GETTERSETTER(unsigned int, SampleRate, sampleRate)
    GETTERSETTER(unsigned int, SampleSize, sampleSize)
    GETTERSETTER(unsigned int, Channels, channels)
    GETTERSETTER(void *, Data, data)

    Wave& operator=(const ::Wave& wave) = delete;
    Wave& operator=(::Wave&& wave) {
        set(wave);

        wave.frameCount = 0;
        wave.sampleRate = 0;
        wave.sampleSize = 0;
        wave.channels = 0;
        wave.data = nullptr;

        return *this;
    }

    Wave& operator=(const Wave& other) {
        if (this == &other) {
            return *this;
        }

        Unload();
        auto copy = other.Copy();
        set(copy);

        copy.frameCount = 0;
        copy.sampleRate = 0;
        copy.sampleSize = 0;
        copy.channels = 0;
        copy.data = nullptr;

        return *this;
    }
    Wave& operator=(Wave&& other) noexcept {
        if (this != &other) {
            return *this;
        }

        Unload();
        set(other);

        other.frameCount = 0;
        other.sampleRate = 0;
        other.sampleSize = 0;
        other.channels = 0;
        other.data = nullptr;

        return *this;
    }

    /**
     * Copy a wave to a new wave
     */
    Wave Copy() const {
        return Wave{::WaveCopy(*this)};
    }

    /**
     * Crop a wave to defined samples range
     */
    Wave& Crop(int initSample, int finalSample) {
        ::WaveCrop(this, initSample, finalSample);
        return *this;
    }

    /**
     * Convert wave data to desired format
     */
    Wave& Format(int SampleRate, int SampleSize, int Channels = 2) {
        ::WaveFormat(this, SampleRate, SampleSize, Channels);
        return *this;
    }

    /**
     * Load samples data from wave as a floats array
     */
    RayWaveSamples LoadSamples() {
        /// @Note: assume allocated size
        return RayWaveSamples{::LoadWaveSamples(*this), frameCount*channels*sizeof(float)};
    }

    /**
     * Unload samples data loaded with LoadWaveSamples()
     */
    static void UnloadSamples(float *samples) {
        ::UnloadWaveSamples(samples);
    }

    /**
     * Export wave data to file, returns true on success
     */
    bool Export(const std::filesystem::path& fileName) {
        // TODO(RobLoach): Throw exception on error.
        return ::ExportWave(*this, fileName.c_str());
    }

    /**
     * Export wave sample data to code (.h), returns true on success
     */
    bool ExportAsCode(const std::filesystem::path& fileName) {
        // TODO(RobLoach): Throw exception on error.
        return ::ExportWaveAsCode(*this, fileName.c_str());
    }

    /**
     * Unload wave data
     */
    void Unload() {
        // Protect against calling UnloadWave() twice.
        if (data != nullptr) {
            ::UnloadWave(*this);
            data = nullptr;
        }
    }

    /**
     * Load sound from wave data
     */
    raylib::Sound LoadSound() {
        return raylib::Sound{::LoadSoundFromWave(*this)};
    }

    /**
     * Load sound from wave data
     */
     /// move ownership ???
    //explicit operator ::Sound() {
    //    return LoadSound();
    //}

    /**
     * Load wave data from file.
     *
     * @throws raylib::RaylibException Throws if the Wave failed to load.
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        set(::LoadWave(fileName.c_str()));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_EXPECTED_OR_THROW(RaylibError("Failed to load Wave from file: " + fileName.string()));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Load wave from memory buffer, fileType refers to extension: i.e. "wav"
     *
     * @throws raylib::RaylibException Throws if the Wave failed to load.
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const std::string& fileType, std::span<const unsigned char> fileData) {
        set(::LoadWaveFromMemory(fileType.c_str(), fileData.data(), static_cast<int>(fileData.size())));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_EXPECTED_OR_THROW(RaylibError("Failed to load Wave from file data of type: " + fileType));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Retrieve whether or not the Wave data has been loaded.
     *
     * @return True or false depending on whether the wave data has been loaded.
     */
    [[nodiscard]] bool IsReady() const {
        return ::IsWaveReady(*this);
    }

 protected:
    constexpr void set(const ::Wave& wave) noexcept {
        frameCount = wave.frameCount;
        sampleRate = wave.sampleRate;
        sampleSize = wave.sampleSize;
        channels = wave.channels;
        data = wave.data;
    }
};

}  // namespace raylib

using RWave = raylib::Wave;

#endif  // RAYLIB_CPP_INCLUDE_WAVE_HPP_
