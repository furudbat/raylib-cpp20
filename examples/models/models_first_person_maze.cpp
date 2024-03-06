/*******************************************************************************************
*
*   raylib [models] example - first person maze
*
*   This example has been created using raylib 2.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2019 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib-cpp.hpp"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    raylib::Window window(screenWidth, screenHeight, "raylib [models] example - first person maze");

    // Define the camera to look into our 3d world
    raylib::Camera camera({ 0.2f, 0.4f, 0.2f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f);

    raylib::Image imMap("resources/cubicmap.png");      // Load cubicmap image (RAM)
    raylib::Texture cubicmap(imMap);                    // Convert image to texture to display (VRAM)
    raylib::Model model(raylib::MeshUnmanaged::Cubicmap(imMap, Vector3{ 1.0f, 1.0f, 1.0f }));

    // NOTE: By default each cube is mapped to one part of texture atlas
    raylib::Texture texture("resources/cubicmap_atlas.png");    // Load map texture
    model.GetMaterials()[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture.c_raylib();     // Set map diffuse texture

    // Get map image data to be used for collision detection
    auto mapPixels = imMap.LoadColors();

    imMap.Unload();                   // Unload image from RAM

    raylib::Vector3 mapPosition({.x = -16.0F,.y =  0.0F,.z = -8.0F});   // Set model position
    raylib::Vector3 playerPosition(camera.position);    // Set player position

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {    // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        raylib::Vector3 oldCamPos(camera.position);    // Store old camera position

        camera.Update(CAMERA_FIRST_PERSON);        // Update camera

        // Check player collision (we simplify to 2D collision detection)
        raylib::Vector2 playerPos({.x = camera.position.x,.y = camera.position.z});
        float playerRadius = 0.1F;  // Collision radius (player is modelled as a cilinder for collision)

        int playerCellX = static_cast<int>(playerPos.x - static_cast<float>(std::lround(mapPosition.x + 0.5F)));
        int playerCellY = static_cast<int>(playerPos.y - static_cast<float>(std::lround(mapPosition.z + 0.5F)));

        // Out-of-limits security check
        if (playerCellX < 0) playerCellX = 0;
        else if (playerCellX >= cubicmap.GetWidth()) playerCellX = cubicmap.GetWidth() - 1;

        if (playerCellY < 0) playerCellY = 0;
        else if (playerCellY >= cubicmap.GetHeight()) playerCellY = cubicmap.GetHeight() - 1;

        // Check map collisions using image data and player position
        // TODO: Improvement: Just check player surrounding cells for collision
        for (int y = 0; y < cubicmap.GetHeight(); y++)
        {
            for (int x = 0; x < cubicmap.GetWidth(); x++)
            {
                std::span data = mapPixels.AsSpan();
                /// @TODO: check data for size
                if ((data[y*cubicmap.GetWidth() + x].r == 255) &&       // Collision: white pixel, only check R channel
                    (playerPos.CheckCollisionCircle(playerRadius,
                    Rectangle{ mapPosition.x - 0.5F + static_cast<float>(x)*1.0F, mapPosition.z - 0.5F + static_cast<float>(y)*1.0F, 1.0F, 1.0F })))
                {
                    // Collision detected, reset camera position
                    camera.position = oldCamPos;
                }
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);

            camera.BeginMode();
            {
                model.Draw(mapPosition);               // Draw maze map
                // playerPosition.DrawCube((Vector3){ 0.2f, 0.4f, 0.2f }, RED);  // Draw player
            }
            camera.EndMode();

            cubicmap.Draw(Vector2{ static_cast<float>(GetScreenWidth() - cubicmap.GetWidth()*4 - 20), 20 }, 0.0F, 4.0F, WHITE);
            ::DrawRectangleLines(GetScreenWidth() - cubicmap.GetWidth()*4 - 20, 20, cubicmap.GetWidth()*4, cubicmap.GetHeight()*4, GREEN);

            // Draw player position radar
            ::DrawRectangle(GetScreenWidth() - cubicmap.GetWidth()*4 - 20 + playerCellX*4, 20 + playerCellY*4, 4, 4, RED);

            ::DrawFPS(10, 10);
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    //UnloadImageColors(mapPixels);          // Unload color array

    //----------------------------------------------------------------------------------

    return 0;
}
