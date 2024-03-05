#include <tl/expected.hpp>

#include "raylib-cpp.hpp"
#include "raylib-assert.h"
#include <string>
#include <vector>
#include <filesystem>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

class Bunny {
public:
    Bunny() {
        position = ::GetMousePosition();
        speed.x = static_cast<float>(::GetRandomValue(-250, 250)) / 60.0f;
        speed.y = static_cast<float>(::GetRandomValue(-250, 250)) / 60.0f;
        color = raylib::Color(
                ::GetRandomValue(50, 240),
                ::GetRandomValue(80, 240),
                ::GetRandomValue(100, 240));
    }

    void Update(const raylib::Texture2D& texBunny) {
        position.x += speed.x;
        position.y += speed.y;

        if (((position.x + texBunny.GetWidth()/2) > ::GetScreenWidth()) ||
            ((position.x + texBunny.GetWidth()/2) < 0)) speed.x *= -1;
        if (((position.y + texBunny.GetHeight()/2) > ::GetScreenHeight()) ||
            ((position.y + texBunny.GetHeight()/2 - 40) < 0)) speed.y *= -1;
    }

    raylib::Vector2 position;
    raylib::Vector2 speed;
    raylib::Color color;
};

TEST_CASE( "image loading", "[textures]" ) {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int ScreenWidth = 800;
    constexpr int ScreenHeight = 450;

    raylib::Window window;
    REQUIRE(window.Init(ScreenWidth, ScreenHeight, "raylib [textures] example - image loading"));
    raylib::Texture texture;
    REQUIRE(texture.Load("resources/raylib_logo.png"));
    raylib::Color textColor = raylib::Color::LightGray();

    REQUIRE_FALSE(window.ShouldClose());
    // Update
    //----------------------------------------------------------------------------------
    // Update your variables here
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    {
        window.ClearBackground(raylib::Color::RayWhite());

        texture.Draw(ScreenWidth / 2 - texture.GetWidth() / 2, ScreenHeight / 2 - texture.GetHeight() / 2);

        textColor.DrawText("this IS a texture loaded from an image!", 300, 370, 10);
    }
    EndDrawing();
    //----------------------------------------------------------------------------------

    window.Close();
}


TEST_CASE( "bunnymark", "[textures]" ) {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int ScreenWidth = 800;
    constexpr int ScreenHeight = 450;
    constexpr int MaxBatchElements = 8192;

    raylib::Window window;
    REQUIRE(window.Init(ScreenWidth, ScreenHeight, "raylib [textures] example - bunnymark"));

    // Load bunny texture
    raylib::Texture2D texBunny;
    REQUIRE(texBunny.Load("resources/wabbit_alpha.png"));

    std::vector<Bunny> bunnies;
    // Create more bunnies
    for (int i = 0; i < 100; i++) {
        bunnies.emplace_back();
    }

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    REQUIRE_FALSE(window.ShouldClose());
    // Update
    //----------------------------------------------------------------------------------
    // Update bunnies
    for (Bunny& bunny: bunnies) {
        bunny.Update(texBunny);
    }
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    {
        window.ClearBackground(RAYWHITE);

        for (Bunny& bunny : bunnies) {
            // NOTE: When internal batch buffer limit is reached (MAX_BATCH_ELEMENTS),
            // a draw call is launched and buffer starts being filled again;
            // before issuing a draw call, updated vertex data from internal CPU buffer is send to GPU...
            // Process of sending data is costly and it could happen that GPU data has not been completely
            // processed for drawing while new data is tried to be sent (updating current in-use buffers)
            // it could generates a stall and consequently a frame drop, limiting the number of drawn bunnies
            texBunny.Draw(bunny.position, bunny.color);
        }

        DrawRectangle(0, 0, ScreenWidth, 40, BLACK);
        raylib::DrawText(TextFormat("bunnies: %i", bunnies.size()), 120, 10, 20, GREEN);
        raylib::DrawText(TextFormat("batched draw calls: %i", 1 + bunnies.size()/MaxBatchElements), 320, 10, 20, MAROON);

        DrawFPS(10, 10);
    }
    EndDrawing();
    //----------------------------------------------------------------------------------

    window.Close();
}