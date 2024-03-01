#ifndef RAYLIB_CPP_INCLUDE_FILEDATA_HPP_
#define RAYLIB_CPP_INCLUDE_FILEDATA_HPP_

#include <bit>
#include <string>
#include <filesystem>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#ifdef __cpp_exceptions
#include "./RaylibException.hpp"
#endif
#include "./RaylibError.hpp"

namespace raylib {

class FileData {
public:
    constexpr FileData() = default;
    ~FileData() { Unload(); }

    explicit FileData(const std::filesystem::path& fileName) {
        Load(fileName);
    }

    GETTER(const unsigned char*, Data, m_data)
    GETTER(int, BytesRead, m_bytesRead)

    [[nodiscard]] std::span<const unsigned char> AsSpan() const noexcept {
        return std::span<const unsigned char>{m_data, static_cast<size_t>(m_bytesRead)};
    }
    [[nodiscard]] std::span<unsigned char> AsSpan() noexcept {
        return std::span<unsigned char>{m_data, static_cast<size_t>(m_bytesRead)};
    }

    [[nodiscard]] std::span<std::byte> AsSpanBytes() noexcept {
        return std::as_writable_bytes(std::span<unsigned char>{m_data, static_cast<size_t>(m_bytesRead)});
    }
    [[nodiscard]] std::span<const std::byte> AsSpanBytes() const noexcept {
        return std::as_bytes(std::span<const unsigned char>{m_data, static_cast<size_t>(m_bytesRead)});
    }

    void Load(const std::filesystem::path& fileName) noexcept { Load(fileName.c_str()); }
    void Load(const char* fileName) noexcept {
        m_data = ::LoadFileData(fileName, &m_bytesRead);
    }

    void Unload() noexcept {
        if (m_data != nullptr) {
            ::UnloadFileData(m_data);
            m_data = nullptr;
        }
    }

private:
    owner<unsigned char*> m_data{nullptr};
    int m_bytesRead{0};
};

}  // namespace raylib

using RFileData = raylib::FileData;

#endif  // RAYLIB_CPP_INCLUDE_FILEDATA_HPP_
