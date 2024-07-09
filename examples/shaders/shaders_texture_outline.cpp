/*******************************************************************************************
*
*   raylib [shaders] example - Apply an shdrOutline to a texture
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   Example originally created with raylib 4.0, last time updated with raylib 4.0
*
*   Example contributed by Samuel Skiff (@GoldenThumbs) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2021-2024 Samuel SKiff (@GoldenThumbs) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib-cpp.hpp"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [shaders] example - Apply an outline to a texture");

    raylib::Texture2D texture ("resources/fudesumi.png");

    raylib::Shader shdrOutline (LoadShader(0, TextFormat("resources/shaders/glsl%i/outline.fs", GLSL_VERSION)));

    float outlineSize = 2.0f;
    const std::array<float, 4> outlineColor = { 1.0f, 0.0f, 0.0f, 1.0f };     // Normalized RED color
    const std::array<float, 2> textureSize = { texture.GetWidthF(), texture.GetHeightF() };

    // Get shader locations
    const int outlineSizeLoc = shdrOutline.GetLocation("outlineSize");
    const int outlineColorLoc = shdrOutline.GetLocation("outlineColor");
    const int textureSizeLoc = shdrOutline.GetLocation("textureSize");

    // Set shader values (they can be changed later)
    shdrOutline.SetValue(outlineSizeLoc, outlineSize);
    shdrOutline.SetValue(outlineColorLoc, outlineColor);
    shdrOutline.SetValue(textureSizeLoc, textureSize);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        outlineSize += GetMouseWheelMove();
        if (outlineSize < 1.0f) outlineSize = 1.0f;

        shdrOutline.SetValue(outlineSizeLoc, outlineSize);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        shdrOutline.BeginMode();
        texture.Draw(GetScreenWidth()/2 - texture.GetWidth()/2, -30, WHITE);
        shdrOutline.EndMode();

        DrawText("Shader-based\ntexture\noutline", 10, 10, 20, GRAY);
        DrawText("Scroll mouse wheel to\nchange outline size", 10, 72, 20, GRAY);
        DrawText(TextFormat("Outline size: %i px", (int)outlineSize), 10, 120, 20, MAROON);

        DrawFPS(710, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //UnloadTexture(texture);
    //UnloadShader(shdrOutline);

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}