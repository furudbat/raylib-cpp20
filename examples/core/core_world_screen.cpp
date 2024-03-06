/*******************************************************************************************
*
*   raylib [core] example - World to screen
*
*   This example has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib-cpp.hpp"

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int ScreenWidth = 800;
    constexpr int ScreenHeight = 450;

    raylib::Window window(ScreenWidth, ScreenHeight, "raylib [core] example - 3d camera free");

    // Define the camera to look into our 3d world
    raylib::Camera camera(
        raylib::Vector3({.x = 10.0F, .y = 10.0F, .z = 10.0F}),
        raylib::Vector3(),
        raylib::Vector3({.x = 0.0F, .y = 1.0F, .z = 0.0F}),
        45.0F,
        CAMERA_PERSPECTIVE);

    raylib::Vector3 cubePosition;
    raylib::Vector2 cubeScreenPosition;

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {     // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        camera.Update(CAMERA_THIRD_PERSON);                // Update camera

        // Calculate cube screen space position (with a little offset to be in top)
        cubeScreenPosition = ::GetWorldToScreen(::Vector3{cubePosition.x, cubePosition.y + 2.5F, cubePosition.z}, camera);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        {
            raylib::DrawingGuard drawingGuard (RAYWHITE);

            {
                raylib::CameraDrawingGuard cameraDrawing (camera);
                DrawCube(cubePosition, 2.0F, 2.0F, 2.0F, RED);
                DrawCubeWires(cubePosition, 2.0F, 2.0F, 2.0F, MAROON);

                DrawGrid(10, 1.0F);
            }

            raylib::DrawText("Enemy: 100 / 100",
                             static_cast<int>(cubeScreenPosition.x -
                                              static_cast<float>(::MeasureText("Enemy: 100/100", 20) / 2.0F)),
                             static_cast<int>(cubeScreenPosition.y), 20,
                             BLACK);
            raylib::DrawText("Text is always on top of the cube",
                (ScreenWidth - ::MeasureText("Text is always on top of the cube", 20)) / 2,
                25, 20, GRAY);
        }
        //----------------------------------------------------------------------------------
    }

    return 0;
}
