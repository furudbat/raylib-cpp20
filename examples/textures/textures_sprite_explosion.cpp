/*******************************************************************************************
*
*   raylib [textures] example - sprite explosion
*
*   Example originally created with raylib 2.5, last time updated with raylib 3.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 Anata and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib-cpp.hpp"

inline constexpr size_t NUM_FRAMES_PER_LINE = 5;
inline constexpr size_t NUM_LINES           = 5;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [textures] example - sprite explosion");

    InitAudioDevice();

    // Load explosion sound
    raylib::Sound fxBoom ("resources/boom.wav");

    // Load explosion texture
    raylib::Texture2D explosion ("resources/explosion.png");

    // Init variables for animation
    const float frameWidth = explosion.GetWidthF()/NUM_FRAMES_PER_LINE;   // Sprite one frame rectangle width
    const float frameHeight = explosion.GetHeightF()/NUM_LINES;           // Sprite one frame rectangle height
    int currentFrame = 0;
    int currentLine = 0;

    Rectangle frameRec = { 0, 0, frameWidth, frameHeight };
    Vector2 position = { 0.0f, 0.0f };

    bool active = false;
    int framesCounter = 0;

    SetTargetFPS(120);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        // Check for mouse button pressed and activate explosion (if not active)
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !active)
        {
            position = GetMousePosition();
            active = true;

            position.x -= frameWidth/2.0f;
            position.y -= frameHeight/2.0f;

            fxBoom.Play();
        }

        // Compute explosion animation frames
        if (active)
        {
            framesCounter++;

            if (framesCounter > 2)
            {
                currentFrame++;

                if (std::cmp_greater_equal(currentFrame, NUM_FRAMES_PER_LINE))
                {
                    currentFrame = 0;
                    currentLine++;

                    if (std::cmp_greater_equal(currentLine, NUM_LINES))
                    {
                        currentLine = 0;
                        active = false;
                    }
                }

                framesCounter = 0;
            }
        }

        frameRec.x = frameWidth*currentFrame;
        frameRec.y = frameHeight*currentLine;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw explosion required frame rectangle
        if (active) explosion.Draw(frameRec, position, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //UnloadTexture(explosion);   // Unload texture
    //UnloadSound(fxBoom);        // Unload sound

    CloseAudioDevice();

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}