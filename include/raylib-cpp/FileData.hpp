#ifndef RAYLIB_CPP_INCLUDE_FILEDATA_HPP_
#define RAYLIB_CPP_INCLUDE_FILEDATA_HPP_


#include "raylib.hpp"
#include "raylib-cpp-utils.hpp"
#ifdef __cpp_exceptions
#include "RaylibException.hpp"
#endif
#include "RaylibError.hpp"

#include <bit>
#include <string>
#include <filesystem>

namespace raylib {

class FileData {
public:
    constexpr FileData() = default;
    FileData(const FileData&) = delete;
    constexpr FileData(FileData&& other) noexcept : m_data(other.m_data), m_bytesRead(other.m_bytesRead) {
        other.m_data = nullptr;
        other.m_bytesRead = 0;
    }
    FileData& operator=(const FileData&) = delete;
    constexpr FileData& operator=(FileData&& other) noexcept {
        std::swap(m_data, other.m_data);
        std::swap(m_bytesRead, other.m_bytesRead);
        return *this;
    }
    ~FileData() noexcept { Unload(); }

    explicit FileData(czstring fileName) {
        Load(fileName);
    }
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

    void Load(const std::filesystem::path& fileName) { Load(fileName.c_str()); }
    void Load(czstring fileName) noexcept {
        m_data = ::LoadFileData(fileName, &m_bytesRead);
    }

    void Unload() noexcept {
        if (m_data != nullptr) {
            ::UnloadFileData(m_data);
            m_data = nullptr;
            m_bytesRead = 0;
        }
    }

private:
    owner<unsigned char*> m_data{nullptr};
    int m_bytesRead{0};
};

}  // namespace raylib

using RFileData = raylib::FileData;

#endif  // RAYLIB_CPP_INCLUDE_FILEDATA_HPP_
