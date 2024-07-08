/*******************************************************************************************
*
*   raylib [shapes] example - Draw raylib logo using basic shapes
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib-cpp.hpp"

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int ScreenWidth = 800;
    constexpr int ScreenHeight = 450;

    raylib::Window window(ScreenWidth, ScreenHeight, "raylib [shapes] example - raylib logo using shapes");
    raylib::Color foreground{{.r = 0, .g = 68, .b = 130, .a = 255}};
    raylib::Color background{RAYWHITE};

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {    // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        // Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        {
            raylib::DrawingGuard drawingGuard;

            window.ClearBackground(background);

            foreground.DrawRectangle(ScreenWidth/2 - 128, ScreenHeight/2 - 128, 256, 256);
            background.DrawRectangle(ScreenWidth/2 - 112, ScreenHeight/2 - 112, 224, 224);
            foreground.DrawText("raylib", ScreenWidth/2 - 44, ScreenHeight/2 + 24, 50);
            foreground.DrawText("cpp", ScreenWidth/2 - 74, ScreenHeight/2 + 54, 50);

            DrawText("this is NOT a texture!", 350, 370, 10, GRAY);
        }
        //----------------------------------------------------------------------------------
    }

    return 0;
}
