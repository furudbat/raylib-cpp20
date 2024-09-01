#include <tl/expected.hpp>

#include "raylib-assert.h"
#include "raylib-cpp.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <string>
#include <vector>

TEST_CASE("font loading", "[text]") {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int ScreenWidth = 800;
    constexpr int ScreenHeight = 450;

    raylib::Window window;
    REQUIRE(window.Init(ScreenWidth, ScreenHeight, "raylib [text] example - font loading"));

    // Define characters to draw
    // NOTE: raylib supports UTF-8 encoding, following list is actually codified as UTF8 internally
    const std::string msg = "!\"#$%&'()*+,-./"
                            "0123456789:;<=>?@ABCDEFGHI\nJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmn\nopqrstuvwxyz{|}~"
                            "¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓ\nÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷\nøùúûüýþÿ";

    // NOTE: Textures/Fonts MUST be loaded after Window initialization (OpenGL context is required)

    // BMFont (AngelCode) : Font data and image atlas have been generated using external program
    raylib::Font fontBm;
    REQUIRE(fontBm.Load("resources/pixantiqua.fnt"));

    // TTF font : Font data and atlas are generated directly from TTF
    // NOTE: We define a font base size of 32 pixels tall and up-to 250 characters
    raylib::Font fontTtf;
    REQUIRE(fontTtf.Load("resources/pixantiqua.ttf", 32, nullptr, 250));

    window.SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    REQUIRE_FALSE(window.ShouldClose());

    SECTION("Use BMFont") {
        // Draw
        //----------------------------------------------------------------------------------
        window.BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);

            raylib::DrawText("Hold SPACE to use TTF generated font", 20, 20, 20, LIGHTGRAY);

            fontBm.DrawText(msg, Vector2{20.0f, 100.0f}, static_cast<float>(fontBm.GetBaseSize()), 2, MAROON);
            raylib::DrawText("Using BMFont (Angelcode) imported", 20, GetScreenHeight() - 30, 20, GRAY);
        }
        window.EndDrawing();
        //----------------------------------------------------------------------------------
    }

    SECTION("Use TTF") {
        // Draw
        //----------------------------------------------------------------------------------
        window.BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);

            raylib::DrawText("Hold SPACE to use TTF generated font", 20, 20, 20, LIGHTGRAY);

            fontTtf.DrawText(msg, Vector2{20.0f, 100.0f}, static_cast<float>(fontTtf.GetBaseSize()), 2, LIME);
            raylib::DrawText("Using TTF font generated", 20, GetScreenHeight() - 30, 20, GRAY);
        }
        window.EndDrawing();
        //----------------------------------------------------------------------------------
    }

    window.Close();
}

TEST_CASE("font loading into vector", "[text]") {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int ScreenWidth = 800;
    constexpr int ScreenHeight = 450;

    raylib::Window window;
    REQUIRE(window.Init(ScreenWidth, ScreenHeight, "raylib [text] example - font loading"));

    // Define characters to draw
    // NOTE: raylib supports UTF-8 encoding, following list is actually codified as UTF8 internally
    const std::string msg = "!\"#$%&'()*+,-./"
                            "0123456789:;<=>?@ABCDEFGHI\nJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmn\nopqrstuvwxyz{|}~"
                            "¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓ\nÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷\nøùúûüýþÿ";

    // NOTE: Textures/Fonts MUST be loaded after Window initialization (OpenGL context is required)

    SECTION("load fonts") {
        auto fonts = []() -> std::vector<raylib::Font>
        {
            std::vector<raylib::Font> ret;
            // BMFont (AngelCode) : Font data and image atlas have been generated using external program
            raylib::Font fontBm;
            REQUIRE(fontBm.Load("resources/pixantiqua.fnt"));

            // TTF font : Font data and atlas are generated directly from TTF
            // NOTE: We define a font base size of 32 pixels tall and up-to 250 characters
            raylib::Font fontTtf;
            REQUIRE(fontTtf.Load("resources/pixantiqua.ttf", 32, nullptr, 250));

            ret.push_back(std::move(fontBm));
            ret.push_back(std::move(fontTtf));
            return ret;
        }();

        REQUIRE(fonts.size() == 2);
    }

    window.SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    REQUIRE_FALSE(window.ShouldClose());

    window.Close();
}