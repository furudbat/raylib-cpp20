/*******************************************************************************************
*
*   raylib [textures] example - Bunnymark
*
*   This example has been created using raylib 1.6 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014-2019 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <list>

#include "raylib-cpp.hpp"

// This is the maximum amount of elements (quads) per batch
// NOTE: This value is defined in [rlgl] module and can be changed there
#define MAX_BATCH_ELEMENTS  8192

class Bunny {
 public:
    Bunny() {
        position = GetMousePosition();
        speed.x = static_cast<float>(GetRandomValue(-250, 250)) / 60.0f;
        speed.y = static_cast<float>(GetRandomValue(-250, 250)) / 60.0f;
        color = raylib::Color(
                static_cast<uint8_t>(GetRandomValue(50, 240)),
                static_cast<uint8_t>(GetRandomValue(80, 240)),
                static_cast<uint8_t>(GetRandomValue(100, 240)));
    }

    void Update(const raylib::Texture2D& texBunny) {
        position.x += speed.x;
        position.y += speed.y;

        if (((position.x + static_cast<float>(texBunny.GetWidth()) / 2.0f) > static_cast<float>(::GetScreenWidth())) ||
            ((position.x + static_cast<float>(texBunny.GetWidth()) / 2.0f) < 0)) speed.x *= -1;
        if (((position.y + static_cast<float>(texBunny.GetHeight()) / 2.0f) > static_cast<float>(::GetScreenHeight())) ||
            ((position.y + static_cast<float>(texBunny.GetHeight()) / 2.0f - 40) < 0)) speed.y *= -1;
    }

    Vector2 position;
    Vector2 speed;
    Color color;
};

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    raylib::Window window(screenWidth, screenHeight, "raylib [textures] example - bunnymark");

    // Load bunny texture
    raylib::Texture2D texBunny("resources/wabbit_alpha.png");

    std::vector<Bunny> bunnies;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {    // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            // Create more bunnies
            bunnies.resize(bunnies.size()+100);
        }

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

            DrawRectangle(0, 0, screenWidth, 40, BLACK);
            raylib::DrawText(::TextFormat("bunnies: %i", bunnies.size()), 120, 10, 20, GREEN);
            raylib::DrawText(::TextFormat("batched draw calls: %i", 1 + bunnies.size()/MAX_BATCH_ELEMENTS), 320, 10, 20, MAROON);

            DrawFPS(10, 10);
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}
