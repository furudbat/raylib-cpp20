/*******************************************************************************************
*
*   raylib [shaders] example - fog
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3).
*
*   Example originally created with raylib 2.5, last time updated with raylib 3.7
*
*   Example contributed by Chris Camacho (@chriscamacho) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 Chris Camacho (@chriscamacho) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib-cpp.hpp"

#include <raymath.h>

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

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

    SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)
    InitWindow(screenWidth, screenHeight, "raylib [shaders] example - fog");

    // Define the camera to look into our 3d world
    raylib::Camera camera;
    camera.SetPosition((Vector3){ 2.0f, 2.0f, 6.0f });    // Camera position
    camera.SetTarget((Vector3){ 0.0f, 0.5f, 0.0f });      // Camera looking at point
    camera.SetUp((Vector3){ 0.0f, 1.0f, 0.0f });          // Camera up vector (rotation towards target)
    camera.SetFovy(45.0f);                                // Camera field-of-view Y
    camera.SetProjection(CAMERA_PERSPECTIVE);             // Camera projection type

    // Load models and texture
    raylib::Model modelA = raylib::Model::LoadFromMesh(GenMeshTorus(0.4f, 1.0f, 16, 32));
    raylib::Model modelB = raylib::Model::LoadFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    raylib::Model modelC = raylib::Model::LoadFromMesh(GenMeshSphere(0.5f, 32, 32));
    raylib::Texture texture = raylib::Texture::LoadFromFile("resources/texel_checker.png");

    // Assign texture to default model material
    modelA.GetMaterials()[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture.c_raylib();
    modelB.GetMaterials()[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture.c_raylib();
    modelC.GetMaterials()[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture.c_raylib();

    // Load shader and set up some uniforms
    raylib::Shader shader (LoadShader(TextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION),
                                      TextFormat("resources/shaders/glsl%i/fog.fs", GLSL_VERSION)));
    shader.SetLocFromUniform(SHADER_LOC_MATRIX_MODEL, "matModel");
    shader.SetLocFromUniform(SHADER_LOC_VECTOR_VIEW, "viewPos");

    // Ambient light level
    int ambientLoc = shader.GetLocation("ambient");
    shader.SetValue(ambientLoc, std::array<float, 4>{{ 0.2f, 0.2f, 0.2f, 1.0f }});

    float fogDensity = 0.15f;
    int fogDensityLoc = shader.GetLocation("fogDensity");
    shader.SetValue(fogDensityLoc, fogDensity);

    // NOTE: All models share the same shader
    modelA.GetMaterials()[0].shader = shader.c_raylib();
    modelB.GetMaterials()[0].shader = shader.c_raylib();
    modelC.GetMaterials()[0].shader = shader.c_raylib();

    // Using just 1 point lights
    CreateLight(LIGHT_POINT, (Vector3){ 0, 2, 6 }, Vector3Zero(), WHITE, shader.c_raylib());

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        camera.Update(CAMERA_ORBITAL);

        if (IsKeyDown(KEY_UP))
        {
            fogDensity += 0.001f;
            if (fogDensity > 1.0f) fogDensity = 1.0f;
        }

        if (IsKeyDown(KEY_DOWN))
        {
            fogDensity -= 0.001f;
            if (fogDensity < 0.0f) fogDensity = 0.0f;
        }

        shader.SetValue(fogDensityLoc, fogDensity);

        // Rotate the torus
        modelA.SetTransform(MatrixMultiply(modelA.GetTransform(), MatrixRotateX(-0.025f)));
        modelA.SetTransform(MatrixMultiply(modelA.GetTransform(), MatrixRotateZ(0.012f)));

        // Update the light shader with the camera view position
        shader.SetValue(shader.GetLoc(SHADER_LOC_VECTOR_VIEW), camera.position);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(GRAY);

        BeginMode3D(camera);

        // Draw the three models
        modelA.Draw(Vector3Zero(), 1.0f, WHITE);
        modelB.Draw((Vector3){ -2.6f, 0, 0 }, 1.0f, WHITE);
        modelC.Draw((Vector3){ 2.6f, 0, 0 }, 1.0f, WHITE);

        for (int i = -20; i < 20; i += 2) modelA.Draw((Vector3){ (float)i, 0, 2 }, 1.0f, WHITE);

        EndMode3D();

        DrawText(TextFormat("Use KEY_UP/KEY_DOWN to change fog density [%.2f]", fogDensity), 10, 10, 20, RAYWHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //UnloadModel(modelA);        // Unload the model A
    //UnloadModel(modelB);        // Unload the model B
    //UnloadModel(modelC);        // Unload the model C
    //UnloadTexture(texture);     // Unload the texture
    //UnloadShader(shader);       // Unload shader

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}