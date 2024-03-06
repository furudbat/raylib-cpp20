/*******************************************************************************************
*
*   raylib [shapes] example - collision area
*
*   This example has been created using raylib 2.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2019 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib-cpp.hpp"

#include <cmath> // NOLINT

int main()
{
    // Initialization
    //---------------------------------------------------------
    constexpr int ScreenWidth = 800;
    constexpr int ScreenHeight = 450;

    raylib::Window window(ScreenWidth, ScreenHeight, "raylib [shapes] example - collision area");

    // Box A: Moving box
    raylib::Rectangle boxA{{.x = 10, .y = static_cast<float>(::GetScreenHeight())/2.0F - 50.0F, .width = 200, .height = 100}};
    int boxASpeedX = 4;

    // Box B: Mouse moved box
    raylib::Rectangle boxB{{.x = static_cast<float>(::GetScreenWidth()) / 2.0F - 30.0F, .y = static_cast<float>(::GetScreenHeight()) / 2.0F - 30.0F, .width = 60, .height = 60}};

    raylib::Rectangle boxCollision; // Collision rectangle

    constexpr int ScreenUpperLimit {40};      // Top menu limits

    bool pause {false};             // Movement pause
    bool collision {false};         // Collision detection

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //----------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {    // Detect window close button or ESC key
        // Update
        //-----------------------------------------------------
        // Move box if not paused
        if (!pause) boxA.x += static_cast<float>(boxASpeedX);

        // Bounce box on x screen limits
        if (((boxA.x + boxA.width) >= static_cast<float>(::GetScreenWidth())) || (boxA.x <= 0)) boxASpeedX *= -1;

        // Update player-controlled-box (box02)
        boxB.x = static_cast<float>(::GetMouseX()) - boxB.width / 2;
        boxB.y = static_cast<float>(::GetMouseY()) - boxB.height / 2;

        // Make sure Box B does not go out of move area limits
        if ((boxB.x + boxB.width) >= static_cast<float>(::GetScreenWidth())) boxB.x = static_cast<float>(::GetScreenWidth()) - boxB.width;
        else if (boxB.x <= 0) boxB.x = 0;

        if ((boxB.y + boxB.height) >= static_cast<float>(::GetScreenHeight())) boxB.y = static_cast<float>(::GetScreenHeight()) - boxB.height;
        else if (boxB.y <= ScreenUpperLimit) boxB.y = ScreenUpperLimit;

        // Check boxes collision
        collision = boxA.CheckCollision(boxB);

        // Get collision rectangle (only on collision)
        if (collision) boxCollision = boxA.GetCollision(boxB);

        // Pause Box A movement
        if (IsKeyPressed(KEY_SPACE)) pause = !pause;
        //-----------------------------------------------------

        // Draw
        //-----------------------------------------------------
        BeginDrawing();

            window.ClearBackground(RAYWHITE);

            DrawRectangle(0, 0, ScreenWidth, ScreenUpperLimit, collision ? RED : BLACK);

            boxA.Draw(GOLD);
            boxB.Draw(BLUE);

            if (collision) {
                // Draw collision area
                boxCollision.Draw(LIME);

                // Draw collision message
                raylib::DrawText("COLLISION!", ::GetScreenWidth()/2 - ::MeasureText("COLLISION!", 20)/2, ScreenUpperLimit/2 - 10, 20, BLACK);

                // Draw collision area
                raylib::DrawText(::TextFormat("Collision Area: %i", static_cast<int>(boxCollision.width * boxCollision.height)),
                                 ::GetScreenWidth() / 2 - 100, ScreenUpperLimit + 10, 20, BLACK);
            }

            DrawFPS(10, 10);

        EndDrawing();
        //-----------------------------------------------------
    }

    return 0;
}
