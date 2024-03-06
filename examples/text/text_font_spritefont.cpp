/*******************************************************************************************
*
*   raylib [text] example - Font loading and usage
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <string>

#include "raylib-cpp.hpp"

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    raylib::Window window(screenWidth, screenHeight, "raylib [text] example - sprite fonts usage");

    const std::string msg1 = "THIS IS A custom SPRITE FONT...";
    const std::string msg2 = "...and this is ANOTHER CUSTOM font...";
    const std::string msg3 = "...and a THIRD one! GREAT! :D";

    // NOTE: Textures/Fonts MUST be loaded after Window initialization (OpenGL context is required)
    raylib::Font font1("resources/custom_mecha.png");          // Font loading
    raylib::Font font2("resources/custom_alagard.png");        // Font loading
    raylib::Font font3("resources/custom_jupiter_crash.png");  // Font loading

    raylib::Vector2 fontPosition1{{.x = screenWidth/2.0f - font1.MeasureTextWithBaseSize(msg1, -3).x/2.0f,
                              .y = screenHeight/2.0f - static_cast<float>(font1.GetBaseSize()) / 2.0f - 80}};

    raylib::Vector2 fontPosition2{{.x = screenWidth/2.0f - font2.MeasureTextWithBaseSize(msg2, -2).x/2.0f,
                              .y = screenHeight/2.0f - static_cast<float>(font2.GetBaseSize()) / 2.0f - 10}};

    raylib::Vector2 fontPosition3{{.x = screenWidth/2.0f - font3.MeasureTextWithBaseSize(msg3, 2).x/2.0f,
                              .y = screenHeight/2.0f - static_cast<float>(font3.GetBaseSize()) / 2.0f + 50}};

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {    // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update variables here...
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);

            font1.DrawTextWithBaseSize(msg1, fontPosition1, -3);
            font2.DrawTextWithBaseSize(msg2, fontPosition2, -2);
            font3.DrawTextWithBaseSize(msg3, fontPosition3, 2);
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}
