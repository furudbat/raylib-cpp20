/*******************************************************************************************
*
*   raylib [shaders] example - Postprocessing with custom uniform variable
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3), to test this example
*         on OpenGL ES 2.0 platforms (Android, Raspberry Pi, HTML5), use #version 100 shaders
*         raylib comes with shaders ready for both versions, check raylib/shaders install folder
*
*   Example originally created with raylib 1.3, last time updated with raylib 4.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib-cpp.hpp"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);      // Enable Multi Sampling Anti Aliasing 4x (if available)

    InitWindow(screenWidth, screenHeight, "raylib [shaders] example - custom uniform variable");

    // Define the camera to look into our 3d world
    raylib::Camera camera;
    camera.position = (Vector3){ 8.0f, 8.0f, 8.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 1.5f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    raylib::Model model (LoadModel("resources/models/barracks.obj"));                   // Load OBJ model
    raylib::Texture2D texture (LoadTexture("resources/models/barracks_diffuse.png"));   // Load model texture (diffuse map)
    model.SetMaterialMapTexture(0, MATERIAL_MAP_DIFFUSE, texture, raylib::ModelMaterialTextureOption::NoUnload);                     // Set model diffuse texture

    raylib::Vector3 position;                                    // Set model position

    // Load postprocessing shader
    // NOTE: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
    raylib::Shader shader (LoadShader(0, TextFormat("resources/shaders/glsl%i/swirl.fs", GLSL_VERSION)));

    // Get variable (uniform) location on the shader to connect with the program
    // NOTE: If uniform variable could not be found in the shader, function returns -1
    int swirlCenterLoc = shader.GetLocation("center");

    std::array<float, 2> swirlCenter {{ (float)screenWidth/2, (float)screenHeight/2 }};

    // Create a RenderTexture2D to be used for render to texture
    raylib::RenderTexture2D target (screenWidth, screenHeight);

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        camera.Update(CAMERA_ORBITAL);

        raylib::Vector2 mousePosition = GetMousePosition();

        swirlCenter[0] = mousePosition.x;
        swirlCenter[1] = screenHeight - mousePosition.y;

        // Send new value to the shader to be used on drawing
        shader.SetValue(swirlCenterLoc, swirlCenter);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        {
            // Enable drawing to texture
            raylib::RenderTextureDrawingGuard renderTextureDrawingGuard (target);
            ClearBackground(RAYWHITE);  // Clear texture background

            BeginMode3D(camera);        // Begin 3d mode drawing
            model.Draw(position, 0.5f, WHITE);   // Draw 3d model with texture
            DrawGrid(10, 1.0f);     // Draw a grid
            EndMode3D();                // End 3d mode drawing, returns to orthographic 2d mode

            DrawText("TEXT DRAWN IN RENDER TEXTURE", 200, 10, 30, RED);
        }               // End drawing to texture (now we have a texture available for next passes)

        BeginDrawing();
        ClearBackground(RAYWHITE);  // Clear screen background

        {
            // Enable shader using the custom uniform
            raylib::ShaderDrawingGuard shaderDrawingGuard (shader);

            // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
            DrawTextureRec(target.GetTextureC(),
                           (Rectangle) {0, 0, (float) target.GetTextureC().width, (float) -target.GetTextureC().height},
                           (Vector2) {0, 0}, WHITE);
        }

        // Draw some 2d text over drawn texture
        DrawText("(c) Barracks 3D model by Alberto Cano", screenWidth - 220, screenHeight - 20, 10, GRAY);
        DrawFPS(10, 10);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //UnloadShader(shader);               // Unload shader
    //UnloadTexture(texture);             // Unload texture
    //UnloadModel(model);                 // Unload model
    //UnloadRenderTexture(target);        // Unload render texture

    CloseWindow();                      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}