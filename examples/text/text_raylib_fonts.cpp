/*******************************************************************************************
*
*   raylib [text] example - raylib font loading and usage
*
*   NOTE: raylib is distributed with some free to use fonts (even for commercial pourposes!)
*         To view details and credits for those fonts, check raylib license file
*
*   This example has been created using raylib 1.7 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2017 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <vector>
#include <array>

#include "raylib-cpp.hpp"


int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int ScreenWidth = 800;
    constexpr int ScreenHeight = 450;

    raylib::Window window(ScreenWidth, ScreenHeight, "raylib [text] example - raylib fonts");
    const raylib::Color textColor {DARKGRAY};

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    constexpr size_t MaxFonts {8};
    const std::array<raylib::Font, MaxFonts> fonts = {
        raylib::Font("resources/fonts/alagard.png"),
        raylib::Font("resources/fonts/pixelplay.png"),
        raylib::Font("resources/fonts/mecha.png"),
        raylib::Font("resources/fonts/setback.png"),
        raylib::Font("resources/fonts/romulus.png"),
        raylib::Font("resources/fonts/pixantiqua.png"),
        raylib::Font("resources/fonts/alpha_beta.png"),
        raylib::Font("resources/fonts/jupiter_crash.png")
    };
    const std::array<std::string, MaxFonts> messages = {
        "ALAGARD FONT designed by Hewett Tsoi",
        "PIXELPLAY FONT designed by Aleksander Shevchuk",
        "MECHA FONT designed by Captain Falcon",
        "SETBACK FONT designed by Brian Kent (AEnigma)",
        "ROMULUS FONT designed by Hewett Tsoi",
        "PIXANTIQUA FONT designed by Gerhard Grossmann",
        "ALPHA_BETA FONT designed by Brian Kent (AEnigma)",
        "JUPITER_CRASH FONT designed by Brian Kent (AEnigma)"
    };
    const std::array<int, MaxFonts> spacings = { 2, 4, 8, 4, 3, 4, 4, 1 };

    const std::array<raylib::Vector2, MaxFonts> positions = [&]() {
        std::array<raylib::Vector2, MaxFonts> ret;
        for (size_t i = 0; i < fonts.size(); i++) {
            auto size = fonts[i].MeasureText(messages[i], fonts[i].GetBaseSize() * 2, spacings[i]);
            ret[i].SetX(ScreenWidth / 2 - size.GetX() / 2);
            ret[i].SetY(60 + fonts[i].GetBaseSize() + 45 * i);
        }

        // Small Y position corrections
        ret[3].y += 8;
        ret[4].y += 2;
        ret[7].y -= 8;

        return ret;
    }();

    constexpr std::array<raylib::Color, MaxFonts> colors = { raylib::Color{MAROON}, raylib::Color{ORANGE}, raylib::Color{DARKGREEN}, raylib::Color{DARKBLUE}, raylib::Color{DARKPURPLE}, raylib::Color{LIME}, raylib::Color{GOLD}, raylib::Color{RED} };
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {    // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        // Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);

            textColor.DrawText("free fonts included with raylib", 250, 20, 20);
            textColor.DrawLine(220, 50, 590, 50);

            for (size_t i = 0; i < fonts.size(); i++)
            {
                fonts[i].DrawText(messages[i], positions[i], fonts[i].GetBaseSize()*2, spacings[i], colors[i]);
            }
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}
