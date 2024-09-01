#ifndef RAYLIB_CPP_INCLUDE_WINDOW_HPP_
#define RAYLIB_CPP_INCLUDE_WINDOW_HPP_


#include "raylib.hpp"

#include "RaylibError.hpp"
#include "Vector2.hpp"
#ifdef __cpp_exceptions
#include "RaylibException.hpp"
#endif

#include <chrono>
#include <cstdint>
#include <string>

namespace raylib {

enum class WindowFullscreenOption : bool {
    Fullscreen = true,
    Windowed = false
};

/**
 * Window and Graphics Device Functions.
 */
class Window {
public:
    /**
     * Build a Window object, but defer the initialization. Ensure you call Init() manually.
     *
     * @see Init()
     */
    Window() = default;

    /**
     * Initialize window and OpenGL context.
     *
     * @param width The width of the window.
     * @param height The height of the window.
     * @param title The desired title of the window.
     * @param flags The ConfigFlags to set prior to initializing the window. See SetConfigFlags for more details.
     *
     * @see ::SetConfigFlags()
     * @see ConfigFlags
     *
     * @throws raylib::RaylibException Thrown if the window failed to initiate.
     */
    static constexpr czstring DefaultTitle = "raylib";
    Window(int width, int height, czstring title = DefaultTitle, uint32_t flags = 0) {
        Init(width, height, title, flags);
    }
    Window(int width, int height, const std::string& title, uint32_t flags = 0) { Init(width, height, title, flags); }

    /**
     * Close window and unload OpenGL context
     */
    ~Window() { Close(); }

    /**
     * Initializes the window.
     *
     * @param width The width of the window.
     * @param height The height of the window.
     * @param title The desired title of the window.
     * @param flags The ConfigFlags to set prior to initializing the window. See SetConfigFlags for more details.
     *
     * @see ::SetConfigFlags()
     * @see ConfigFlags
     *
     * @throws raylib::RaylibException Thrown if the window failed to initiate.
     */
    static constexpr int DefaultWidth = 800;
    static constexpr int DefaultHeight = 450;
    static RAYLIB_CPP_EXPECTED_RESULT_VOID
    Init(int width = DefaultWidth, int height = DefaultHeight, czstring title = DefaultTitle, uint32_t flags = 0) {
        if (flags != 0) {
            ::SetConfigFlags(flags);
        }
        ::InitWindow(width, height, title);
        if (!::IsWindowReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to create Window"));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }
    static RAYLIB_CPP_EXPECTED_RESULT_VOID Init(int width, int height, const std::string& title, uint32_t flags = 0) {
        RAYLIB_CPP_RETURN_EXPECTED_VOID_VALUE(Init(width, height, title.c_str(), flags));
    }

    /**
     * Check if KEY_ESCAPE pressed or Close icon pressed
     */
    static bool ShouldClose() { return ::WindowShouldClose(); }

    /**
     * Set a custom key to exit program (default is ESC)
     */
    static void SetExitKey(int key) { ::SetExitKey(key); }

    /**
     * Close window and unload OpenGL context
     */
    static void Close() {
        if (::IsWindowReady()) {
            ::CloseWindow();
        }
    }

    /**
     * Check if cursor is on the current screen
     */
    [[nodiscard]] static bool IsCursorOnScreen() { return ::IsCursorOnScreen(); }

    /**
     * Check if window is currently fullscreen
     */
    [[nodiscard]] static bool IsFullscreen() { return ::IsWindowFullscreen(); }

    /**
     * Check if window is currently hidden
     */
    [[nodiscard]] static bool IsHidden() { return ::IsWindowHidden(); }

    /**
     * Check if window is currently minimized
     */
    [[nodiscard]] static bool IsMinimized() { return ::IsWindowMinimized(); }

    /**
     * Check if window is currently minimized
     */
    [[nodiscard]] static bool IsMaximized() { return ::IsWindowMaximized(); }

    /**
     * Check if window is currently focused
     */
    [[nodiscard]] static bool IsFocused() { return ::IsWindowFocused(); }

    /**
     * Check if window has been resized last frame
     */
    [[nodiscard]] static bool IsResized() { return ::IsWindowResized(); }

    /**
     * Check if one specific window flag is enabled
     */
    [[nodiscard]] static bool IsState(uint32_t flag) { return ::IsWindowState(flag); }

    /**
     * Set window configuration state using flags
     */
    Window& SetState(uint32_t flag) {
        ::SetWindowState(flag);
        return *this;
    }

    /**
     * Clear window configuration state flags
     */
    Window& ClearState(uint32_t flag) {
        ::ClearWindowState(flag);
        return *this;
    }

    /**
     * Clear window with given color.
     */
    Window& ClearBackground(::Color color = BLACK) {
        ::ClearBackground(color);
        return *this;
    }

    /**
     * Toggle window state: fullscreen/windowed
     */
    Window& ToggleFullscreen() {
        ::ToggleFullscreen();
        return *this;
    }

    /**
     * Set whether or not the application should be fullscreen.
     */
    Window& SetFullscreen(bool fullscreen) {
        if (fullscreen) {
            if (!IsFullscreen()) {
                ToggleFullscreen();
            }
        }
        else {
            if (IsFullscreen()) {
                ToggleFullscreen();
            }
        }

        return *this;
    }

    Window& SetFullscreen(WindowFullscreenOption fullscreen) {
        switch (fullscreen) {
            case WindowFullscreenOption::Fullscreen:
                if (!IsFullscreen()) {
                    ToggleFullscreen();
                }
                break;
            case WindowFullscreenOption::Windowed:
                if (IsFullscreen()) {
                    ToggleFullscreen();
                }
                break;
        }

        return *this;
    }

    /**
     * Toggle window state: borderless/windowed
     */
    Window& ToggleBorderless() {
        ::ToggleBorderlessWindowed();
        return *this;
    }

    /**
     * Set window state: maximized, if resizable (only PLATFORM_DESKTOP)
     */
    Window& Maximize() {
        ::MaximizeWindow();
        return *this;
    }

    /**
     * Set window state: minimized, if resizable (only PLATFORM_DESKTOP)
     */
    Window& Minimize() {
        ::MinimizeWindow();
        return *this;
    }

    /**
     * Set window state: not minimized/maximized (only PLATFORM_DESKTOP)
     */
    Window& Restore() {
        ::RestoreWindow();
        return *this;
    }

    /**
     * Set icon for window
     */
    Window& SetIcon(const ::Image& image) {
        ::SetWindowIcon(image);
        return *this;
    }

    /**
     * Set icon for window (multiple images, RGBA 32bit, only PLATFORM_DESKTOP)
     */
    [[deprecated("Use SetIcons(span)")]]
    Window& SetIcons(::Image* images, int count) {
        ::SetWindowIcons(images, count);
        return *this;
    }
    Window& SetIcons(std::span<::Image> images) {
        assert(images.size() <= std::numeric_limits<int>::max());
        ::SetWindowIcons(images.data(), static_cast<int>(images.size()));
        return *this;
    }

    /**
     * Set title for window
     */
    Window& SetTitle(czstring title) {
        ::SetWindowTitle(title);
        return *this;
    }
    Window& SetTitle(const std::string& title) {
        ::SetWindowTitle(title.c_str());
        return *this;
    }

    /**
     * Set window position on screen
     */
    Window& SetPosition(int x, int y) {
        ::SetWindowPosition(x, y);
        return *this;
    }

    /**
     * Set window position on screen
     */
    Window& SetPosition(::Vector2 position) {
        return SetPosition(static_cast<int>(position.x), static_cast<int>(position.y));
    }

    /**
     * Set monitor for the current window
     */
    Window& SetMonitor(int monitor) {
        ::SetWindowMonitor(monitor);
        return *this;
    }

    /**
     * Set window minimum dimensions
     */
    Window& SetMinSize(int width, int height) {
        ::SetWindowMinSize(width, height);
        return *this;
    }

    /**
     * Set window minimum dimensions
     */
    Window& SetMinSize(::Vector2 size) {
        ::SetWindowMinSize(static_cast<int>(size.x), static_cast<int>(size.y));
        return *this;
    }

    /**
     * Set window dimensions
     */
    Window& SetSize(int width, int height) {
        ::SetWindowSize(width, height);
        return *this;
    }

    /**
     * Set window opacity [0.0f..1.0f] (only PLATFORM_DESKTOP)
     */
    Window& SetOpacity(float opacity) {
        ::SetWindowOpacity(opacity);
        return *this;
    }

    /**
     * Set window focused (only PLATFORM_DESKTOP)
     */
    Window& SetFocused() {
        ::SetWindowFocused();
        return *this;
    }

    /**
     * Set window dimensions
     */
    Window& SetSize(::Vector2 size) { return SetSize(static_cast<int>(size.x), static_cast<int>(size.y)); }

    /**
     * Get the screen's width and height.
     */
    [[nodiscard]] Vector2 GetSize() {
        return Vector2{{.x = static_cast<float>(GetWidth()), .y = static_cast<float>(GetHeight())}};
    }

    /**
     * Get native window handle
     */
    [[nodiscard]] static void* GetHandle() { return ::GetWindowHandle(); }

    /**
     * Setup canvas (framebuffer) to start drawing
     */
    Window& BeginDrawing() {
        ::BeginDrawing();
        return *this;
    }

    /**
     * End canvas drawing and swap buffers (double buffering)
     */
    Window& EndDrawing() {
        ::EndDrawing();
        return *this;
    }

    /**
     * Get current screen width
     */
    [[nodiscard]] static int GetWidth() { return ::GetScreenWidth(); }

    /**
     * Get current screen height
     */
    [[nodiscard]] static int GetHeight() { return ::GetScreenHeight(); }

    /**
     * Get current render width (it considers HiDPI)
     */
    [[nodiscard]] static int GetRenderWidth() { return ::GetRenderWidth(); }

    /**
     * Get current render height (it considers HiDPI)
     */
    [[nodiscard]] static int GetRenderHeight() { return ::GetRenderHeight(); }

    /**
     * Get window position XY on monitor
     */
    [[nodiscard]] static ::Vector2 GetPosition() { return ::GetWindowPosition(); }

    /**
     * Get window scale DPI factor
     */
    [[nodiscard]] static ::Vector2 GetScaleDPI() { return ::GetWindowScaleDPI(); }

    /**
     * Set clipboard text content
     */
    void SetClipboardText(czstring text) { ::SetClipboardText(text); }
    void SetClipboardText(const std::string& text) { ::SetClipboardText(text.c_str()); }

    /**
     * Get clipboard text content
     */
    [[nodiscard]] static std::string GetClipboardText() { return ::GetClipboardText(); }

    /**
     * Set target FPS (maximum)
     */
    Window& SetTargetFPS(int fps) {
        ::SetTargetFPS(fps);
        return *this;
    }

    /**
     * Returns current FPS
     */
    [[nodiscard]] static int GetFPS() { return ::GetFPS(); }

    /**
     * Draw current FPS
     */
    static constexpr int DefaultDrawFPSPosX = 10;
    static constexpr int DefaultDrawFPSPosY = 10;
    void DrawFPS(int posX = DefaultDrawFPSPosX, int posY = DefaultDrawFPSPosY) const { ::DrawFPS(posX, posY); }

    /**
     * Returns time in seconds for last frame drawn
     */
    [[nodiscard]] static float GetFrameTime() { return ::GetFrameTime(); }
    [[nodiscard]] static std::chrono::milliseconds GetFrameTimeMs() {
        return std::chrono::milliseconds{static_cast<int64_t>(::GetFrameTime() * 1000)};
    }

    /**
     * Returns elapsed time in seconds since InitWindow()
     */
    [[nodiscard]] static double GetTime() { return ::GetTime(); }
    [[nodiscard]] static std::chrono::milliseconds GetTimeMs() {
        return std::chrono::milliseconds{static_cast<int64_t>(::GetTime() * 1000)};
    }

    /**
     * Check if window has been initialized successfully
     */
    [[nodiscard]] static bool IsReady() { return ::IsWindowReady(); }

    /**
     * Sets the configuration flags for raylib.
     *
     * @param flags The ConfigFlags to apply to the configuration.
     *
     * @see ::SetConfigFlags
     */
    static void SetConfigFlags(uint32_t flags) { ::SetConfigFlags(flags); }
};
} // namespace raylib

using RWindow = raylib::Window;

#endif // RAYLIB_CPP_INCLUDE_WINDOW_HPP_
