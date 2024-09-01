#include <tl/expected.hpp>

#include "raylib-assert.h"
#include "raylib-cpp.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <string>
#include <vector>

TEST_CASE("basic window", "[core]") {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int ScreenWidth = 800;
    constexpr int ScreenHeight = 450;
    raylib::Color textColor = raylib::Color::LightGray();

    raylib::Window window;
    REQUIRE(window.Init(ScreenWidth, ScreenHeight, "raylib [core] example - basic window"));

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    REQUIRE_FALSE(window.ShouldClose());
    // Update
    //----------------------------------------------------------------------------------
    // Update your variables here
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    {
        window.ClearBackground(RAYWHITE);
        textColor.DrawText("Congrats! You created your first window!", 190, 200, 20);
    }
    EndDrawing();
    //----------------------------------------------------------------------------------

    window.Close();
}