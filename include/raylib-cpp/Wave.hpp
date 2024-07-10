#ifndef RAYLIB_CPP_INCLUDE_WAVE_HPP_
#define RAYLIB_CPP_INCLUDE_WAVE_HPP_

#include "raylib.hpp"
#include "raylib-cpp-utils.hpp"
#ifdef __cpp_exceptions
#include "RaylibException.hpp"
#endif
#include "RaylibError.hpp"

#include <cstddef>
#include <string>
#include <filesystem>
#include <span>

namespace raylib {

struct RayWaveSamplesDeleter {
    void operator()(float* arg) const {
        if (arg != nullptr) {
            UnloadWaveSamples(arg);
        }
    }
};
using RayWaveSamples = RayArrayHolder<float, RayWaveSamplesDeleter>;

/**
 * Wave type, defines audio wave data
 */
class Wave {
 public:
    inline static constexpr int DefaultFormatChannels = 2;

    constexpr explicit Wave(const ::Wave& wave) = delete;
    constexpr explicit Wave(::Wave&& wave = {
            .frameCount = 0,
            .sampleRate = 0,
            .sampleSize = 0,
            .channels = 0,
            .data = nullptr,
    }) noexcept {
        set(wave);

        wave.frameCount = 0;
        wave.sampleRate = 0;
        wave.sampleSize = 0;
        wave.channels = 0;
        wave.data = nullptr;
    }

    [[deprecated("Use Wave(wave)")]]
    constexpr explicit Wave(
            unsigned int frameCount,
            unsigned int sampleRate = 0,
            unsigned int sampleSize = 0,
            unsigned int channels = 0,
            owner<void*> data = nullptr) : m_data{frameCount, sampleRate, sampleSize, channels, data} {
        // Nothing.
    }

    /**
     * Load wave data from file
     *
     * @throws raylib::RaylibException Throws if the Wave failed to load.
     */
    Wave(czstring fileName) RAYLIB_CPP_THROWS {
        Load(fileName);
    }
    Wave(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        Load(fileName);
    }

    /**
     * Load wave from memory buffer, fileType refers to extension: i.e. "wav"
     *
     * @throws raylib::RaylibException Throws if the Wave failed to load.
     */
    Wave(czstring fileType, std::span<const unsigned char> fileData) RAYLIB_CPP_THROWS {
        Load(fileType, fileData);
    }
    Wave(const std::string& fileType, std::span<const unsigned char> fileData) RAYLIB_CPP_THROWS {
        Load(fileType, fileData);
    }
    Wave(const std::string& fileType, std::span<const std::byte> fileData) RAYLIB_CPP_THROWS {
        Load(fileType, std::span{std::bit_cast<const unsigned char*>(fileData.data()), fileData.size()});
    }

    Wave(const Wave& other) {
        auto copy = other.Copy();
        set(copy.m_data);

        copy.m_data.frameCount = 0;
        copy.m_data.sampleRate = 0;
        copy.m_data.sampleSize = 0;
        copy.m_data.channels = 0;
        copy.m_data.data = nullptr;
    }
    Wave(Wave&& other) noexcept {
        set(other.m_data);

        other.m_data.frameCount = 0;
        other.m_data.sampleRate = 0;
        other.m_data.sampleSize = 0;
        other.m_data.channels = 0;
        other.m_data.data = nullptr;
    }

    /**
     * Unload wave data
     */
    ~Wave() noexcept {
        Unload();
    }

    Wave& operator=(const ::Wave& wave) = delete;
    Wave& operator=(::Wave&& wave) noexcept {
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
        set(copy.m_data);

        copy.m_data.frameCount = 0;
        copy.m_data.sampleRate = 0;
        copy.m_data.sampleSize = 0;
        copy.m_data.channels = 0;
        copy.m_data.data = nullptr;

        return *this;
    }
    Wave& operator=(Wave&& other) noexcept {
        if (this != &other) {
            return *this;
        }

        Unload();
        set(other.m_data);

        other.m_data.frameCount = 0;
        other.m_data.sampleRate = 0;
        other.m_data.sampleSize = 0;
        other.m_data.channels = 0;
        other.m_data.data = nullptr;

        return *this;
    }

    explicit operator ::Wave() const noexcept {
        return m_data;
    }
    [[nodiscard]] ::Wave c_raylib() const & noexcept {
        return m_data;
    }

    GETTER(unsigned int, FrameCount, m_data.frameCount)
    GETTER(unsigned int, SampleRate, m_data.sampleRate)
    GETTER(unsigned int, SampleSize, m_data.sampleSize)
    GETTER(unsigned int, Channels, m_data.channels)
    CONST_GETTER(void *, Data, m_data.data)

    /**
     * Copy a wave to a new wave
     */
    [[nodiscard]] Wave Copy() const {
        return Wave{::WaveCopy(m_data)};
    }

    /**
     * Crop a wave to defined samples range
     */
    Wave& Crop(int initSample, int finalSample) {
        ::WaveCrop(&m_data, initSample, finalSample);
        return *this;
    }

    /**
     * Convert wave data to desired format
     */
    Wave& Format(int SampleRate, int SampleSize, int Channels = DefaultFormatChannels) {
        ::WaveFormat(&m_data, SampleRate, SampleSize, Channels);
        return *this;
    }

    /**
     * Load samples data from wave as a floats array
     */
    RayWaveSamples LoadSamples() {
        /// @Note: assume allocated size
        return RayWaveSamples{::LoadWaveSamples(m_data), static_cast<unsigned long>(m_data.frameCount*m_data.channels)*sizeof(float)};
    }

    /**
     * Unload samples data loaded with LoadWaveSamples()
     */
    static void UnloadSamples(float *samples) noexcept {
        ::UnloadWaveSamples(samples);
    }

    /**
     * Export wave data to file, returns true on success
     */
    bool Export(czstring fileName) {
        // TODO(RobLoach): Throw exception on error.
        return ::ExportWave(m_data, fileName);
    }
    bool Export(const std::filesystem::path& fileName) {
        // TODO(RobLoach): Throw exception on error.
        return ::ExportWave(m_data, fileName.c_str());
    }

    /**
     * Export wave sample data to code (.h), returns true on success
     */
    bool ExportAsCode(czstring fileName) {
        // TODO(RobLoach): Throw exception on error.
        return ::ExportWaveAsCode(m_data, fileName);
    }
    bool ExportAsCode(const std::filesystem::path& fileName) {
        // TODO(RobLoach): Throw exception on error.
        return ::ExportWaveAsCode(m_data, fileName.c_str());
    }

    /**
     * Unload wave data
     */
    void Unload() noexcept {
        // Protect against calling UnloadWave() twice.
        if (m_data.data != nullptr) {
            ::UnloadWave(m_data);
            m_data.data = nullptr;
        }
    }

    /**
     * Load sound from wave data
     */
    raylib::Sound LoadSound() const {
        return raylib::Sound{::LoadSoundFromWave(m_data)};
    }

    /**
     * Load sound from wave data
     */
    /*
    explicit operator raylib::Sound() const {
        return LoadSound();
    }
    */

    /**
     * Load wave data from file.
     *
     * @throws raylib::RaylibException Throws if the Wave failed to load.
     */
    RAYLIB_CPP_EXPECTED_RESULT_VOID Load(czstring fileName) RAYLIB_CPP_THROWS {
        set(::LoadWave(fileName));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError(::TextFormat("Failed to load Wave from file: %s", fileName)));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }
    RAYLIB_CPP_EXPECTED_RESULT_VOID Load(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        RAYLIB_CPP_RETURN_EXPECTED_VOID_VALUE(Load(fileName.c_str()));
    }

    /**
     * Load wave from memory buffer, fileType refers to extension: i.e. "wav"
     *
     * @throws raylib::RaylibException Throws if the Wave failed to load.
     */
    RAYLIB_CPP_EXPECTED_RESULT_VOID Load(czstring fileType, std::span<const unsigned char> fileData) {
        assert(std::cmp_less_equal(fileData.size(), std::numeric_limits<int>::max()));
        set(::LoadWaveFromMemory(fileType, fileData.data(), static_cast<int>(fileData.size())));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError(::TextFormat("Failed to load Wave from file data of type: %s", fileType)));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }
    RAYLIB_CPP_EXPECTED_RESULT_VOID Load(const std::string& fileType, std::span<const unsigned char> fileData) {
        RAYLIB_CPP_RETURN_EXPECTED_VOID_VALUE(Load(fileType.c_str(), fileData));
    }

    /**
     * Retrieve whether or not the Wave data has been loaded.
     *
     * @return True or false depending on whether the wave data has been loaded.
     */
    [[nodiscard]] bool IsReady() const noexcept {
        return ::IsWaveReady(m_data);
    }

 protected:
    constexpr void set(const ::Wave& wave) noexcept {
        m_data.frameCount = wave.frameCount;
        m_data.sampleRate = wave.sampleRate;
        m_data.sampleSize = wave.sampleSize;
        m_data.channels = wave.channels;
        m_data.data = wave.data;
    }

    ::Wave m_data;
};

}  // namespace raylib

using RWave = raylib::Wave;

#endif  // RAYLIB_CPP_INCLUDE_WAVE_HPP_
