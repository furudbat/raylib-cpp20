#include <tl/expected.hpp>

#include "raylib-cpp.hpp"
#include "raylib-assert.h"
#include <string>
#include <vector>
#include <filesystem>
#include <variant>
#include <unordered_map>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE( "first person maze", "[models]" ) {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int ScreenWidth = 800;
    constexpr int ScreenHeight = 450;

    raylib::Window window;
    REQUIRE(window.Init(ScreenWidth, ScreenHeight, "raylib [models] example - first person maze"));

    // Define the camera to look into our 3d world
    raylib::Camera camera({ 0.2f, 0.4f, 0.2f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f);

    raylib::Texture cubicmap;
    raylib::Texture texture;
    raylib::RayImageColors mapPixels;
    raylib::Model model;

    // load from imMap
    {
        raylib::Image imMap;
        REQUIRE(imMap.Load("resources/cubicmap.png"));      // Load cubicmap image (RAM)
        REQUIRE(cubicmap.Load(imMap));                    // Convert image to texture to display (VRAM)
        REQUIRE(model.Load(raylib::Mesh::Cubicmap(imMap, Vector3{1.0f, 1.0f, 1.0f})));

        // NOTE: By default each cube is mapped to one part of texture atlas
        REQUIRE(texture.Load("resources/cubicmap_atlas.png"));    // Load map texture
        model.GetMaterials()[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture.c_raylib();     // Set map diffuse texture

        // Get map image data to be used for collision detection
        mapPixels = imMap.LoadColors();
    }

    raylib::Vector3 mapPosition({.x = -16.0F,.y =  0.0F,.z = -8.0F});   // Set model position
    raylib::Vector3 playerPosition(camera.position);    // Set player position

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    REQUIRE_FALSE(window.ShouldClose());
    // Update
    //----------------------------------------------------------------------------------
    raylib::Vector3 oldCamPos(camera.position);    // Store old camera position

    camera.Update(CAMERA_FIRST_PERSON);        // Update camera

    // Check player collision (we simplify to 2D collision detection)
    raylib::Vector2 playerPos({.x = camera.position.x,.y = camera.position.z});
    float playerRadius = 0.1F;  // Collision radius (player is modelled as a cilinder for collision)

    int playerCellX = static_cast<int>(playerPos.x - mapPosition.x + 0.5F);
    int playerCellY = static_cast<int>(playerPos.y - mapPosition.z + 0.5F);

    // Out-of-limits security check
    if (playerCellX < 0) playerCellX = 0;
    else if (playerCellX >= cubicmap.GetWidth()) playerCellX = cubicmap.GetWidth() - 1;

    if (playerCellY < 0) playerCellY = 0;
    else if (playerCellY >= cubicmap.GetHeight()) playerCellY = cubicmap.GetHeight() - 1;

    // Check map collisions using image data and player position
    // TODO: Improvement: Just check player surrounding cells for collision
    for (int y = 0; y < cubicmap.GetHeight(); y++)
    {
        for (int x = 0; x < cubicmap.GetWidth(); x++)
        {
            std::span data = mapPixels.AsSpan();
            /// @TODO: check data for size
            if ((data[static_cast<size_t>(y * cubicmap.GetWidth() + x)].r == 255) &&       // Collision: white pixel, only check R channel
                (playerPos.CheckCollisionCircle(playerRadius,
                                                Rectangle{ mapPosition.x - 0.5f + static_cast<float>(x)*1.0f, mapPosition.z - 0.5f + static_cast<float>(y)*1.0f, 1.0f, 1.0f })))
            {
                // Collision detected, reset camera position
                camera.position = oldCamPos;
            }
        }
    }
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    {
        window.ClearBackground(RAYWHITE);

        camera.BeginMode();
        {
            model.Draw(mapPosition);               // Draw maze map
            // playerPosition.DrawCube((Vector3){ 0.2f, 0.4f, 0.2f }, RED);  // Draw player
        }
        camera.EndMode();

        cubicmap.Draw(Vector2{ static_cast<float>(GetScreenWidth() - cubicmap.GetWidth()*4 - 20), 20 }, 0.0f, 4.0f, WHITE);
        DrawRectangleLines(GetScreenWidth() - cubicmap.GetWidth()*4 - 20, 20, cubicmap.GetWidth()*4, cubicmap.GetHeight()*4, GREEN);

        // Draw player position radar
        DrawRectangle(GetScreenWidth() - cubicmap.GetWidth()*4 - 20 + playerCellX*4, 20 + playerCellY*4, 4, 4, RED);

        DrawFPS(10, 10);
    }
    EndDrawing();
    //----------------------------------------------------------------------------------

    window.Close();
}


TEST_CASE( "model and texture with variant", "[models][textures]" ) {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int ScreenWidth = 800;
    constexpr int ScreenHeight = 450;

    raylib::Window window;
    REQUIRE(window.Init(ScreenWidth, ScreenHeight, "raylib [models] example - variant"));

    // Define the camera to look into our 3d world
    raylib::Camera camera({0.2f, 0.4f, 0.2f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f);

    raylib::Texture texture;
    raylib::Texture cubicmap;

    const auto loadTextureAtlas = [] {
        raylib::Texture atlas_texture;
        // NOTE: By default each cube is mapped to one part of texture atlas
        REQUIRE(atlas_texture.Load("resources/cubicmap_atlas.png"));    // Load map texture
        return atlas_texture;
    };

    const auto loadModel = [&] {
        raylib::Model model;
        raylib::Image imMap;
        REQUIRE(imMap.Load("resources/cubicmap.png"));      // Load cubicmap image (RAM)
        REQUIRE(cubicmap.Load(imMap));                    // Convert image to texture to display (VRAM)
        REQUIRE(model.Load(raylib::Mesh::Cubicmap(imMap, Vector3{1.0f, 1.0f, 1.0f})));

        // NOTE: By default each cube is mapped to one part of texture atlas
        REQUIRE(texture.Load("resources/cubicmap_atlas.png"));    // Load map texture
        model.GetMaterials()[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture.c_raylib();     // Set map diffuse texture

        return model;
    };

    SECTION("variant with model") {
        std::variant<raylib::Model, raylib::Texture> variant = loadModel();
    }

    SECTION("map with model") {
        std::unordered_map<std::string, std::variant<raylib::Model, raylib::Texture>> map;
        map["model"] = loadModel();
        map["texture"] = loadTextureAtlas();

        REQUIRE(map.contains("model"));
        REQUIRE(map.contains("texture"));
    }

    SECTION("vector with model") {
        std::vector<raylib::Model> models;
        models.emplace_back(std::move(loadModel()));
        models.emplace_back(std::move(loadModel()));
        models.emplace_back(std::move(loadModel()));
    }

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    REQUIRE_FALSE(window.ShouldClose());
    // Update
    //----------------------------------------------------------------------------------

    camera.Update(CAMERA_FIRST_PERSON);        // Update camera

    window.Close();
}