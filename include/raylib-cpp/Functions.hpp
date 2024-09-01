/**
 * C++ wrapper functions for raylib.
 */
#ifndef RAYLIB_CPP_INCLUDE_FUNCTIONS_HPP_
#define RAYLIB_CPP_INCLUDE_FUNCTIONS_HPP_

#include "enums.hpp"
#include "raylib-cpp-utils.hpp"
#include "raylib.hpp"

#include <filesystem>
#include <string>
#include <vector>

namespace raylib {

static inline constexpr auto Deg2Rad = DEG2RAD;
static inline constexpr auto Rad2Deg = RAD2DEG;

class RayDirectoryFilesFilePathList {
public:
    FilePathList files;

    explicit RayDirectoryFilesFilePathList(const std::filesystem::path& dirPath)
        : files(::LoadDirectoryFiles(dirPath.c_str())) {}
    ~RayDirectoryFilesFilePathList() noexcept { ::UnloadDirectoryFiles(files); }
};
class RayDroppedFilesFilePathList {
public:
    FilePathList files;

    RayDroppedFilesFilePathList() : files(::LoadDroppedFiles()) {}
    ~RayDroppedFilesFilePathList() noexcept { ::UnloadDroppedFiles(files); }
};

/**
 * Initialize window and OpenGL context
 */
[[maybe_unused]] RLCPPAPI_INLINE void InitWindow(int width, int height, const std::string& title = "raylib") {
    ::InitWindow(width, height, title.c_str());
}

/**
 * Set title for window
 */
[[maybe_unused]] RLCPPAPI_INLINE void SetWindowTitle(const std::string& title) {
    ::SetWindowTitle(title.c_str());
}

/**
 * Get the human-readable, UTF-8 encoded name of the primary monitor
 */
[[maybe_unused]] RLCPPAPI_INLINE std::string GetMonitorName(int monitor = 0) noexcept {
    return ::GetMonitorName(monitor);
}

/**
 * Set clipboard text content
 */
[[maybe_unused]] RLCPPAPI_INLINE void SetClipboardText(const std::string& text) {
    ::SetClipboardText(text.c_str());
}

/**
 * Get clipboard text content
 */
[[maybe_unused]] RLCPPAPI_INLINE std::string GetClipboardText() {
    return ::GetClipboardText();
}

/**
 * Takes a screenshot of current screen (saved a .png)
 */
[[maybe_unused]] RLCPPAPI_INLINE void TakeScreenshot(const std::filesystem::path& fileName) {
    ::TakeScreenshot(fileName.c_str());
}

/**
 * Get gamepad internal name id
 */
[[maybe_unused]] RLCPPAPI_INLINE std::string GetGamepadName(int gamepad) {
    return ::GetGamepadName(gamepad);
}

/**
 * Load text data from file (read)
 */
[[maybe_unused]] RLCPPAPI_INLINE std::string LoadFileText(const std::filesystem::path& fileName) {
    char* text = ::LoadFileText(fileName.c_str());
    std::string output(text);
    ::UnloadFileText(text);
    return output;
}

/**
 * Save text data to file (write)
 */
[[maybe_unused]] RLCPPAPI_INLINE bool SaveFileText(const std::filesystem::path& fileName, const std::string& text) {
    return ::SaveFileText(fileName.c_str(), const_cast<char*>(text.c_str()));
}

/**
 * Check if file exists
 */
[[maybe_unused]] RLCPPAPI_INLINE bool FileExists(const std::filesystem::path& fileName) {
    return ::FileExists(fileName.c_str());
}

/**
 * Check if directory path exists
 */
[[maybe_unused]] RLCPPAPI_INLINE bool DirectoryExists(const std::filesystem::path& dirPath) {
    return ::DirectoryExists(dirPath.c_str());
}

/**
 * Check file extension (including point: .png, .wav)
 */
[[maybe_unused]] RLCPPAPI_INLINE bool IsFileExtension(const std::filesystem::path& fileName, const std::string& ext) {
    return ::IsFileExtension(fileName.c_str(), ext.c_str());
}

/**
 * Get pointer to extension for a filename string (including point: ".png")
 */
[[maybe_unused]] RLCPPAPI_INLINE std::string GetFileExtension(const std::filesystem::path& fileName) {
    return ::GetFileExtension(fileName.c_str());
}

/**
 * Get pointer to filename for a path string
 */
[[maybe_unused]] RLCPPAPI_INLINE std::string GetFileName(const std::filesystem::path& filePath) {
    return ::GetFileName(filePath.c_str());
}

/**
 * Get filename string without extension
 */
[[maybe_unused]] RLCPPAPI_INLINE std::string GetFileNameWithoutExt(const std::filesystem::path& filePath) {
    return ::GetFileNameWithoutExt(filePath.c_str());
}

/**
 * Get full path for a given fileName with path
 */
[[maybe_unused]] RLCPPAPI_INLINE std::string GetDirectoryPath(const std::filesystem::path& filePath) {
    return ::GetDirectoryPath(filePath.c_str());
}

/**
 * Get previous directory path for a given path
 */
[[maybe_unused]] RLCPPAPI_INLINE std::string GetPrevDirectoryPath(const std::filesystem::path& dirPath) {
    return ::GetPrevDirectoryPath(dirPath.c_str());
}

/**
 * Get current working directory
 */
[[maybe_unused]] RLCPPAPI_INLINE std::filesystem::path GetWorkingDirectory() {
    return ::GetWorkingDirectory();
}

/**
 * Get filenames in a directory path
 */
[[maybe_unused]] RLCPPAPI_INLINE std::vector<std::filesystem::path>
LoadDirectoryFiles(const std::filesystem::path& dirPath) {
    auto filesList = RayDirectoryFilesFilePathList(dirPath);
    return std::vector<std::filesystem::path>(
        filesList.files.paths,
        std::next(filesList.files.paths, filesList.files.count));
}

/**
 * Change working directory, return true on success
 */
[[maybe_unused]] RLCPPAPI_INLINE bool ChangeDirectory(const std::filesystem::path& dir) {
    return ::ChangeDirectory(dir.c_str());
}

/**
 * Get dropped files names
 */
[[maybe_unused]] RLCPPAPI_INLINE std::vector<std::filesystem::path> LoadDroppedFiles() {
    if (!::IsFileDropped()) {
        return {};
    }
    auto filesList = RayDroppedFilesFilePathList();
    return std::vector<std::filesystem::path>(
        filesList.files.paths,
        std::next(filesList.files.paths, filesList.files.count));
}

/**
 * Get file modification time (last write time)
 */
[[maybe_unused]] RLCPPAPI_INLINE long GetFileModTime(const std::filesystem::path& fileName) { // NOLINT
    return ::GetFileModTime(fileName.c_str());
}

/**
 * Open URL with default system browser (if available)
 */
[[maybe_unused]] RLCPPAPI_INLINE void OpenURL(const std::string& url) {
    return ::OpenURL(url.c_str());
}


struct LoadImageAnimResult {
    raylib::Image image;
    int frames;
};
/**
 * Load animated image data
 */
#ifdef RAYLIB_CPP_EXPECTED
[[maybe_unused]] RLCPPAPI_INLINE RAYLIB_CPP_EXPECTED_RESULT(LoadImageAnimResult)
    LoadImageRaw(const std::filesystem::path& fileName) {
    LoadImageAnimResult ret;
    if (auto result = ret.image.LoadAnim(fileName, ret.frames); !result) [[unlikely]] {
        RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(result.error());
    }
    RAYLIB_CPP_RETURN_EXPECTED_VALUE(ret);
}
#else
[[maybe_unused]] RLCPPAPI_INLINE LoadImageAnimResult LoadImageAnim(const std::filesystem::path& fileName) {
    int frames{0};
    raylib::Image image;
    image.LoadAnim(fileName.c_str(), frames);
    return {.image = image, .frames = frames};
}
#endif


/**
 * Draw text (using default font)
 */
[[maybe_unused]] RLCPPAPI_INLINE void
DrawText(czstring text, int posX, int posY, int fontSize, ::Color color) noexcept {
    ::DrawText(text, posX, posY, fontSize, color);
}
[[maybe_unused]] RLCPPAPI_INLINE void
DrawText(const std::string& text, int posX, int posY, int fontSize, ::Color color) {
    ::DrawText(text.c_str(), posX, posY, fontSize, color);
}

/**
 * Draw text using font and additional parameters
 */
[[maybe_unused]] RLCPPAPI_INLINE void
DrawTextEx(const Font& font, czstring text, ::Vector2 position, float fontSize, float spacing, ::Color tint) {
    ::DrawTextEx(font.c_raylib(), text, position, fontSize, spacing, tint);
}

/**
 * Draw text using font and additional parameters
 */
[[maybe_unused]] RLCPPAPI_INLINE void
DrawTextEx(const Font& font, const std::string& text, ::Vector2 position, float fontSize, float spacing, ::Color tint) {
    ::DrawTextEx(font.c_raylib(), text.c_str(), position, fontSize, spacing, tint);
}

/**
 * Draw text using Font and pro parameters (rotation)
 */
[[maybe_unused]] RLCPPAPI_INLINE void DrawTextPro(
    const Font& font,
    czstring text,
    ::Vector2 position,
    ::Vector2 origin,
    float rotation,
    float fontSize,
    float spacing,
    ::Color tint) {
    ::DrawTextPro(font.c_raylib(), text, position, origin, rotation, fontSize, spacing, tint);
}

/**
 * Draw text using Font and pro parameters (rotation)
 */
[[maybe_unused]] RLCPPAPI_INLINE void DrawTextPro(
    const Font& font,
    const std::string& text,
    ::Vector2 position,
    ::Vector2 origin,
    float rotation,
    float fontSize,
    float spacing,
    ::Color tint) {
    ::DrawTextPro(font.c_raylib(), text.c_str(), position, origin, rotation, fontSize, spacing, tint);
}

/**
 * Load font from file (filename must include file extension)
 */
[[maybe_unused]] RLCPPAPI_INLINE ::Font LoadFont(const std::filesystem::path& fileName) {
    return ::LoadFont(fileName.c_str());
}

/**
 * Load font from file (filename must include file extension)
 */
[[maybe_unused]] RLCPPAPI_INLINE ::Font
LoadFontEx(const std::filesystem::path& fileName, int fontSize, std::span<int> fontChars) {
    return ::LoadFontEx(fileName.c_str(), fontSize, fontChars.data(), static_cast<int>(fontChars.size()));
}

/**
 * Measure string width for default font
 */
[[maybe_unused]] RLCPPAPI_INLINE int MeasureText(czstring text, int fontSize) noexcept {
    return ::MeasureText(text, fontSize);
}

/**
 * Measure string width for default font
 */
[[maybe_unused]] RLCPPAPI_INLINE int MeasureText(const std::string& text, int fontSize) {
    return ::MeasureText(text.c_str(), fontSize);
}

/**
 * Check if two text string are equal
 */
[[maybe_unused]] RLCPPAPI_INLINE bool TextIsEqual(czstring text1, const char* text2) {
    return ::TextIsEqual(text1, text2);
}

/**
 * Check if two text string are equal
 */
[[maybe_unused]] RLCPPAPI_INLINE bool TextIsEqual(const std::string& text1, const std::string& text2) {
    return ::TextIsEqual(text1.c_str(), text2.c_str());
}

/**
 * Check if two text string are equal
 */
[[maybe_unused]] RLCPPAPI_INLINE unsigned int TextLength(czstring text) noexcept {
    return ::TextLength(text);
}

/**
 * Check if two text string are equal
 */
[[maybe_unused]] RLCPPAPI_INLINE unsigned int TextLength(const std::string& text) {
    return ::TextLength(text.c_str());
}

/**
 * Get text length, checks for '\0' ending
 */
[[maybe_unused]] RLCPPAPI_INLINE std::string TextSubtext(const std::string& text, int position, int length) {
    return ::TextSubtext(text.c_str(), position, length);
}

/**
 * Replace text string
 */
[[maybe_unused]] RLCPPAPI_INLINE std::string
TextReplace(const std::string& text, const std::string& replace, const std::string& by) {
    const char* input = text.c_str();
    auto output = RayUniquePtr<char>(::TextReplace(const_cast<char*>(input), replace.c_str(), by.c_str()));
    if (output != nullptr) {
        return output.get();
    }
    return "";
}

/**
 * Insert text in a position
 */
[[maybe_unused]] RLCPPAPI_INLINE std::string
TextInsert(const std::string& text, const std::string& insert, int position) {
    auto output = RayUniquePtr<char>(::TextInsert(text.c_str(), insert.c_str(), position));
    if (output != nullptr) {
        return output.get();
    }
    return "";
}

/**
 * Split text into multiple strings
 */
[[maybe_unused]] RLCPPAPI_INLINE std::vector<std::string> TextSplit(const std::string& text, char delimiter) {
    int count{0};
    const char** split = ::TextSplit(text.c_str(), delimiter, &count);
    return std::vector<std::string>(split, std::next(split, count));
}

/**
 * Find first text occurrence within a string
 */
[[maybe_unused]] RLCPPAPI int TextFindIndex(const std::string& text, const std::string& find) {
    return ::TextFindIndex(text.c_str(), find.c_str());
}

/**
 * Get upper case version of provided string
 */
[[maybe_unused]] RLCPPAPI_INLINE std::string TextToUpper(const std::string& text) {
    return ::TextToUpper(text.c_str());
}

/**
 * Get lower case version of provided string
 */
[[maybe_unused]] RLCPPAPI_INLINE std::string TextToLower(const std::string& text) {
    return ::TextToLower(text.c_str());
}

/**
 * Get Pascal case notation version of provided string
 */
[[maybe_unused]] RLCPPAPI_INLINE std::string TextToPascal(const std::string& text) {
    return ::TextToPascal(text.c_str());
}

/**
 * Get integer value from text (negative values not supported)
 */
[[maybe_unused]] RLCPPAPI_INLINE int TextToInteger(const std::string& text) {
    return ::TextToInteger(text.c_str());
}

#ifdef RAYLIB_CPP_EXPECTED
[[maybe_unused]] RLCPPAPI_INLINE RAYLIB_CPP_EXPECTED_RESULT(AudioDevice) InitAudioDevice() {
    AudioDevice ret;
    if (auto result = ret.Init(); !result) [[unlikely]] {
        RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(result.error());
    }
    RAYLIB_CPP_RETURN_EXPECTED_VALUE(ret);
}
#endif


#ifdef RAYLIB_CPP_EXPECTED
[[maybe_unused]] RLCPPAPI_INLINE RAYLIB_CPP_EXPECTED_RESULT(AutomationEventList)
    LoadAutomationEventList(const std::filesystem::path& fileName) {
    AutomationEventList ret;
    if (auto result = ret.Load(fileName); !result) [[unlikely]] {
        RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(result.error());
    }
    RAYLIB_CPP_RETURN_EXPECTED_VALUE(ret);
}
#endif

[[maybe_unused]] RLCPPAPI_INLINE std::chrono::milliseconds GetFrameTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>{::GetFrameTime()});
}

} // namespace raylib

#endif // RAYLIB_CPP_INCLUDE_FUNCTIONS_HPP_
