#ifndef RAYLIB_CPP_INCLUDE_IMAGE_HPP_
#define RAYLIB_CPP_INCLUDE_IMAGE_HPP_

#include <string>
#include <filesystem>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "./Color.hpp"
#include "./Rectangle.hpp"
#ifdef __cpp_exceptions
#include "./RaylibException.hpp"
#endif
#include "./RaylibError.hpp"

namespace raylib {

struct RayImagePlatteDeleter {
    void operator()(::Color* arg) const {
        ::UnloadImagePalette(arg);
    }
};
using RayImagePlatte = RayArrayHolder<::Color, RayImagePlatteDeleter>;

struct RayImageColorsDeleter {
    void operator()(::Color* arg) const {
        ::UnloadImageColors(arg);
    }
};
using RayImageColors = RayArrayHolder<::Color, RayImageColorsDeleter>;

class Texture;
class Texurte2D;
class Font;

/**
 * Image type, bpp always RGBA (32bit)
 *
 * Data stored in CPU memory (RAM)
 */
class Image {
 public:
    inline static constexpr ::Color DefaultColor {255, 255, 255, 255};
    inline static constexpr ::Color DefaultClearBackgroundColor {0, 0, 0, 255};
    inline static constexpr int DefaultDrawRectangleLinesThick = 1;

    explicit constexpr Image(owner<void*> _data,
            int _width = 0,
            int _height = 0,
            int _mipmaps = 1,
            int _format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8) : m_data{_data, _width, _height, _mipmaps, _format} {
        // Nothing.
    }

    explicit constexpr Image(owner<const ::Image&> image) = delete;
    explicit constexpr Image(owner<::Image&&> image = {
            .data = nullptr,
            .width = 0,
            .height = 0,
            .mipmaps = 1,
            .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8}) noexcept {
        set(image);

        image.data = nullptr;
        image.width = 0;
        image.height = 0;
        image.mipmaps = 0;
        image.format = 0;
    }

    /**
     * Load an image from the given file.
     *
     * @throws raylib::RaylibException Thrown if the image failed to load from the file.
     *
     * @see Load()
     */
    explicit Image(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        Load(fileName);
    }

    /**
     * Load a raw image from the given file, with the provided width, height, and formats.
     *
     * @throws raylib::RaylibException Thrown if the image failed to load from the file.
     *
     * @see LoadRaw()
     */
    Image(const std::filesystem::path& fileName, int _width, int _height, int _format, int headerSize = 0) RAYLIB_CPP_THROWS {
        LoadRaw(fileName, _width, _height, _format, headerSize);
    }

    /**
     * Load an animation image from the given file.
     *
     * @throws raylib::RaylibException Thrown if the image failed to load from the file.
     *
     * @see LoadAnim()
     */
    Image(const std::filesystem::path& fileName, int& frames) RAYLIB_CPP_THROWS {
        LoadAnim(fileName, frames);
    }

    /**
     * Load an image from the given file.
     *
     * @throws raylib::RaylibException Thrown if the image failed to load from the file.
     */
    Image(const std::string& fileType, std::span<const unsigned char> fileData) RAYLIB_CPP_THROWS {
        LoadFromMemory(fileType, fileData);
    }

    /**
     * Load an image from the given file.
     *
     * @throws raylib::RaylibException Thrown if the image failed to load from the file.
     */
    explicit Image(const ::Texture2D& texture) RAYLIB_CPP_THROWS {
        LoadFromTexture(texture);
    }

    Image(int _width, int _height, ::Color color = DefaultColor) {
        set(::GenImageColor(_width, _height, color));
    }

    Image(const std::string& text, int fontSize, ::Color color = DefaultColor) {
        set(::ImageText(text.c_str(), fontSize, color));
    }

    Image(const ::Font& font, const std::string& text, float fontSize, float spacing,
            ::Color tint = DefaultColor) {
        set(::ImageTextEx(font, text.c_str(), fontSize, spacing, tint));
    }

    Image(const Image& other) {
        auto copy = other.Copy();
        set(copy.m_data);

        copy.m_data.data = nullptr;
        copy.m_data.width = 0;
        copy.m_data.height = 0;
        copy.m_data.mipmaps = 0;
        copy.m_data.format = 0;
    }
    Image(Image&& other) {
        set(other.m_data);

        other.m_data.data = nullptr;
        other.m_data.width = 0;
        other.m_data.height = 0;
        other.m_data.mipmaps = 0;
        other.m_data.format = 0;
    }


    ~Image() {
        Unload();
    }

    constexpr Image& operator=(const ::Image& image) = delete;
    constexpr Image& operator=(::Image&& image) {
        set(image);

        image.data = nullptr;
        image.width = 0;
        image.height = 0;
        image.mipmaps = 0;
        image.format = 0;

        return *this;
    }

    Image& operator=(const Image& other) {
        if (this == &other) {
            return *this;
        }

        Unload();
        auto copy = other.Copy();
        set(copy.m_data);

        copy.m_data.data = nullptr;
        copy.m_data.width = 0;
        copy.m_data.height = 0;
        copy.m_data.mipmaps = 0;
        copy.m_data.format = 0;

        return *this;
    }

    Image& operator=(Image&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other.m_data);

        other.m_data.data = nullptr;
        other.m_data.width = 0;
        other.m_data.height = 0;
        other.m_data.mipmaps = 0;
        other.m_data.format = 0;

        return *this;
    }

    explicit operator ::Image() const noexcept {
        return m_data;
    }
    [[nodiscard]] ::Image c_raylib() const & noexcept {
        return m_data;
    }

    static ::Image Text(const std::string& text, int fontSize,
            ::Color color = DefaultColor) {
        return ::ImageText(text.c_str(), fontSize, color);
    }

    static ::Image Text(const ::Font& font, const std::string& text, float fontSize, float spacing,
            ::Color tint = DefaultColor) {
        return ::ImageTextEx(font, text.c_str(), fontSize, spacing, tint);
    }

    /**
     * Get pixel data from screen buffer and return an Image (screenshot)
     */
    static Image LoadFromScreen() {
        return Image{::LoadImageFromScreen()};
    }

    /**
     * Generate image: plain color
     */
    static Image Color(int width, int height, ::Color color = DefaultColor) {
        return Image{::GenImageColor(width, height, color)};
    }

    /**
     * Generate image: linear gradient
     */
    static Image GradientLinear(int width, int height, int direction, ::Color start, ::Color end) {
        return Image{::GenImageGradientLinear(width, height, direction, start, end)};
    }

    /**
     * Generate image: radial gradient
     */
    static Image GradientRadial(int width, int height, float density,
            ::Color inner, ::Color outer) {
        return Image{::GenImageGradientRadial(width, height, density, inner, outer)};
    }

    /**
     * Generate image: checked
     */
    static Image Checked(int width, int height, int checksX, int checksY,
            ::Color col1 = {255, 255, 255, 255}, ::Color col2 = {0, 0, 0, 255}) {
        return Image{::GenImageChecked(width, height, checksX, checksY, col1, col2)};
    }

    /**
     * Generate image: white noise
     */
    static Image WhiteNoise(int width, int height, float factor) {
        return Image{::GenImageWhiteNoise(width, height, factor)};
    }

    /**
     * Generate image: cellular algorithm. Bigger tileSize means bigger cells
     */
    static Image Cellular(int width, int height, int tileSize) {
        return Image{::GenImageCellular(width, height, tileSize)};
    }

    /**
     * Load image from file into CPU memory (RAM)
     *
     * @throws raylib::RaylibException Thrown if the image failed to load from the file.
     *
     * @see ::LoadImage()
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        set(::LoadImage(fileName.c_str()));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Image from file: " + fileName.string()));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Load image from RAW file data.
     *
     * @throws raylib::RaylibException Thrown if the image failed to load from the file.
     *
     * @see ::LoadImageRaw()
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) LoadRaw(const std::filesystem::path& fileName, int _width, int _height, int _format, int headerSize) RAYLIB_CPP_THROWS {
        set(::LoadImageRaw(fileName.c_str(), _width, _height, _format, headerSize));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Image from file: " + fileName.string()));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Load image sequence from file (frames appended to image.data).
     *
     * @throws raylib::RaylibException Thrown if the image animation to load from the file.
     *
     * @see ::LoadImageAnim()
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) LoadAnim(const std::filesystem::path& fileName, int& frames) RAYLIB_CPP_THROWS {
        set(::LoadImageAnim(fileName.c_str(), &frames));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Image from file: " + fileName.string()));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Load image from memory buffer, fileType refers to extension: i.e. "png".
     *
     * @throws raylib::RaylibException Thrown if the image animation to load from the file.
     *
     * @see ::LoadImageFromMemory()
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) LoadFromMemory(
            const std::string& fileType,
            std::span<const unsigned char> fileData) RAYLIB_CPP_THROWS {
        set(::LoadImageFromMemory(fileType.c_str(), fileData.data(), static_cast<int>(fileData.size())));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Image data with file type: " + fileType));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Load an image from the given file.
     *
     * @throws raylib::RaylibException Thrown if the image animation to load from the file.
     *
     * @see ::LoadImageFromTexture()
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) LoadFromTexture(const ::Texture2D& texture) RAYLIB_CPP_THROWS {
        set(::LoadImageFromTexture(texture));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Image from texture."));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Unload image from CPU memory (RAM)
     */
    void Unload() noexcept {
        if (m_data.data != nullptr) {
            ::UnloadImage(m_data);
            m_data.data = nullptr;
        }
    }

    /**
     * Export image data to file, returns true on success
     *
     * @throws raylib::RaylibException Thrown if the image failed to load from the file.
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Export(const std::filesystem::path& fileName) const RAYLIB_CPP_THROWS {
        if (!::ExportImage(m_data, fileName.c_str())) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to export Image to file: " + fileName.string()));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Export image to memory buffer
     */
    RayArrayHolder<unsigned char> ExportToMemory(const std::string &fileType, int &fileSize) {
        auto* idata = ::ExportImageToMemory(m_data, fileType.c_str(), &fileSize);
        return RayArrayHolder<unsigned char>(idata, static_cast<size_t>(fileSize));
    }
    RayArrayHolder<unsigned char> ExportToMemory(const std::string &fileType) {
        int fileSize;
        auto* idata = ::ExportImageToMemory(m_data, fileType.c_str(), &fileSize);
        return RayArrayHolder<unsigned char>(idata, static_cast<size_t>(fileSize));
    }

    /**
     * Export image as code file defining an array of bytes, returns true on success
     *
     * @throws raylib::RaylibException Thrown if the image failed to load from the file.
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) ExportAsCode(const std::filesystem::path& fileName) const RAYLIB_CPP_THROWS {
        if (!::ExportImageAsCode(m_data, fileName.c_str())) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to export Image code to file: " + fileName.string()));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    CONST_GETTER(void*, Data, m_data.data)
    GETTERSETTER(int, Width, m_data.width)
    GETTERSETTER(int, Height, m_data.height)
    GETTERSETTER(int, Mipmaps, m_data.mipmaps)
    GETTERSETTER(int, Format, m_data.format)

    /**
     * Retrieve the width and height of the image.
     */
    [[nodiscard]] constexpr raylib::Vector2 GetSize() const noexcept {
        return raylib::Vector2{{.x = static_cast<float>(m_data.width), .y = static_cast<float>(m_data.height)}};
    }

    /**
     * Create an image duplicate (useful for transformations)
     */
    [[nodiscard]] Image Copy() const {
        return Image{::ImageCopy(m_data)};
    }

    /**
     * Create an image from another image piece
     */
    [[nodiscard]] Image FromImage(const ::Rectangle& rec) const {
        return Image{::ImageFromImage(m_data, rec)};
    }

    /**
     * Convert image data to desired format
     */
    Image& Format(int newFormat) {
        ::ImageFormat(&m_data, newFormat);
        return *this;
    }

    /**
     * Convert image to POT (power-of-two)
     */
    Image& ToPOT(::Color fillColor) {
        ::ImageToPOT(&m_data, fillColor);
        return *this;
    }

    /**
     * Crop an image to area defined by a rectangle
     */
    Image& Crop(::Rectangle crop) {
        ::ImageCrop(&m_data, crop);
        return *this;
    }

    /**
     * Crop image depending on alpha value
     */
    Image& AlphaCrop(float threshold) {
        ::ImageAlphaCrop(&m_data, threshold);
        return *this;
    }

    /**
     * Clear alpha channel to desired color
     */
    Image& AlphaClear(::Color color, float threshold) {
        ::ImageAlphaClear(&m_data, color, threshold);
        return *this;
    }

    /**
     * Apply alpha mask to image
     */
    Image& AlphaMask(const ::Image& alphaMask) {
        ::ImageAlphaMask(&m_data, alphaMask);
        return *this;
    }

    /**
     * Premultiply alpha channel
     */
    Image& AlphaPremultiply() {
        ::ImageAlphaPremultiply(&m_data);
        return *this;
    }

    /**
     * Crop an image to a new given width and height.
     */
    Image& Crop(int newWidth, int newHeight) {
        return Crop(0, 0, newWidth, newHeight);
    }

    /**
     * Crop an image to a new given width and height based on a vector.
     */
    Image& Crop(::Vector2 size) {
        return Crop(0, 0, static_cast<int>(size.x), static_cast<int>(size.y));
    }

    /**
     * Crop an image to area defined by a rectangle
     */
    Image& Crop(int offsetX, int offsetY, int newWidth, int newHeight) {
        ::Rectangle rect{
            static_cast<float>(offsetX),
            static_cast<float>(offsetY),
            static_cast<float>(newWidth),
            static_cast<float>(newHeight)
        };
        ::ImageCrop(&m_data, rect);
        return *this;
    }

    /**
     * Resize and image to new size
     */
    Image& Resize(int newWidth, int newHeight) {
        ::ImageResize(&m_data, newWidth, newHeight);
        return *this;
    }

    /**
     * Resize and image to new size using Nearest-Neighbor scaling algorithm
     */
    Image& ResizeNN(int newWidth, int newHeight) {
        ::ImageResizeNN(&m_data, newWidth, newHeight);
        return *this;
    }

    /**
     * Resize canvas and fill with color
     */
    Image& ResizeCanvas(int newWidth, int newHeight, int offsetX = 0, int offsetY = 0,
            ::Color color = DefaultColor) {
        ::ImageResizeCanvas(&m_data, newWidth, newHeight, offsetX, offsetY, color);
        return *this;
    }

    /**
     * Generate all mipmap levels for a provided image
     */
    Image& Mipmaps() {
        ::ImageMipmaps(&m_data);
        return *this;
    }

    /**
     * Dither image data to 16bpp or lower (Floyd-Steinberg dithering)
     */
    Image& Dither(int rBpp, int gBpp, int bBpp, int aBpp) {
        ::ImageDither(&m_data, rBpp, gBpp, bBpp, aBpp);
        return *this;
    }

    /**
     * Flip image vertically
     */
    Image& FlipVertical() {
        ::ImageFlipVertical(&m_data);
        return *this;
    }

    /**
     * Flip image horizontally
     */
    Image& FlipHorizontal() {
        ::ImageFlipHorizontal(&m_data);
        return *this;
    }

    /**
     * Rotate image by input angle in degrees (-359 to 359)
     */
    Image& Rotate(int degrees) {
        ::ImageRotate(&m_data, degrees);
        return *this;
    }

    /**
     * Rotate image clockwise 90deg
     */
    Image& RotateCW() {
        ::ImageRotateCW(&m_data);
        return *this;
    }

    /**
     * Rotate image counter-clockwise 90deg
     */
    Image& RotateCCW() {
        ::ImageRotateCCW(&m_data);
        return *this;
    }

    /**
     * Modify image color: tint
     */
    Image& ColorTint(::Color color = DefaultColor) {
        ::ImageColorTint(&m_data, color);
        return *this;
    }

    /**
     * Modify image color: invert
     */
    Image& ColorInvert() {
        ::ImageColorInvert(&m_data);
        return *this;
    }

    /**
     * Modify image color: grayscale
     */
    Image& ColorGrayscale() {
        ::ImageColorGrayscale(&m_data);
        return *this;
    }

    /**
     * Modify image color: contrast
     *
     * @param contrast Contrast values between -100 and 100
     */
    Image& ColorContrast(float contrast) {
        ::ImageColorContrast(&m_data, contrast);
        return *this;
    }

    /**
     * Modify image color: brightness
     *
     * @param brightness Brightness values between -255 and 255
     */
    Image& ColorBrightness(int brightness) {
        ::ImageColorBrightness(&m_data, brightness);
        return *this;
    }

    /**
     * Modify image color: replace color
     */
    Image& ColorReplace(::Color color, ::Color replace) {
        ::ImageColorReplace(&m_data, color, replace);
        return *this;
    }

    /**
     * Get image alpha border rectangle
     *
     * @param threshold Threshold is defined as a percentatge: 0.0f -> 1.0f
     */
    [[nodiscard]] raylib::Rectangle GetAlphaBorder(float threshold) const {
        return raylib::Rectangle{::GetImageAlphaBorder(m_data, threshold)};
    }

    /**
     * Get image pixel color at (x, y) position
     */
    [[nodiscard]] raylib::Color GetColor(int x = 0, int y = 0) const {
        return raylib::Color{::GetImageColor(m_data, x, y)};
    }

    /**
     * Get image pixel color at vector position
     */
    [[nodiscard]] raylib::Color GetColor(::Vector2 position) const {
        return raylib::Color{::GetImageColor(m_data, static_cast<int>(position.x), static_cast<int>(position.y))};
    }

    /**
     * Clear image background with given color
     */
    Image& ClearBackground(::Color color = DefaultClearBackgroundColor) {
        ::ImageClearBackground(&m_data, color);
        return *this;
    }

    /**
     * Draw pixel within an image
     */
    void DrawPixel(int posX, int posY, ::Color color = DefaultColor) {
        ::ImageDrawPixel(&m_data, posX, posY, color);
    }

    void DrawPixel(::Vector2 position, ::Color color = DefaultColor) {
        ::ImageDrawPixelV(&m_data, position, color);
    }

    void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY,
            ::Color color = DefaultColor) {
        ::ImageDrawLine(&m_data, startPosX, startPosY, endPosX, endPosY, color);
    }

    void DrawLine(::Vector2 start, ::Vector2 end, ::Color color = {255, 255, 255, 255}) {
        ::ImageDrawLineV(&m_data, start, end, color);
    }

    void DrawCircle(int centerX, int centerY, int radius,
            ::Color color = DefaultColor) {
        ::ImageDrawCircle(&m_data, centerX, centerY, radius, color);
    }

    void DrawCircle(::Vector2 center, int radius,
            ::Color color = DefaultColor) {
        ::ImageDrawCircleV(&m_data, center, radius, color);
    }

    void DrawRectangle(int posX, int posY, int _width, int _height,
            ::Color color = DefaultColor) {
        ::ImageDrawRectangle(&m_data, posX, posY, _width, _height, color);
    }

    void DrawRectangle(::Vector2 position, ::Vector2 size,
            ::Color color = DefaultColor) {
        ::ImageDrawRectangleV(&m_data, position, size, color);
    }

    void DrawRectangle(const ::Rectangle& rec, ::Color color = DefaultColor) {
        ::ImageDrawRectangleRec(&m_data, rec, color);
    }

    void DrawRectangleLines(const ::Rectangle& rec, int thick = DefaultDrawRectangleLinesThick,
            ::Color color = DefaultColor) {
        ::ImageDrawRectangleLines(&m_data, rec, thick, color);
    }

    void Draw(const ::Image& src, const ::Rectangle& srcRec, const ::Rectangle& dstRec,
            ::Color tint = DefaultColor) {
        ::ImageDraw(&m_data, src, srcRec, dstRec, tint);
    }
    void Draw(const raylib::Image& src, const ::Rectangle& srcRec, const ::Rectangle& dstRec,
              ::Color tint = DefaultColor) {
        Draw(src.c_raylib(), srcRec, dstRec, tint);
    }

    void DrawText(const char* text, ::Vector2 position, int fontSize,
            ::Color color = DefaultColor) {
        ::ImageDrawText(&m_data,
            text,
            static_cast<int>(position.x),
            static_cast<int>(position.y),
            fontSize,
            color);
    }

    void DrawText(const std::string& text, ::Vector2 position, int fontSize,
            ::Color color = DefaultColor) {
        ::ImageDrawText(&m_data,
            text.c_str(),
            static_cast<int>(position.x),
            static_cast<int>(position.y),
            fontSize,
            color);
    }

    void DrawText(const std::string& text, int x, int y, int fontSize,
            ::Color color = DefaultColor) {
        ::ImageDrawText(&m_data, text.c_str(), x, y, fontSize, color);
    }

    void DrawText(const char* text, int x, int y, int fontSize,
            ::Color color = DefaultColor) {
        ::ImageDrawText(&m_data, text, x, y, fontSize, color);
    }

    void DrawText(const ::Font& font, const std::string& text, ::Vector2 position,
            float fontSize, float spacing, ::Color tint = DefaultColor) {
        ::ImageDrawTextEx(&m_data, font, text.c_str(), position, fontSize, spacing, tint);
    }
    /*
    void DrawText(const raylib::Font& font, const std::string& text, ::Vector2 position,
                  float fontSize, float spacing, ::Color tint = {255, 255, 255, 255}) {
        DrawText(font.c_raylib(), text, position, fontSize, spacing, tint);
    }
    */

    void DrawText(const ::Font& font, const char* text, ::Vector2 position,
            float fontSize, float spacing, ::Color tint = DefaultColor) {
        ::ImageDrawTextEx(&m_data, font, text, position, fontSize, spacing, tint);
    }
    /*
    void DrawText(const raylib::Font& font, const char* text, ::Vector2 position,
                  float fontSize, float spacing, ::Color tint = DefaultColor) {
        DrawText(font.c_raylib(), text, position, fontSize, spacing, tint);
    }
    */

    /**
     * Load color data from image as a Color array (RGBA - 32bit)
     */
    [[nodiscard]] RayImageColors LoadColors() const {
        /// @NOTE: assume allocated size in LoadImageColors
        return {::LoadImageColors(m_data), static_cast<unsigned long>(m_data.width * m_data.height) * sizeof(::Color)};
    }
    /// @TODO: get RayImageColors as mdspan

    /**
     * Load colors palette from image as a Color array (RGBA - 32bit)
     */
    [[nodiscard]] RayImagePlatte LoadPalette(int maxPaletteSize, int &colorsCount) const {
        ::Color* colors = ::LoadImagePalette(m_data, maxPaletteSize, &colorsCount);
        return {colors, static_cast<size_t>(colorsCount)};
    }
    [[nodiscard]] RayImagePlatte LoadPalette(int maxPaletteSize) const {
        int colorsCount{0};
        ::Color* colors = ::LoadImagePalette(m_data, maxPaletteSize, &colorsCount);
        return {colors, static_cast<size_t>(colorsCount)};
    }

    /**
     * Unload color data loaded with LoadImageColors()
     */
    [[deprecated("Use LoadColors() for loading Colors and avoid unload Colors by hand")]]
    static void UnloadColors(::Color* colors) noexcept {
        ::UnloadImageColors(colors);
    }

    /**
     * Unload colors palette loaded with LoadImagePalette()
     */
    [[deprecated("Use LoadPalette() for loading Colors and avoid unload Palette by hand")]]
    static void UnloadPalette(::Color* colors) noexcept {
        ::UnloadImagePalette(colors);
    }

    /**
     * Load texture from image data.
     */
     /*
    [[deprecated("Use Texture.Load()")]]
    [[nodiscard]] raylib::Texture2D LoadTexture() const {
        return raylib::Texture2D{::LoadTextureFromImage(*this)};
    }
    */

    /**
     * Loads a texture from the image data.
     *
     * @see LoadTexture()
     */
     /*
     [[deprecated("Use LoadTexture()")]]
    explicit operator ::Texture2D() {
        return LoadTexture();
    }
    */

    /**
     * Get pixel data size in bytes for certain format
     */
    static int GetPixelDataSize(int width, int height, int format = PIXELFORMAT_UNCOMPRESSED_R32G32B32A32) noexcept {
        return ::GetPixelDataSize(width, height, format);
    }

    /**
     * Returns the pixel data size based on the current image.
     *
     * @return The pixel data size of the image.
     */
    [[nodiscard]] int GetPixelDataSize() const noexcept {
        return ::GetPixelDataSize(m_data.width, m_data.height, m_data.format);
    }

    /**
     * Retrieve whether or not the Image has been loaded.
     *
     * @return True or false depending on whether the Image has been loaded.
     */
    [[nodiscard]] bool IsReady() const noexcept {
        return ::IsImageReady(m_data);
    }

 protected:
    constexpr void set(const ::Image& image) noexcept {
        m_data.data = image.data;
        m_data.width = image.width;
        m_data.height = image.height;
        m_data.mipmaps = image.mipmaps;
        m_data.format = image.format;
    }

    ::Image m_data;
};

}  // namespace raylib

using RImage = raylib::Image;

#endif  // RAYLIB_CPP_INCLUDE_IMAGE_HPP_
