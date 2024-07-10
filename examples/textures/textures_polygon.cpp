/*******************************************************************************************
*
*   raylib [shapes] example - Draw Textured Polygon
*
*   Example originally created with raylib 3.7, last time updated with raylib 3.7
*
*   Example contributed by Chris Camacho (@codifies) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2021-2024 Chris Camacho (@codifies) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib-cpp.hpp"

#include <rlgl.h>           // Required for: Vertex definition
#include <raymath.h>

#include <span>

inline constexpr int MAX_POINTS = 11;      // 10 points and back to the start

// Draw textured polygon, defined by vertex and texture coordinates
void DrawTexturePoly(const raylib::Texture2D& texture, ::Vector2 center, std::span<::Vector2> points, std::span<::Vector2> texcoords, ::Color tint);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [textures] example - textured polygon");

    // Define texture coordinates to map our texture to poly
    std::array<::Vector2, MAX_POINTS> texcoords = {
            Vector2{ 0.75f, 0.0f },
            Vector2{ 0.25f, 0.0f },
            Vector2{ 0.0f, 0.5f },
            Vector2{ 0.0f, 0.75f },
            Vector2{ 0.25f, 1.0f},
            Vector2{ 0.375f, 0.875f},
            Vector2{ 0.625f, 0.875f},
            Vector2{ 0.75f, 1.0f},
            Vector2{ 1.0f, 0.75f},
            Vector2{ 1.0f, 0.5f},
            Vector2{ 0.75f, 0.0f}  // Close the poly
    };

    // Define the base poly vertices from the UV's
    // NOTE: They can be specified in any other way
    std::array<::Vector2, MAX_POINTS> points;
    for (size_t i = 0; i < points.size() && i < points.size(); i++)
    {
        points[i].x = (texcoords[i].x - 0.5f)*256.0f;
        points[i].y = (texcoords[i].y - 0.5f)*256.0f;
    }

    // Define the vertices drawing position
    // NOTE: Initially same as points but updated every frame
    std::array<::Vector2, MAX_POINTS> positions;
    for (size_t i = 0; i < positions.size() && i < points.size(); i++) positions[i] = points[i];

    // Load texture to be mapped to poly
    raylib::Texture texture ("resources/cat.png");

    float angle = 0.0f;             // Rotation angle (in degrees)

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Update points rotation with an angle transform
        // NOTE: Base points position are not modified
        angle++;
        for (size_t i = 0; i < positions.size() && i < points.size(); i++) positions[i] = Vector2Rotate(points[i], angle*DEG2RAD);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("textured polygon", 20, 20, 20, DARKGRAY);

        DrawTexturePoly(texture, Vector2{ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f },
                        positions, texcoords, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //UnloadTexture(texture); // Unload texture

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Draw textured polygon, defined by vertex and texture coordinates
// NOTE: Polygon center must have straight line path to all points
// without crossing perimeter, points must be in anticlockwise order
void DrawTexturePoly(const raylib::Texture2D& texture, ::Vector2 center, std::span<::Vector2> points, std::span<::Vector2> texcoords, ::Color tint)
{
    rlSetTexture(texture.GetId());

    // Texturing is only supported on RL_QUADS
    rlBegin(RL_QUADS);

    rlColor4ub(tint.r, tint.g, tint.b, tint.a);

    for (size_t i = 0; i < std::min(points.size() - 1, texcoords.size() - 1); i++)
    {
        rlTexCoord2f(0.5f, 0.5f);
        rlVertex2f(center.x, center.y);

        rlTexCoord2f(texcoords[i].x, texcoords[i].y);
        rlVertex2f(points[i].x + center.x, points[i].y + center.y);

        rlTexCoord2f(texcoords[i + 1].x, texcoords[i + 1].y);
        rlVertex2f(points[i + 1].x + center.x, points[i + 1].y + center.y);

        rlTexCoord2f(texcoords[i + 1].x, texcoords[i + 1].y);
        rlVertex2f(points[i + 1].x + center.x, points[i + 1].y + center.y);
    }
    rlEnd();

    rlSetTexture(0);
}