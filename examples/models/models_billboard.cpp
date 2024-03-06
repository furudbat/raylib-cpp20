/*******************************************************************************************
*
*   raylib [models] example - Drawing billboards
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
    const int screenWidth = 800;
    const int screenHeight = 450;

    raylib::Window window(screenWidth, screenHeight, "raylib [models] example - drawing billboards");

    // Define the camera to look into our 3d world
    raylib::Camera camera(
        raylib::Vector3({.x = 5.0F, .y = 4.0F, .z = 5.0F}),
        raylib::Vector3({.x = 0.0F, .y = 2.0F, .z = 0.0F}),
        raylib::Vector3({.x = 0.0F, .y = 1.0F, .z = 0.0F}),
        45.0F,
        CAMERA_PERSPECTIVE);

    raylib::Texture2D bill("resources/billboard.png");     // Our texture billboard
    raylib::Vector3 billPosition{{.x = 0.0F, .y = 2.0F, .z = 0.0F}};                // Position where draw billboard

    SetTargetFPS(60);                       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {    // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        camera.Update(CAMERA_ORBITAL);              // Update camera
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        {
            raylib::DrawingGuard drawingGuard (RAYWHITE);

            {
                raylib::CameraDrawingGuard cameraDrawing (camera);

                ::DrawGrid(10, 1.0F);        // Draw a grid
                bill.DrawBillboard(camera, billPosition, 2.0F);
            }

            DrawFPS(10, 10);
        }
        //----------------------------------------------------------------------------------
    }

    return 0;
}
