/*******************************************************************************************
 *
 *   raylib [shaders] example - texture tiling
 *
 *   Example demonstrates how to tile a texture on a 3D model using raylib.
 *
 *   Example contributed by Luis Almeida (@luis605) and reviewed by Ramon Santamaria (@raysan5)
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
 *   BSD-like license that allows static linking with closed source software
 *
 *   Copyright (c) 2023 Luis Almeida (@luis605)
 *
 ********************************************************************************************/

#include "raylib-cpp.hpp"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [shaders] example - texture tiling");

    // Define the camera to look into our 3d world
    raylib::Camera3D camera;
    camera.position = Vector3{4.0f, 4.0f, 4.0f}; // Camera position
    camera.target = Vector3{0.0f, 0.5f, 0.0f}; // Camera looking at point
    camera.up = Vector3{0.0f, 1.0f, 0.0f}; // Camera up vector (rotation towards target)
    camera.fovy = 45.0f; // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE; // Camera projection type

    // Load a cube model
    raylib::Model model(LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f)));

    // Load a texture and assign to cube model
    raylib::Texture2D texture("resources/cubicmap_atlas.png");
    model.SetMaterialMapTexture(0, MATERIAL_MAP_DIFFUSE, texture, raylib::ModelMaterialTextureOption::NoUnload);

    // Set the texture tiling using a shader
    std::array<float, 2> tiling = {3.0f, 3.0f};
    raylib::Shader shader(raylib::Shader::LoadShaderOptions{
        .vsFileName = "",
        .fsFileName = TextFormat("resources/shaders/glsl%i/tiling.fs", GLSL_VERSION)});
    shader.SetValueFromLocation("tiling", tiling);
    model.SetMaterialShader(0, shader, raylib::ModelMaterialShaderOption::NoUnload);

    DisableCursor(); // Limit cursor to relative movement inside the window

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_FREE);

        if (IsKeyPressed('Z')) camera.target = {0.0f, 0.5f, 0.0f};
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        {
            raylib::ShaderDrawingGuard shaderDrawingGuard(shader);
            model.Draw(Vector3{0.0f, 0.0f, 0.0f}, 2.0f, WHITE);
        }

        DrawGrid(10, 1.0f);

        EndMode3D();

        DrawText("Use mouse to rotate the camera", 10, 10, 20, DARKGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //UnloadModel(model);         // Unload model
    //UnloadShader(shader);       // Unload shader
    //UnloadTexture(texture);     // Unload texture

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}