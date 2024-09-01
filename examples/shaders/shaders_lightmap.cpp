/*******************************************************************************************
 *
 *   raylib [shaders] example - lightmap
 *
 *   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
 *         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
 *
 *   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3).
 *
 *   Example contributed by Jussi Viitala (@nullstare) and reviewed by Ramon Santamaria (@raysan5)
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
 *   BSD-like license that allows static linking with closed source software
 *
 *   Copyright (c) 2019-2024 Jussi Viitala (@nullstare) and Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include <cstdio>
#include <cstdlib>

#include "raylib-cpp.hpp"
#include <raymath.h>
#include <rlgl.h>

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

inline constexpr int MapSize = 10;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT); // Enable Multi Sampling Anti Aliasing 4x (if available)
    InitWindow(screenWidth, screenHeight, "raylib [shaders] example - lightmap");

    // Define the camera to look into our 3d world
    raylib::Camera camera;
    camera.position = (Vector3){4.0f, 6.0f, 8.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.0f, 0.0f}; // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f}; // Camera up vector (rotation towards target)
    camera.fovy = 45.0f; // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE; // Camera projection type

    raylib::Mesh mesh(GenMeshPlane((float)MapSize, (float)MapSize, 1, 1));

    // GenMeshPlane doesn't generate texcoords2 so we will upload them separately
    mesh.ReallocTexCoords2(mesh.GetVertexCount());

    // X                            // Y
    mesh.GetTexCoord2(0) = 0.0f;
    mesh.GetTexCoord2(1) = 0.0f;
    mesh.GetTexCoord2(2) = 1.0f;
    mesh.GetTexCoord2(3) = 0.0f;
    mesh.GetTexCoord2(4) = 0.0f;
    mesh.GetTexCoord2(5) = 1.0f;
    mesh.GetTexCoord2(6) = 1.0f;
    mesh.GetTexCoord2(7) = 1.0f;

    // Load a new texcoords2 attributes buffer
    mesh.SetVboId(
        SHADER_LOC_VERTEX_TEXCOORD02,
        rlLoadVertexBuffer(mesh.GetTexCoords2(), mesh.GetVertexCount() * 2 * sizeof(float), false));
    rlEnableVertexArray(mesh.GetVaoId());

    // Index 5 is for texcoords2
    rlSetVertexAttribute(5, 2, RL_FLOAT, 0, 0, 0);
    rlEnableVertexAttribute(5);
    rlDisableVertexArray();

    // Load lightmap shader
    raylib::Shader shader(LoadShader(
        TextFormat("resources/shaders/glsl%i/lightmap.vs", GLSL_VERSION),
        TextFormat("resources/shaders/glsl%i/lightmap.fs", GLSL_VERSION)));

    raylib::Texture texture(LoadTexture("resources/cubicmap_atlas.png"));
    raylib::Texture light(LoadTexture("resources/spark_flame.png"));

    texture.GenMipmaps();
    texture.SetFilter(raylib::TextureFilter::Trilinear);

    raylib::RenderTexture lightmap(MapSize, MapSize);

    SetTextureFilter(lightmap.GetTextureC(), TEXTURE_FILTER_TRILINEAR);

    raylib::Material material(LoadMaterialDefault());
    material.SetShader(shader, raylib::MaterialShaderOption::UnbindShaderWhenUnload);
    material.SetMaterialTexture(MATERIAL_MAP_ALBEDO, texture);
    material.SetMaterialTexture(MATERIAL_MAP_METALNESS, lightmap.GetTextureC());

    // Drawing to lightmap
    {
        raylib::RenderTextureDrawingGuard renderTextureDrawingGuard(lightmap);
        ClearBackground(BLACK);

        BeginBlendMode(BLEND_ADDITIVE);
        light.Draw(
            (Rectangle){0, 0, (float)light.GetWidth(), (float)light.GetHeight()},
            (Rectangle){0, 0, 20, 20},
            (Vector2){10.0, 10.0},
            0.0,
            RED);
        light.Draw(
            (Rectangle){0, 0, (float)light.GetWidth(), (float)light.GetHeight()},
            (Rectangle){8, 4, 20, 20},
            (Vector2){10.0, 10.0},
            0.0,
            BLUE);
        light.Draw(
            (Rectangle){0, 0, (float)light.GetWidth(), (float)light.GetHeight()},
            (Rectangle){8, 8, 10, 10},
            (Vector2){5.0, 5.0},
            0.0,
            GREEN);
        BeginBlendMode(BLEND_ALPHA);
    }

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        camera.Update(CAMERA_ORBITAL);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        mesh.Draw(material.c_raylib(), MatrixIdentity());
        EndMode3D();

        DrawFPS(10, 10);

        DrawTexturePro(
            lightmap.GetTextureC(),
            (Rectangle){0, 0, -MapSize, -MapSize},
            (Rectangle){static_cast<float>(GetRenderWidth() - MapSize * 8 - 10), 10, MapSize * 8, MapSize * 8},
            (Vector2){0.0, 0.0},
            0.0,
            WHITE);

        DrawText("lightmap", GetRenderWidth() - 66, 16 + MapSize * 8, 10, GRAY);
        DrawText("10x10 pixels", GetRenderWidth() - 76, 30 + MapSize * 8, 10, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //UnloadMesh(mesh);       // Unload the mesh
    //UnloadShader(shader);   // Unload shader

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}