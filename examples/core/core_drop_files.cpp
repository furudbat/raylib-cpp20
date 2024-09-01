/*******************************************************************************************
 *
 *   raylib [core] example - Windows drop files
 *
 *   This example only works on platforms that support drag & drop (Windows, Linux, OSX, Html5?)
 *
 *   This example has been created using raylib-cpp (www.raylib.com)
 *   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
 *
 *   Copyright (c) 2020 Rob Loach (@RobLoach)
 *
 ********************************************************************************************/

#include "raylib-cpp.hpp"

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int ScreenWidth = 800;
    constexpr int ScreenHeight = 450;

    raylib::Window window(ScreenWidth, ScreenHeight, "raylib [core] example - drop files");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) { // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        std::vector<std::filesystem::path> droppedFiles = []
        {
            if (::IsFileDropped()) {
                return raylib::LoadDroppedFiles();
            }
            return std::vector<std::filesystem::path>{};
        }();
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);

            // Check if there are files to process.
            if (droppedFiles.empty()) {
                raylib::DrawText("Drop your files to this window!", 100, 40, 20, DARKGRAY);
            }
            else {
                raylib::DrawText("Dropped files:", 100, 40, 20, DARKGRAY);

                // Iterate through all the dropped files.
                for (size_t i = 0; i < droppedFiles.size(); i++) {
                    if (i % 2 == 0)
                        DrawRectangle(0, static_cast<int>(85 + 40 * i), ScreenWidth, 40, Fade(LIGHTGRAY, 0.5f));
                    else
                        DrawRectangle(0, static_cast<int>(85 + 40 * i), ScreenWidth, 40, Fade(LIGHTGRAY, 0.3f));

                    // Display the path to the dropped file.
                    raylib::DrawText(droppedFiles[i].c_str(), 120, static_cast<int>(100 + 40 * i), 10, GRAY);
                }

                raylib::DrawText(
                    "Drop new files...",
                    100,
                    110 + 40 * static_cast<int>(droppedFiles.size()),
                    20,
                    DARKGRAY);
            }
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}
