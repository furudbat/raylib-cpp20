/*******************************************************************************************
 *
 *   raylib [shaders] example - Depth buffer writing
 *
 *   Example originally created with raylib 4.2, last time updated with raylib 4.2
 *
 *   Example contributed by Buğra Alptekin Sarı (@BugraAlptekinSari) and reviewed by Ramon Santamaria (@raysan5)
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
 *   BSD-like license that allows static linking with closed source software
 *
 *   Copyright (c) 2022-2024 Buğra Alptekin Sarı (@BugraAlptekinSari)
 *
 ********************************************************************************************/

#include "raylib-cpp.hpp"

#include <rlgl.h>

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

//------------------------------------------------------------------------------------
// Declare custom functions required for the example
//------------------------------------------------------------------------------------
// Load custom render texture, create a writable depth texture buffer
static raylib::RenderTexture2D LoadRenderTextureDepthTex(int width, int height);

// Unload render texture from GPU memory (VRAM)
// static void UnloadRenderTextureDepthTex(RenderTexture2D target);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [shaders] example - write depth buffer");

    // The shader inverts the depth buffer by writing into it by `gl_FragDepth = 1 - gl_FragCoord.z;`
    raylib::Shader shader(LoadShader(0, TextFormat("resources/shaders/glsl%i/write_depth.fs", GLSL_VERSION)));

    // Use Customized function to create writable depth texture buffer
    raylib::RenderTexture2D target = LoadRenderTextureDepthTex(screenWidth, screenHeight);

    // Define the camera to look into our 3d world
    raylib::Camera camera(
        /*.position = */ Vector3{2.0f, 2.0f, 3.0f}, // Camera position
        /*.target = */ Vector3{0.0f, 0.5f, 0.0f}, // Camera looking at point
        /*.up = */ Vector3{0.0f, 1.0f, 0.0f}, // Camera up vector (rotation towards target)
        /*.fovy = */ 45.0f, // Camera field-of-view Y
        /*.projection = */ CAMERA_PERSPECTIVE // Camera projection type
    );

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

        // Draw into our custom render texture (framebuffer)
        {
            raylib::RenderTextureDrawingGuard renderTextureDrawingGuard(target);
            ClearBackground(WHITE);

            BeginMode3D(camera);
            {
                raylib::ShaderDrawingGuard shaderDrawingGuard(shader);

                DrawCubeWiresV((Vector3){0.0f, 0.5f, 1.0f}, (Vector3){1.0f, 1.0f, 1.0f}, RED);
                DrawCubeV((Vector3){0.0f, 0.5f, 1.0f}, (Vector3){1.0f, 1.0f, 1.0f}, PURPLE);
                DrawCubeWiresV((Vector3){0.0f, 0.5f, -1.0f}, (Vector3){1.0f, 1.0f, 1.0f}, DARKGREEN);
                DrawCubeV((Vector3){0.0f, 0.5f, -1.0f}, (Vector3){1.0f, 1.0f, 1.0f}, YELLOW);
                DrawGrid(10, 1.0f);
            }
            EndMode3D();
            EndTextureMode();
        }

        // Draw into screen our custom render texture
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTextureRec(
            target.GetTextureC(),
            Rectangle{0, 0, (float)screenWidth, (float)-screenHeight},
            Vector2{0, 0},
            WHITE);
        DrawFPS(10, 10);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Color texture attached to FBO is deleted
    // rlUnloadTexture(target.texture.id);
    // rlUnloadTexture(target.depth.id);
    // NOTE: Depth texture is automatically
    // queried and deleted before deleting framebuffer
    // rlUnloadFramebuffer(target.id);
    // UnloadShader(shader);

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Define custom functions required for the example
//------------------------------------------------------------------------------------
// Load custom render texture, create a writable depth texture buffer
raylib::RenderTexture2D LoadRenderTextureDepthTex(int width, int height) {
    raylib::RenderTexture2D target(
        rlLoadFramebuffer(width, height),
        raylib::RenderTextureOptions::UnloadFramebuffer); // Load an empty framebuffer

    if (target.GetId() > 0) {
        rlEnableFramebuffer(target.GetId());

        // Create color texture (default to RGBA)
        target.SetTexture(
            ::Texture{
                .id = rlLoadTexture(0, width, height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1),
                .width = width,
                .height = height,
                .mipmaps = 1,
                .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
            },
            raylib::RenderTextureTextureOptions::UnloadTexture);

        // Create depth texture buffer (instead of raylib default renderbuffer)
        target.SetDepth(
            ::Texture{
                .id = rlLoadTextureDepth(width, height, false),
                .width = width,
                .height = height,
                .mipmaps = 1,
                .format = 19, // DEPTH_COMPONENT_24BIT?
            },
            raylib::RenderTextureTextureOptions::UnloadTexture);

        // Attach color texture and depth texture to FBO
        rlFramebufferAttach(
            target.GetId(),
            target.GetTextureC().id,
            RL_ATTACHMENT_COLOR_CHANNEL0,
            RL_ATTACHMENT_TEXTURE2D,
            0);
        rlFramebufferAttach(target.GetId(), target.GetDepthC().id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

        // Check if fbo is complete with attachments (valid)
        if (rlFramebufferComplete(target.GetId()))
            TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

        rlDisableFramebuffer();
    }
    else
        TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

    return target;
}
