/*******************************************************************************************
 *
 *   raylib [core] example - window scale letterbox (and virtual mouse)
 *
 *   Example originally created with raylib 2.5, last time updated with raylib 4.0
 *
 *   Example contributed by Anata (@anatagawa) and reviewed by Ramon Santamaria (@raysan5)
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
 *   BSD-like license that allows static linking with closed source software
 *
 *   Copyright (c) 2019-2023 Anata (@anatagawa) and Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "raylib-cpp.hpp"
#include <array>

#include "raylib-cpp/raymath.hpp" // Required for: Vector2Clamp()

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main() {
    constexpr int WindowWidth = 800;
    constexpr int WindowHeight = 450;

    // Enable config flags for resizable window and vertical synchro
    raylib::Window window(
        WindowWidth,
        WindowHeight,
        "raylib [core] example - window scale letterbox",
        FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    window.SetMinSize(320, 240);

    int gameScreenWidth = 640;
    int gameScreenHeight = 480;

    // Render texture initialization, used to hold the rendering result so we can easily resize it
    raylib::RenderTexture2D target(gameScreenWidth, gameScreenHeight);
    target.GetTexture().SetFilter(raylib::TextureFilter::Bilinear); // Texture scale filter to use

    std::array<raylib::Color, 10> colors;
    // colors.fill(raylib::Color{0});
    for (auto& color : colors) {
        color = raylib::Color(
            {.r = static_cast<unsigned char>(GetRandomValue(100, 250)),
             .g = static_cast<unsigned char>(GetRandomValue(50, 150)),
             .b = static_cast<unsigned char>(GetRandomValue(10, 100)),
             .a = 255});
    }

    window.SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Compute required framebuffer scaling
        float scale = std::min<float>(
            static_cast<float>(GetScreenWidth()) / static_cast<float>(gameScreenWidth),
            static_cast<float>(GetScreenHeight()) / static_cast<float>(gameScreenHeight));

        if (IsKeyPressed(KEY_SPACE)) {
            // Recalculate random colors for the bars
            for (auto& color : colors)
                color = raylib::Color{
                    {.r = static_cast<unsigned char>(GetRandomValue(100, 250)),
                     .g = static_cast<unsigned char>(GetRandomValue(50, 150)),
                     .b = static_cast<unsigned char>(GetRandomValue(10, 100)),
                     .a = 255}};
        }

        // Update virtual mouse (clamped mouse value behind game screen)
        raylib::Vector2 mouse = raylib::Mouse::GetPosition();
        raylib::Vector2 virtualMouse(
            {{.x = (mouse.GetX() -
                    (static_cast<float>(GetScreenWidth()) - (static_cast<float>(gameScreenWidth) * scale)) * 0.5F) /
                   scale,
              .y = (mouse.GetY() -
                    (static_cast<float>(GetScreenHeight()) - (static_cast<float>(gameScreenHeight) * scale)) * 0.5F) /
                   scale}});
        virtualMouse = virtualMouse.Clamp(
            raylib::Vector2::Zero(),
            raylib::Vector2({{.x = static_cast<float>(gameScreenWidth), .y = static_cast<float>(gameScreenHeight)}}));

        // Apply the same transformation as the virtual mouse to the real mouse (i.e. to work with raygui)
        // SetMouseOffset(-(GetScreenWidth() - (gameScreenWidth*scale))*0.5f, -(GetScreenHeight() -
        // (gameScreenHeight*scale))*0.5f);
        //SetMouseScale(1/scale, 1/scale);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        // Draw everything in the render texture, note this will not be rendered on screen, yet
        {
            raylib::RenderTextureDrawingGuard textureDrawing(target);
            ClearBackground(RAYWHITE); // Clear render texture background color

            for (size_t i = 0; i < colors.size(); i++) {
                DrawRectangle(
                    0,
                    static_cast<int>((static_cast<size_t>(gameScreenHeight / 10)) * i),
                    gameScreenWidth,
                    gameScreenHeight / 10,
                    colors[i]);
            }

            DrawText(
                "If executed inside a window,\nyou can resize the window,\nand see the screen scaling!",
                10,
                25,
                20,
                WHITE);
            DrawText(
                ::TextFormat("Default Mouse: [%i , %i]", static_cast<int>(mouse.x), static_cast<int>(mouse.y)),
                350,
                25,
                20,
                GREEN);
            DrawText(
                ::TextFormat(
                    "Virtual Mouse: [%i , %i]",
                    static_cast<int>(virtualMouse.x),
                    static_cast<int>(virtualMouse.y)),
                350,
                55,
                20,
                YELLOW);
        }

        BeginDrawing();
        ClearBackground(BLACK); // Clear screen background

        // Draw render texture to screen, properly scaled
        target.GetTexture().Draw(
            raylib::Rectangle{
                {.x = 0.0f,
                 .y = 0.0f,
                 .width = static_cast<float>(target.GetTexture().GetWidth()),
                 .height = static_cast<float>(-target.GetTexture().GetHeight())}},
            raylib::Rectangle{{
                .x = (static_cast<float>(GetScreenWidth()) - (static_cast<float>(gameScreenWidth) * scale)) * 0.5f,
                .y = (static_cast<float>(GetScreenHeight()) - (static_cast<float>(gameScreenHeight) * scale)) * 0.5f,
                .width = static_cast<float>(gameScreenWidth) * scale,
                .height = static_cast<float>(gameScreenHeight) * scale,
            }},
            raylib::Vector2::Zero(),
            0.0f,
            WHITE);
        EndDrawing();
        //--------------------------------------------------------------------------------------
    }

    return 0;
}
