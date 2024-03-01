#ifndef RAYLIB_CPP_INCLUDE_FILETEXT_HPP_
#define RAYLIB_CPP_INCLUDE_FILETEXT_HPP_

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

class FileText {
public:
    constexpr FileText() = default;
    ~FileText() { Unload(); }

    explicit FileText(const std::filesystem::path& fileName) {
        Load(fileName);
    }

    GETTER(const char*, Data, m_data)
    GETTER(unsigned int, Length, m_length)

    [[nodiscard]] const char* c_str() const noexcept { return m_data; }

    [[nodiscard]] std::string_view ToStringView() const noexcept { return std::string_view(m_data, m_length); }
    explicit operator std::string_view() const {
        return std::string_view(m_data, m_length);
    }

    [[nodiscard]] std::string ToString() const noexcept { return m_data; }
    explicit operator std::string() const {
        return m_data;
    }

    void Load(const std::filesystem::path& fileName) noexcept { Load(fileName.c_str()); }
    void Load(const char* fileName) noexcept {
        m_data = ::LoadFileText(fileName);
        m_length = ::TextLength(m_data);
    }

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
