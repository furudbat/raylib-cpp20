/*******************************************************************************************
*
*   raylib [models] example - Waving cubes
*
*   Example originally created with raylib 2.5, last time updated with raylib 3.7
*
*   Example contributed by Codecat (@codecat) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 Codecat (@codecat) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib-cpp.hpp"

#include <cmath>       // Required for: sinf()

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - waving cubes");

    // Initialize the camera
    raylib::Camera3D camera;
    camera.position = Vector3{ 30.0f, 20.0f, 30.0f }; // Camera position
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 70.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    // Specify the amount of blocks in each direction
    constexpr int numBlocks = 15;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        const double time = GetTime();

        // Calculate time scale for cube position and size
        const float scale = static_cast<float>(2.0 + std::sin(time))*0.7f;

        // Move camera around the scene
        const double cameraTime = time*0.3;
        camera.position.x = static_cast<float>(std::cos(cameraTime)*40.0);
        camera.position.z = static_cast<float>(std::sin(cameraTime)*40.0);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawGrid(10, 5.0f);

        for (int x = 0; x < numBlocks; x++)
        {
            for (int y = 0; y < numBlocks; y++)
            {
                for (int z = 0; z < numBlocks; z++)
                {
                    // Scale of the blocks depends on x/y/z positions
                    const float blockScale = static_cast<float>(x + y + z)/30.0f;

                    // Scatter makes the waving effect by adding blockScale over time
                    const float scatter = sinf(blockScale*20.0f + (float)(time*4.0f));

                    // Calculate the cube position
                    const raylib::Vector3 cubePos = {
                            (float)(x - numBlocks/2)*(scale*3.0f) + scatter,
                            (float)(y - numBlocks/2)*(scale*2.0f) + scatter,
                            (float)(z - numBlocks/2)*(scale*3.0f) + scatter
                    };

                    // Pick a color with a hue depending on cube position for the rainbow color effect
                    // NOTE: This function is quite costly to be done per cube and frame,
                    // pre-catching the results into a separate array could improve performance
                    const auto cubeColor = raylib::Color::FromHSV((float)(((x + y + z)*18)%360), 0.75f, 0.9f);

                    // Calculate cube size
                    const float cubeSize = (2.4f - scale)*blockScale;

                    // And finally, draw the cube!
                    DrawCube(cubePos, cubeSize, cubeSize, cubeSize, cubeColor);
                }
            }
        }

        EndMode3D();

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}