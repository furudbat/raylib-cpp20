/*******************************************************************************************
*
*   raylib [text] example - Font loading
*
*   raylib can load fonts from multiple file formats:
*
*     - TTF/OTF > Sprite font atlas is generated on loading, user can configure
*                 some of the generation parameters (size, characters to include)
*     - BMFonts > Angel code font fileformat, sprite font image must be provided
*                 together with the .fnt file, font generation cna not be configured
*     - XNA Spritefont > Sprite font image, following XNA Spritefont conventions,
*                 Characters in image must follow some spacing and order rules
*
*   This example has been created using raylib 2.6 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2016-2019 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <string>
#include "raylib-cpp.hpp"

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int ScreenWidth = 800;
    constexpr int ScreenHeight = 450;

    raylib::Window window(ScreenWidth, ScreenHeight, "raylib [text] example - font loading");

    // Define characters to draw
    // NOTE: raylib supports UTF-8 encoding, following list is actually codified as UTF8 internally
    const std::string msg = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHI\nJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmn\nopqrstuvwxyz{|}~¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓ\nÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷\nøùúûüýþÿ";

    // NOTE: Textures/Fonts MUST be loaded after Window initialization (OpenGL context is required)

    // BMFont (AngelCode) : Font data and image atlas have been generated using external program
    raylib::Font fontBm("resources/pixantiqua.fnt");

    // TTF font : Font data and atlas are generated directly from TTF
    // NOTE: We define a font base size of 32 pixels tall and up-to 250 characters
    raylib::Font fontTtf("resources/pixantiqua.ttf", 32, nullptr, 250);

    ::SetTextLineSpacing(32);

    bool useTtf {false};

    window.SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {    // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        useTtf = IsKeyDown(KEY_SPACE);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        window.BeginDrawing();
        {

            window.ClearBackground(RAYWHITE);

            raylib::DrawText("Hold SPACE to use TTF generated font", 20, 20, 20, LIGHTGRAY);

            if (!useTtf)
            {
                fontBm.DrawTextWithBaseSize(msg, ::Vector2{ 20.0F, 100.0F }, 2, MAROON);
                raylib::DrawText("Using BMFont (Angelcode) imported", 20, ::GetScreenHeight() - 30, 20, GRAY);
            }
            else
            {
                fontTtf.DrawTextWithBaseSize(msg, ::Vector2{ 20.0F, 100.0F }, 2, LIME);
                raylib::DrawText("Using TTF font generated", 20, ::GetScreenHeight() - 30, 20, GRAY);
            }
        }
        window.EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}
