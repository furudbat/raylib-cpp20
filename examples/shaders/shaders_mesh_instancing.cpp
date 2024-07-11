/*******************************************************************************************
*
*   raylib [shaders] example - Mesh instancing
*
*   Example originally created with raylib 3.7, last time updated with raylib 4.2
*
*   Example contributed by @seanpringle and reviewed by Max (@moliad) and Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2020-2024 @seanpringle, Max (@moliad) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/


#include "raylib-cpp.hpp"
#include <raymath.h>

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include <vector>
#include <cstdlib>

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

inline static constexpr size_t MaxInstances = 10000;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [shaders] example - mesh instancing");

    // Define the camera to look into our 3d world
    raylib::Camera camera;
    camera.position = (Vector3){ -125.0f, 125.0f, -125.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };              // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };                  // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                        // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                     // Camera projection type

    // Define mesh to be instanced
    raylib::Mesh cube (GenMeshCube(1.0f, 1.0f, 1.0f));

    // Define transforms to be uploaded to GPU for instances
    std::vector<::Matrix> transforms (MaxInstances);   // Pre-multiplied transformations passed to rlgl

    // Translate and rotate cubes randomly
    for (auto& mat : transforms)
    {
        const raylib::Matrix translation = MatrixTranslate((float)GetRandomValue(-50, 50), (float)GetRandomValue(-50, 50), (float)GetRandomValue(-50, 50));
        const raylib::Vector3 axis (Vector3Normalize(Vector3{ (float)GetRandomValue(0, 360), (float)GetRandomValue(0, 360), (float)GetRandomValue(0, 360) }));
        const float angle = (float)GetRandomValue(0, 10)*DEG2RAD;
        const raylib::Matrix rotation = MatrixRotate(axis, angle);

        mat = MatrixMultiply(rotation, translation);
    }

    // Load lighting shader
    const auto LoadMaterialShader = []() {
        raylib::Shader shader(LoadShader(TextFormat("resources/shaders/glsl%i/lighting_instancing.vs", GLSL_VERSION),
                                         TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION)));
        // Get shader locations
        shader.SetLocFromLocation(raylib::ShaderLocationIndex::MatrixMvp, "mvp");
        shader.SetLocFromLocation(raylib::ShaderLocationIndex::VectorView, "viewPos");
        shader.SetLocFromLocation(raylib::ShaderLocationIndex::MatrixModel, "instanceTransform");

        // Set shader value: ambient light level
        //const int ambientLoc = shader.GetLocation("ambient");
        shader.SetValueFromLocation("ambient", std::array<float, 4>{{0.2f, 0.2f, 0.2f, 1.0f}});

        // Create one light
        CreateLight(LIGHT_DIRECTIONAL, Vector3{50.0f, 50.0f, 0.0f}, Vector3Zero(), WHITE, shader.c_raylib());

        return shader;
    };

    // NOTE: We are assigning the intancing shader to material.shader
    // to be used on mesh drawing with DrawMeshInstanced()
    raylib::Material matInstances (LoadMaterialDefault());
    matInstances.SetShader(LoadMaterialShader());
    matInstances.GetMap(MATERIAL_MAP_DIFFUSE).color = RED;

    // Load default material (using raylib intenral default shader) for non-instanced mesh drawing
    // WARNING: Default shader enables vertex color attribute BUT GenMeshCube() does not generate vertex colors, so,
    // when drawing the color attribute is disabled and a default color value is provided as input for thevertex attribute
    raylib::Material matDefault (LoadMaterialDefault());
    matDefault.GetMap(MATERIAL_MAP_DIFFUSE).color = BLUE;

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        camera.Update(CAMERA_ORBITAL);

        /*
        matInstances.UpdateShader([&](auto& shader){
            // Update the light shader with the camera view position
            const std::array<float, 3> cameraPos {{ camera.position.x, camera.position.y, camera.position.z }};
            shader.SetValue(shader.GetLoc(SHADER_LOC_VECTOR_VIEW), cameraPos);
        });
        */
        // Update the light shader with the camera view position
        //const std::array<float, 3> cameraPos {{ camera.position.x, camera.position.y, camera.position.z }};
        matInstances.SetShaderValueFromLoc(SHADER_LOC_VECTOR_VIEW, camera.position);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        {
            raylib::DrawingGuard drawingGuard;
            ClearBackground(RAYWHITE);

            // draw camera
            {
                raylib::Camera3DDrawingGuard cameraDrawing (camera);

                // Draw cube mesh with default material (BLUE)
                cube.Draw(matDefault.c_raylib(), MatrixTranslate(-10.0f, 0.0f, 0.0f));

                // Draw meshes instanced using material containing instancing shader (RED + lighting),
                // transforms[] for the instances should be provided, they are dynamically
                // updated in GPU every frame, so we can animate the different mesh instances
                cube.Draw(matInstances.c_raylib(), std::span{transforms});

                // Draw cube mesh with default material (BLUE)
                cube.Draw(matDefault.c_raylib(), MatrixTranslate(10.0f, 0.0f, 0.0f));
            }

            DrawFPS(10, 10);
        }
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //RL_FREE(transforms);    // Free transforms

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}