/*******************************************************************************************
*
*   raylib [textures] example - Image loading and drawing on it
*
*   NOTE: Images are loaded in CPU memory (RAM); textures are loaded in GPU memory (VRAM)
*
*   This example has been created using raylib 1.4 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib-cpp.hpp"

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int ScreenWidth = 800;
    constexpr int ScreenHeight = 450;

    raylib::Window window(ScreenWidth, ScreenHeight, "raylib [textures] example - image drawing");
    const raylib::Color darkGray {DARKGRAY};

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    raylib::Image cat("resources/cat.png");             // Load image in CPU memory (RAM)
    cat.Crop(raylib::Rectangle{{.x = 100, .y = 10, .width = 280, .height = 380}})      // Crop an image piece
        .FlipHorizontal()                              // Flip cropped image horizontally
        .Resize(150, 200);                            // Resize flipped-cropped image

    raylib::Image parrots("resources/parrots.png");     // Load image in CPU memory (RAM)

    // Draw one image over the other with a scaling of 1.5f
    parrots
        .Draw(cat,
            raylib::Rectangle{{.x = 0, .y = 0, .width = static_cast<float>(cat.GetWidth()), .height = static_cast<float>(cat.GetHeight())}},
            raylib::Rectangle{{.x = 30, .y = 40, .width = static_cast<float>(cat.GetWidth()) * 1.5F, .height = static_cast<float>(cat.GetHeight()) * 1.5F}});
    parrots.Crop(raylib::Rectangle{{.x = 0, .y = 50, .width = static_cast<float>(parrots.GetWidth()),
                                   .height = static_cast<float>(parrots.GetHeight() - 100)}});  // Crop resulting image

    // Load custom font for frawing on image
    raylib::Font font("resources/custom_jupiter_crash.png");

    // Draw over image using custom font
    parrots.DrawText(font.c_raylib(), "PARROTS & CAT", raylib::Vector2{{.x = 300,.y =  230}}, static_cast<float>(font.GetBaseSize()), -2);

    raylib::Texture2D texture(parrots);      // Image converted to texture, uploaded to GPU memory (VRAM)

    SetTargetFPS(60);
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {    // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        // Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);

            texture.Draw(ScreenWidth / 2 - texture.GetWidth() / 2,
                ScreenHeight / 2 - texture.GetHeight() / 2 - 40);
            darkGray.DrawRectangleLines(ScreenWidth / 2 - texture.GetWidth() / 2,
                ScreenHeight / 2 - texture.GetHeight() / 2 - 40,
                texture.GetWidth(), texture.GetHeight());

            darkGray.DrawText("We are drawing only one texture from various images composed!",
                240, 350, 10);
            darkGray.DrawText("Source images have been cropped, scaled, flipped and copied one over the other.",
                190, 370, 10);
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}
