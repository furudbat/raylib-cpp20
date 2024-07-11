#ifndef RAYLIB_CPP_INCLUDE_FILETEXT_HPP_
#define RAYLIB_CPP_INCLUDE_FILETEXT_HPP_

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

class FileText {
public:
    constexpr FileText() = default;
    FileText(const FileText&) = delete;
    FileText(FileText&& other) noexcept : m_data(other.m_data), m_length(other.m_length) {
        other.m_data = nullptr;
        other.m_length = 0;
    }
    FileText& operator=(const FileText&) = delete;
    FileText& operator=(FileText&& other) noexcept {
        std::exchange(m_data, other.m_data);
        std::exchange(m_length, other.m_length);
        return *this;
    }
    ~FileText() noexcept { Unload(); }

    explicit FileText(const std::filesystem::path& fileName) {
        Load(fileName);
    }
    explicit FileText(czstring fileName) {
        Load(fileName);
    }

    GETTER(const char*, Data, m_data)
    GETTER(unsigned int, Length, m_length)

    [[nodiscard]] const char* c_str() const noexcept { return m_data; }

    [[nodiscard]] std::string_view ToStringView() const noexcept { return std::string_view(m_data, m_length); }
    explicit operator std::string_view() const noexcept {
        return std::string_view(m_data, m_length);
    }

    [[nodiscard]] std::string ToString() const { return m_data; }
    explicit operator std::string() const {
        return m_data;
    }

    void Load(czstring fileName) noexcept {
        m_data = ::LoadFileText(fileName);
        m_length = ::TextLength(m_data);
    }
    void Load(const std::filesystem::path& fileName) { Load(fileName.c_str()); }

    void Unload() noexcept {
        if (m_data != nullptr) {
            ::UnloadFileText(m_data);
            m_data = nullptr;
            m_length = 0;
        }
    }

private:
    owner<char*> m_data{nullptr};
    unsigned int m_length{0};
};

}  // namespace raylib

using RFileText = raylib::FileText;

#endif  // RAYLIB_CPP_INCLUDE_FILETEXT_HPP_
