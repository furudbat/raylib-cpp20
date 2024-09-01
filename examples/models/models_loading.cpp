/*******************************************************************************************
 *
 *   raylib [models] example - Models loading
 *
 *   NOTE: raylib supports multiple models file formats:
 *
 *     - OBJ  > Text file format. Must include vertex position-texcoords-normals information,
 *              if files references some .mtl materials file, it will be loaded (or try to).
 *     - GLTF > Text/binary file format. Includes lot of information and it could
 *              also reference external files, raylib will try loading mesh and materials data.
 *     - IQM  > Binary file format. Includes mesh vertex data but also animation data,
 *              raylib can load .iqm animations.
 *     - VOX  > Binary file format. MagikaVoxel mesh format:
 *              https://github.com/ephtracy/voxel-model/blob/master/MagicaVoxel-file-format-vox.txt
 *     - M3D  > Binary file format. Model 3D format:
 *              https://bztsrc.gitlab.io/model3d
 *
 *   Example originally created with raylib 2.0, last time updated with raylib 4.2
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
 *   BSD-like license that allows static linking with closed source software
 *
 *   Copyright (c) 2014-2024 Ramon Santamaria (@raysan5)
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

    InitWindow(screenWidth, screenHeight, "raylib [models] example - models loading");

    // Define the camera to look into our 3d world
    raylib::Camera camera;
    camera.position = (Vector3){50.0f, 50.0f, 50.0f}; // Camera position
    camera.target = (Vector3){0.0f, 10.0f, 0.0f}; // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f}; // Camera up vector (rotation towards target)
    camera.fovy = 45.0f; // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE; // Camera mode type

    raylib::Model model(LoadModel("resources/models/obj/castle.obj")); // Load model
    raylib::Texture2D texture(LoadTexture("resources/models/obj/castle_diffuse.png")); // Load model texture
    model.SetMaterialMapTexture(
        0,
        MATERIAL_MAP_DIFFUSE,
        texture,
        raylib::ModelMaterialTextureOption::NoUnload); // Set map diffuse texture

    Vector3 position = {0.0f, 0.0f, 0.0f}; // Set model position

    BoundingBox bounds = GetMeshBoundingBox(model.GetMesh(0)); // Set model bounds

    // NOTE: bounds are calculated from the original size of the model,
    // if model is scaled on drawing, bounds must be also scaled

    bool selected = false; // Selected object flag

    DisableCursor(); // Limit cursor to relative movement inside the window

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        camera.Update(CAMERA_FIRST_PERSON);

        // Load new models/textures on drag&drop
        if (IsFileDropped()) {
            FilePathList droppedFiles = LoadDroppedFiles();

            if (droppedFiles.count == 1) // Only support one file dropped
            {
                if (IsFileExtension(droppedFiles.paths[0], ".obj") || IsFileExtension(droppedFiles.paths[0], ".gltf") ||
                    IsFileExtension(droppedFiles.paths[0], ".glb") || IsFileExtension(droppedFiles.paths[0], ".vox") ||
                    IsFileExtension(droppedFiles.paths[0], ".iqm") ||
                    IsFileExtension(droppedFiles.paths[0], ".m3d")) // Model file formats supported
                {
                    model = LoadModel(droppedFiles.paths[0]); // Load new model
                    // Set current map diffuse texture
                    model.SetMaterialMapTexture(
                        0,
                        MATERIAL_MAP_DIFFUSE,
                        texture,
                        raylib::ModelMaterialTextureOption::NoUnload);

                    bounds = GetMeshBoundingBox(model.GetMesh(0));

                    // TODO: Move camera position from target enough distance to visualize model properly
                }
                else if (IsFileExtension(droppedFiles.paths[0], ".png")) // Texture file formats supported
                {
                    // Unload current model texture and load new one
                    texture = LoadTexture(droppedFiles.paths[0]);
                    model.SetMaterialMapTexture(
                        0,
                        MATERIAL_MAP_DIFFUSE,
                        texture,
                        raylib::ModelMaterialTextureOption::NoUnload);
                }
            }

            UnloadDroppedFiles(droppedFiles); // Unload filepaths from memory
        }

        // Select model on mouse click
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Check collision between ray and box
            /// @FIXME: GetScreenToWorldRay not found
            // if (GetRayCollisionBox(GetScreenToWorldRay(GetMousePosition(), camera), bounds).hit) selected =
            // !selected; else selected = false;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        model.Draw(position, 1.0f, WHITE); // Draw 3d model with texture

        DrawGrid(20, 10.0f); // Draw a grid

        if (selected) DrawBoundingBox(bounds, GREEN); // Draw selection box

        EndMode3D();

        DrawText("Drag & drop model to load mesh/texture.", 10, GetScreenHeight() - 20, 10, DARKGRAY);
        if (selected) DrawText("MODEL SELECTED", GetScreenWidth() - 110, 10, 10, GREEN);

        DrawText("(c) Castle 3D model by Alberto Cano", screenWidth - 200, screenHeight - 20, 10, GRAY);

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