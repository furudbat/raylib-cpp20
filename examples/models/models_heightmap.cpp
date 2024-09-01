/*******************************************************************************************
 *
 *   raylib [models] example - Heightmap loading and drawing
 *
 *   Example originally created with raylib 1.8, last time updated with raylib 3.5
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
 *   BSD-like license that allows static linking with closed source software
 *
 *   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "raylib-cpp.hpp"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - heightmap loading and drawing");

    // Define our custom camera to look into our 3d world
    raylib::Camera camera;
    camera.position = Vector3{18.0f, 21.0f, 18.0f}; // Camera position
    camera.target = Vector3{0.0f, 0.0f, 0.0f}; // Camera looking at point
    camera.up = Vector3{0.0f, 1.0f, 0.0f}; // Camera up vector (rotation towards target)
    camera.fovy = 45.0f; // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE; // Camera projection type

    raylib::Image image(LoadImage("resources/heightmap.png")); // Load heightmap image (RAM)
    raylib::Texture2D texture = raylib::Texture2D::LoadFromImage(image); // Convert image to texture (VRAM)

    // Generate heightmap mesh (RAM and VRAM)
    raylib::Model model(raylib::Mesh::GenHeightmap(image, Vector3{16, 8, 16})); // Load model from generated mesh

    model.SetMaterialMapTexture(
        0,
        MATERIAL_MAP_DIFFUSE,
        texture,
        raylib::ModelMaterialTextureOption::NoUnload); // Set map diffuse texture
    Vector3 mapPosition = {-8.0f, 0.0f, -8.0f}; // Define model position

    //UnloadImage(image);             // Unload heightmap image from RAM, already uploaded to VRAM
    image.Unload();

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_ORBITAL);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        model.Draw(mapPosition, 1.0f, RED);

        DrawGrid(20, 1.0f);

        EndMode3D();

        texture.Draw(screenWidth - texture.GetWidth() - 20, 20, WHITE);
        DrawRectangleLines(screenWidth - texture.GetWidth() - 20, 20, texture.GetHeight(), texture.GetHeight(), GREEN);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //UnloadTexture(texture);     // Unload texture
    //UnloadModel(model);         // Unload model

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}