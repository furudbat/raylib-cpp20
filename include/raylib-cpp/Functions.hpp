/**
 * C++ wrapper functions for raylib.
 */
#ifndef RAYLIB_CPP_INCLUDE_FUNCTIONS_HPP_
#define RAYLIB_CPP_INCLUDE_FUNCTIONS_HPP_

#include <filesystem>
#include <string>
#include <vector>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"

namespace raylib {

class RayDirectoryFilesFilePathList {
public:
    FilePathList files;

    explicit RayDirectoryFilesFilePathList(const std::filesystem::path& dirPath) : files(::LoadDirectoryFiles(dirPath.c_str())) {}
    ~RayDirectoryFilesFilePathList() noexcept {
        ::UnloadDirectoryFiles(files);
    }
};
class RayDroppedFilesFilePathList {
public:
    FilePathList files;

    RayDroppedFilesFilePathList() : files(::LoadDroppedFiles()) {}
    ~RayDroppedFilesFilePathList() noexcept {
        ::UnloadDroppedFiles(files);
    }
};

/**
 * Initialize window and OpenGL context
 */
[[maybe_unused]] RLCPPAPI inline void InitWindow(int width, int height, const std::string& title = "raylib") {
    ::InitWindow(width, height, title.c_str());
}

/**
 * Set title for window
 */
[[maybe_unused]] RLCPPAPI inline void SetWindowTitle(const std::string& title) {
    ::SetWindowTitle(title.c_str());
}

/**
 * Get the human-readable, UTF-8 encoded name of the primary monitor
 */
[[maybe_unused]] RLCPPAPI inline std::string GetMonitorName(int monitor = 0) noexcept {
    return ::GetMonitorName(monitor);
}

/**
 * Set clipboard text content
 */
[[maybe_unused]] RLCPPAPI inline void SetClipboardText(const std::string& text) {
    ::SetClipboardText(text.c_str());
}

/**
 * Get clipboard text content
 */
[[maybe_unused]] RLCPPAPI inline std::string GetClipboardText() {
    return ::GetClipboardText();
}

/**
 * Takes a screenshot of current screen (saved a .png)
 */
[[maybe_unused]] RLCPPAPI inline void TakeScreenshot(const std::filesystem::path& fileName) {
    ::TakeScreenshot(fileName.c_str());
}

/**
 * Get gamepad internal name id
 */
[[maybe_unused]] RLCPPAPI inline std::string GetGamepadName(int gamepad) {
    return ::GetGamepadName(gamepad);
}

/**
 * Load text data from file (read)
 */
[[maybe_unused]] RLCPPAPI std::string LoadFileText(const std::filesystem::path& fileName) {
    char* text = ::LoadFileText(fileName.c_str());
    std::string output(text);
    ::UnloadFileText(text);
    return output;
}

/**
 * Save text data to file (write)
 */
[[maybe_unused]] RLCPPAPI inline bool SaveFileText(const std::filesystem::path& fileName, const std::string& text) {
    return ::SaveFileText(fileName.c_str(), const_cast<char*>(text.c_str()));
}

/**
 * Check if file exists
 */
[[maybe_unused]] RLCPPAPI inline bool FileExists(const std::filesystem::path& fileName) {
    return ::FileExists(fileName.c_str());
}

/**
 * Check if directory path exists
 */
[[maybe_unused]] RLCPPAPI inline bool DirectoryExists(const std::filesystem::path& dirPath) {
    return ::DirectoryExists(dirPath.c_str());
}

/**
 * Check file extension (including point: .png, .wav)
 */
[[maybe_unused]] RLCPPAPI inline bool IsFileExtension(const std::filesystem::path& fileName, const std::string& ext) {
    return ::IsFileExtension(fileName.c_str(), ext.c_str());
}

/**
 * Get pointer to extension for a filename string (including point: ".png")
 */
[[maybe_unused]] RLCPPAPI inline std::string GetFileExtension(const std::filesystem::path& fileName) {
    return ::GetFileExtension(fileName.c_str());
}

/**
 * Get pointer to filename for a path string
 */
[[maybe_unused]] RLCPPAPI inline std::string GetFileName(const std::filesystem::path& filePath) {
    return ::GetFileName(filePath.c_str());
}

/**
 * Get filename string without extension
 */
[[maybe_unused]] RLCPPAPI inline std::string GetFileNameWithoutExt(const std::filesystem::path& filePath) {
    return ::GetFileNameWithoutExt(filePath.c_str());
}

/**
 * Get full path for a given fileName with path
 */
[[maybe_unused]] RLCPPAPI inline std::string GetDirectoryPath(const std::filesystem::path& filePath) {
    return ::GetDirectoryPath(filePath.c_str());
}

/**
 * Get previous directory path for a given path
 */
[[maybe_unused]] RLCPPAPI inline std::string GetPrevDirectoryPath(const std::filesystem::path& dirPath) {
    return ::GetPrevDirectoryPath(dirPath.c_str());
}

/**
 * Get current working directory
 */
[[maybe_unused]] RLCPPAPI inline std::filesystem::path GetWorkingDirectory() {
    return ::GetWorkingDirectory();
}

/**
 * Get filenames in a directory path
 */
[[maybe_unused]] RLCPPAPI std::vector<std::filesystem::path> LoadDirectoryFiles(const std::filesystem::path& dirPath) {
    auto filesList = RayDirectoryFilesFilePathList(dirPath);
    return std::vector<std::filesystem::path>(filesList.files.paths, std::next(filesList.files.paths, filesList.files.count));
}

/**
 * Change working directory, return true on success
 */
[[maybe_unused]] RLCPPAPI inline bool ChangeDirectory(const std::filesystem::path& dir) {
    return ::ChangeDirectory(dir.c_str());
}

/**
 * Get dropped files names
 */
[[maybe_unused]] RLCPPAPI std::vector<std::filesystem::path> LoadDroppedFiles() {
    if (!::IsFileDropped()) {
        return {};
    }
    auto filesList = RayDroppedFilesFilePathList();
    return std::vector<std::filesystem::path>(filesList.files.paths, std::next(filesList.files.paths, filesList.files.count));
}

/**
 * Get file modification time (last write time)
 */
[[maybe_unused]] RLCPPAPI inline long GetFileModTime(const std::filesystem::path& fileName) { // NOLINT
    return ::GetFileModTime(fileName.c_str());
}

/**
 * Open URL with default system browser (if available)
 */
[[maybe_unused]] RLCPPAPI inline void OpenURL(const std::string& url) {
    return ::OpenURL(url.c_str());
}

/**
 * Load an image.
 */
#ifdef RAYLIB_CPP_EXPECTED
[[maybe_unused]] RLCPPAPI inline RAYLIB_CPP_EXPECTED_RESULT(raylib::Image) LoadImage(const std::filesystem::path& fileName) {
    raylib::Image ret;
    if (auto result = ret.Load(fileName); !result) [[unlikely]] {
        RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(result.error());
    }
    RAYLIB_CPP_RETURN_EXPECTED_VALUE(ret);
}
#else
[[maybe_unused]] RLCPPAPI inline raylib::Image LoadImage(const std::filesystem::path& fileName) {
    return raylib::Image{::LoadImage(fileName.c_str())};
}
#endif

/**
 * Load an image from RAW file data
 */
#ifdef RAYLIB_CPP_EXPECTED
[[maybe_unused]] RLCPPAPI inline RAYLIB_CPP_EXPECTED_RESULT(raylib::Image) LoadImageRaw(const std::filesystem::path& fileName, int width, int height, int format, int headerSize) {
    raylib::Image ret;
    if (auto result = ret.LoadRaw(fileName, width, height, format, headerSize); !result) [[unlikely]] {
        RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(result.error());
    }
    RAYLIB_CPP_RETURN_EXPECTED_VALUE(ret);
}
#else
[[maybe_unused]] RLCPPAPI inline raylib::Image LoadImageRaw(const std::filesystem::path& fileName,
        int width, int height,
        int format, int headerSize) {
    return raylib::Image{::LoadImageRaw(fileName.c_str(), width, height, format, headerSize)};
}
#endif

/**
 * Load animated image data
 */
struct LoadImageAnimResult { raylib::Image image; int frames; };
#ifdef RAYLIB_CPP_EXPECTED
[[maybe_unused]] RLCPPAPI inline RAYLIB_CPP_EXPECTED_RESULT(LoadImageAnimResult) LoadImageRaw(const std::filesystem::path& fileName) {
    LoadImageAnimResult ret;
    if (auto result = ret.image.LoadAnim(fileName, ret.frames); !result) [[unlikely]] {
        RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(result.error());
    }
    RAYLIB_CPP_RETURN_EXPECTED_VALUE(ret);
}
#else
[[maybe_unused]] RLCPPAPI inline raylib::Image LoadImageAnim(const std::filesystem::path& fileName, int &frames) {
    return raylib::Image{::LoadImageAnim(fileName.c_str(), &frames)};
}
[[maybe_unused]] RLCPPAPI inline LoadImageAnimResult LoadImageAnim(const std::filesystem::path& fileName) {
    int frames{0};
    auto image = raylib::Image{::LoadImageAnim(fileName.c_str(), &frames)};
    return { .image = image, .frames = frames };
}
#endif

/**
 * Load image from memory buffer, fileType refers to extension like "png"
 */
#ifdef RAYLIB_CPP_EXPECTED
[[maybe_unused]] RLCPPAPI inline RAYLIB_CPP_EXPECTED_RESULT(raylib::Image) LoadImageFromMemory(const std::string& fileType, std::span<const unsigned char> fileData) {
    raylib::Image ret;
    if (auto result = ret.LoadFromMemory(fileType, fileData); !result) [[unlikely]] {
        RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(result.error());
    }
    RAYLIB_CPP_RETURN_EXPECTED_VALUE(ret);
}
#else
[[maybe_unused]] RLCPPAPI inline raylib::Image LoadImageFromMemory(const std::string& fileType,
                                                                   std::span<const unsigned char> fileData) {
    return raylib::Image{::LoadImageFromMemory(fileType.c_str(), fileData.data(), static_cast<int>(fileData.size()))};
}
#endif

#ifdef RAYLIB_CPP_EXPECTED
[[maybe_unused]] RLCPPAPI inline RAYLIB_CPP_EXPECTED_RESULT(raylib::Image) LoadImageFromTexture(const ::Texture2D& texture) {
    raylib::Image ret;
    if (auto result = ret.LoadFromTexture(texture); !result) [[unlikely]] {
        RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(result.error());
    }
    RAYLIB_CPP_RETURN_EXPECTED_VALUE(ret);
}
#endif

/**
 * Export image data to file
 */
#ifdef RAYLIB_CPP_EXPECTED
[[maybe_unused]] RLCPPAPI inline RAYLIB_CPP_EXPECTED_RESULT(void) ExportImage(const raylib::Image& image, const std::filesystem::path& fileName) {
    return image.Export(fileName);
}
#else
[[maybe_unused]] RLCPPAPI inline bool ExportImage(const raylib::Image& image, const std::filesystem::path& fileName) {
    return ::ExportImage(image.c_raylib(), fileName.c_str());
}
#endif

/**
 * Export image as code file (.h) defining an array of bytes
 */
#ifdef RAYLIB_CPP_EXPECTED
[[maybe_unused]] RLCPPAPI inline RAYLIB_CPP_EXPECTED_RESULT(void) ExportImageAsCode(const raylib::Image& image, const std::filesystem::path& fileName) {
    return image.ExportAsCode(fileName);
}
#else
[[maybe_unused]] RLCPPAPI inline bool ExportImageAsCode(const raylib::Image& image, const std::filesystem::path& fileName) {
    return ::ExportImageAsCode(image.c_raylib(), fileName.c_str());
}
#endif

/**
 * Draw text (using default font)
 */
[[maybe_unused]] RLCPPAPI inline void DrawText(const char* text, int posX, int posY, int fontSize, ::Color color) noexcept {
    ::DrawText(text, posX, posY, fontSize, color);
}

/**
 * Draw text (using default font)
 */
[[maybe_unused]] RLCPPAPI inline void DrawText(
        const std::string& text,
        int posX, int posY,
        int fontSize,
        ::Color color) {
    ::DrawText(text.c_str(), posX, posY, fontSize, color);
}

/**
 * Draw text using font and additional parameters
 */
[[maybe_unused]] RLCPPAPI inline void DrawTextEx(const Font& font, const char* text, ::Vector2 position,
                                                 float fontSize, float spacing, ::Color tint) {
    ::DrawTextEx(font.c_raylib(), text, position, fontSize, spacing, tint);
}

/**
 * Draw text using font and additional parameters
 */
[[maybe_unused]] RLCPPAPI inline void DrawTextEx(const Font& font, const std::string& text, ::Vector2 position,
        float fontSize, float spacing, ::Color tint) {
    ::DrawTextEx(font.c_raylib(), text.c_str(), position, fontSize, spacing, tint);
}

/**
 * Draw text using Font and pro parameters (rotation)
 */
[[maybe_unused]] RLCPPAPI inline void DrawTextPro(const Font& font, const char* text, ::Vector2 position,
        ::Vector2 origin, float rotation, float fontSize, float spacing, ::Color tint) {
    ::DrawTextPro(font.c_raylib(), text, position, origin, rotation, fontSize, spacing, tint);
}

/**
 * Draw text using Font and pro parameters (rotation)
 */
[[maybe_unused]] RLCPPAPI inline void DrawTextPro(const Font& font, const std::string& text, ::Vector2 position,
        ::Vector2 origin, float rotation, float fontSize, float spacing, ::Color tint) {
    ::DrawTextPro(font.c_raylib(), text.c_str(), position, origin, rotation, fontSize, spacing, tint);
}

/**
 * Load font from file (filename must include file extension)
 */
[[maybe_unused]] RLCPPAPI inline ::Font LoadFont(const std::filesystem::path& fileName) {
    return ::LoadFont(fileName.c_str());
}

/**
 * Load font from file (filename must include file extension)
 */
[[maybe_unused]] RLCPPAPI inline ::Font LoadFontEx(const std::filesystem::path& fileName, int fontSize, std::span<int> fontChars) {
    return ::LoadFontEx(fileName.c_str(), fontSize, fontChars.data(), static_cast<int>(fontChars.size()));
}

/**
 * Measure string width for default font
 */
[[maybe_unused]] RLCPPAPI inline int MeasureText(const char* text, int fontSize) noexcept {
    return ::MeasureText(text, fontSize);
}

/**
 * Measure string width for default font
 */
[[maybe_unused]] RLCPPAPI inline int MeasureText(const std::string& text, int fontSize) {
    return ::MeasureText(text.c_str(), fontSize);
}

/**
 * Check if two text string are equal
 */
[[maybe_unused]] RLCPPAPI inline bool TextIsEqual(const char* text1, const char* text2) {
    return ::TextIsEqual(text1, text2);
}

/**
 * Check if two text string are equal
 */
[[maybe_unused]] RLCPPAPI inline bool TextIsEqual(const std::string& text1, const std::string& text2) {
    return ::TextIsEqual(text1.c_str(), text2.c_str());
}

/**
 * Check if two text string are equal
 */
[[maybe_unused]] RLCPPAPI inline unsigned int TextLength(const char* text) noexcept {
    return ::TextLength(text);
}

/**
 * Check if two text string are equal
 */
[[maybe_unused]] RLCPPAPI inline unsigned int TextLength(const std::string& text) {
    return ::TextLength(text.c_str());
}

/**
 * Get text length, checks for '\0' ending
 */
[[maybe_unused]] RLCPPAPI inline std::string TextSubtext(const std::string& text, int position, int length) {
    return ::TextSubtext(text.c_str(), position, length);
}

/**
 * Replace text string
 */
[[maybe_unused]] RLCPPAPI std::string TextReplace(const std::string& text, const std::string& replace, const std::string& by) {
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
[[maybe_unused]] RLCPPAPI std::string TextInsert(const std::string& text, const std::string& insert, int position) {
    auto output = RayUniquePtr<char>(::TextInsert(text.c_str(), insert.c_str(), position));
    if (output != nullptr) {
        return output.get();
    }
    return "";
}

/**
 * Split text into multiple strings
 */
[[maybe_unused]] RLCPPAPI std::vector<std::string> TextSplit(const std::string& text, char delimiter) {
    int count{0};
    const char** split = ::TextSplit(text.c_str(), delimiter, &count);
    return std::vector<std::string>(split, std::next(split, count));
}

/**
 * Find first text occurrence within a string
 */
[[maybe_unused]] RLCPPAPI inline int TextFindIndex(const std::string& text, const std::string& find) {
    return ::TextFindIndex(text.c_str(), find.c_str());
}

/**
 * Get upper case version of provided string
 */
[[maybe_unused]] RLCPPAPI inline std::string TextToUpper(const std::string& text) {
    return ::TextToUpper(text.c_str());
}

/**
 * Get lower case version of provided string
 */
[[maybe_unused]] RLCPPAPI inline std::string TextToLower(const std::string& text) {
    return ::TextToLower(text.c_str());
}

/**
 * Get Pascal case notation version of provided string
 */
[[maybe_unused]] RLCPPAPI inline std::string TextToPascal(const std::string& text) {
    return ::TextToPascal(text.c_str());
}

/**
 * Get integer value from text (negative values not supported)
 */
[[maybe_unused]] RLCPPAPI inline int TextToInteger(const std::string& text) {
    return ::TextToInteger(text.c_str());
}

#ifdef RAYLIB_CPP_EXPECTED
inline RAYLIB_CPP_EXPECTED_RESULT(AudioDevice) InitAudioDevice() {
    AudioDevice ret;
    if (auto result = ret.Init(); !result) [[unlikely]] {
        RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(result.error());
    }
    RAYLIB_CPP_RETURN_EXPECTED_VALUE(ret);
}
#endif


#ifdef RAYLIB_CPP_EXPECTED
inline RAYLIB_CPP_EXPECTED_RESULT(AutomationEventList) LoadAutomationEventList(const std::filesystem::path& fileName) {
    AutomationEventList ret;
    if (auto result = ret.Load(fileName); !result) [[unlikely]] {
        RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(result.error());
    }
    RAYLIB_CPP_RETURN_EXPECTED_VALUE(ret);
}
#endif

[[maybe_unused]] RLCPPAPI inline std::chrono::milliseconds GetFrameTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>{::GetFrameTime()});
}

}  // namespace raylib

#endif  // RAYLIB_CPP_INCLUDE_FUNCTIONS_HPP_
