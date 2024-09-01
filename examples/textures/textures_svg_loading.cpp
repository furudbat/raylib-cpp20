/*******************************************************************************************
 *
 *   raylib [textures] example - SVG loading and texture creation
 *
 *   NOTE: Images are loaded in CPU memory (RAM); textures are loaded in GPU memory (VRAM)
 *
 *   Example originally created with raylib 4.2, last time updated with raylib 4.2
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
 *   BSD-like license that allows static linking with closed source software
 *
 *   Copyright (c) 2022 Dennis Meinen (@bixxy#4258 on Discord)
 *
 ********************************************************************************************/

#include "raylib-cpp.hpp"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [textures] example - svg loading");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    raylib::Texture2D texture = []()
    {
        raylib::Image image(LoadImageSvg("resources/test.svg", 400, 350)); // Loaded in CPU memory (RAM)
        return raylib::Texture2D(image); // Image converted to texture, GPU memory (VRAM)
        // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM
    }();

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        texture.Draw(screenWidth / 2 - texture.GetWidth() / 2, screenHeight / 2 - texture.GetHeight() / 2, WHITE);

        // Red border to illustrate how the SVG is centered within the specified dimensions
        DrawRectangleLines(
            (screenWidth / 2 - texture.GetWidth() / 2) - 1,
            (screenHeight / 2 - texture.GetHeight() / 2) - 1,
            texture.GetWidth() + 2,
            texture.GetHeight() + 2,
            RED);

        DrawText("this IS a texture loaded from an SVG file!", 300, 410, 10, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //UnloadTexture(texture);       // Texture unloading

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}