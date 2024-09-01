/*******************************************************************************************
 *
 *   raylib [shaders] example - Basic PBR
 *
 *   Example originally created with raylib 5.0, last time updated with raylib 5.1-dev
 *
 *   Example contributed by Afan OLOVCIC (@_DevDad) and reviewed by Ramon Santamaria (@raysan5)
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
 *   BSD-like license that allows static linking with closed source software
 *
 *   Copyright (c) 2023-2024 Afan OLOVCIC (@_DevDad)
 *
 *   Model: "Old Rusty Car" (https://skfb.ly/LxRy) by Renafox,
 *   licensed under Creative Commons Attribution-NonCommercial
 *   (http://creativecommons.org/licenses/by-nc/4.0/)
 *
 ********************************************************************************************/

#include "raylib-cpp.hpp"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 120
#endif

#include <array>

//#define MAX_LIGHTS  4           // Max dynamic lights supported by shader
inline constexpr int MaxLights = 4;

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

// Light type
enum class LightType {
    Directional = 0,
    Point,
    Spot
};

// Light data
struct Light {
    LightType type{LightType::Directional};
    int enabled{0};
    raylib::Vector3 position;
    raylib::Vector3 target;
    std::array<float, 4> color;
    float intensity{0.0f};

    // Shader light parameters locations
    int typeLoc{0};
    int enabledLoc{0};
    int positionLoc{0};
    int targetLoc{0};
    int colorLoc{0};
    int intensityLoc{0};
};

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static int lightCount = 0; // Current number of dynamic lights that have been created

//----------------------------------------------------------------------------------
// Module specific Functions Declaration
//----------------------------------------------------------------------------------
// Create a light and get shader locations
static Light CreateLight(
    LightType type,
    ::Vector3 position,
    ::Vector3 target,
    ::Color color,
    float intensity,
    raylib::Shader& shader);

// Update light properties on shader
// NOTE: Light shader locations should be available
static void UpdateLight(raylib::Shader& shader, const Light& light);

//----------------------------------------------------------------------------------
// Main Entry Point
//----------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    raylib::InitWindow(screenWidth, screenHeight, "raylib [shaders] example - basic pbr");

    // Define the camera to look into our 3d world
    raylib::Camera camera({
        .position = Vector3{2.0f, 2.0f, 6.0f}, // Camera position
        .target = Vector3{0.0f, 0.5f, 0.0f}, // Camera looking at point
        .up = Vector3{0.0f, 1.0f, 0.0f}, // Camera up vector (rotation towards target)
        .fovy = 45.0f, // Camera field-of-view Y
        .projection = CAMERA_PERSPECTIVE, // Camera projection type
    });

    // Load PBR shader and setup all required locations
    const auto LoadPBRShader = [&]() -> raylib::Shader
    {
        raylib::Shader shader = raylib::Shader::Load(raylib::Shader::LoadShaderOptionsC{
            .vsFileName = TextFormat("resources/shaders/glsl%i/pbr.vs", GLSL_VERSION),
            .fsFileName = TextFormat("resources/shaders/glsl%i/pbr.fs", GLSL_VERSION),
        });

        shader.SetLocFromLocation(raylib::ShaderLocationIndex::MapAlbedo, "albedoMap");
        // WARNING: Metalness, roughness, and ambient occlusion are all packed into a MRA texture
        // They are passed as to the SHADER_LOC_MAP_METALNESS location for convenience,
        // shader already takes care of it accordingly
        shader.SetLocFromLocation(raylib::ShaderLocationIndex::MapMetalness, "mraMap");
        shader.SetLocFromLocation(raylib::ShaderLocationIndex::MapNormal, "normalMap");
        // WARNING: Similar to the MRA map, the emissive map packs different information
        // into a single texture: it stores height and emission data
        // It is binded to SHADER_LOC_MAP_EMISSION location an properly processed on shader
        shader.SetLocFromLocation(raylib::ShaderLocationIndex::MapEmission, "emissiveMap");
        shader.SetLocFromLocation(raylib::ShaderLocationIndex::ColorDiffuse, "albedoColor");

        // Setup additional required shader locations, including lights data
        shader.SetLocFromLocation(raylib::ShaderLocationIndex::VectorView, "viewPos");
        int lightCountLoc = shader.GetLocation("numOfLights");
        int maxLightCount = MaxLights;
        shader.SetValue(lightCountLoc, maxLightCount);

        // Setup ambient color and intensity parameters
        float ambientIntensity = 0.02f;
        Color ambientColor{26, 32, 135, 255};
        Vector3 ambientColorNormalized{ambientColor.r / 255.0f, ambientColor.g / 255.0f, ambientColor.b / 255.0f};
        shader.SetValue(shader.GetLocation("ambientColor"), ambientColorNormalized);
        shader.SetValue(shader.GetLocation("ambient"), ambientIntensity);

        return shader;
    };
    auto shader = LoadPBRShader();
    // Get location for shader parameters that can be modified in real time
    int emissiveIntensityLoc = shader.GetLocation("emissivePower");
    int emissiveColorLoc = shader.GetLocation("emissiveColor");
    int textureTilingLoc = shader.GetLocation("tiling");

    // Load old car model using PBR maps and shader
    // WARNING: We know this model consists of a single model.meshes[0] and
    // that model.materials[0] is by default assigned to that mesh
    // There could be more complex models consisting of multiple meshes and
    // multiple materials defined for those meshes... but always 1 mesh = 1 material
    raylib::Model car;
    car.Load("resources/models/old_car_new.glb");

    // Assign already setup PBR shader to model.materials[0], used by models.meshes[0]
    car.SetMaterialShader(0, shader, raylib::ModelMaterialShaderOption::UnbindShaderBeforeUnloadAndUnloadMaterial);

    // Setup materials[0].maps default parameters
    car.GetMaterialMap(0, raylib::MaterialMapIndex::Albedo).color = WHITE;
    car.GetMaterialMap(0, raylib::MaterialMapIndex::Metalness).value = 0.0f;
    car.GetMaterialMap(0, raylib::MaterialMapIndex::Roughness).value = 0.0f;
    car.GetMaterialMap(0, raylib::MaterialMapIndex::Occlusion).value = 1.0f;
    car.GetMaterialMap(0, raylib::MaterialMapIndex::Emission).color = (Color){255, 162, 0, 255};

    raylib::Texture old_car_d;
    raylib::Texture old_car_mra;
    raylib::Texture old_car_n;
    raylib::Texture old_car_e;
    old_car_d.Load("resources/old_car_d.png");
    old_car_mra.Load("resources/old_car_mra.png");
    old_car_n.Load("resources/old_car_n.png");
    old_car_e.Load("resources/old_car_e.png");

    // Setup materials[0].maps default textures (connect loaded textures)
    car.GetMaterials()[0].maps[MATERIAL_MAP_ALBEDO].texture = old_car_d.c_raylib();
    car.GetMaterials()[0].maps[MATERIAL_MAP_METALNESS].texture = old_car_mra.c_raylib();
    car.GetMaterials()[0].maps[MATERIAL_MAP_NORMAL].texture = old_car_n.c_raylib();
    car.GetMaterials()[0].maps[MATERIAL_MAP_EMISSION].texture = old_car_e.c_raylib();

    // Load floor model mesh and assign material parameters
    // NOTE: A basic plane shape can be generated instead of being loaded from a model file
    raylib::Model floor;
    floor.Load("resources/models/plane.glb");
    // Mesh floorMesh = GenMeshPlane(10, 10, 10, 10);
    //GenMeshTangents(&floorMesh);      // TODO: Review tangents generation
    // Model floor = LoadModelFromMesh(floorMesh);

    // Assign material shader for our floor model, same PBR shader
    floor.SetMaterialShader(0, shader, raylib::ModelMaterialShaderOption::UnbindShaderBeforeUnloadAndUnloadMaterial);

    floor.GetMaterials()[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;
    floor.GetMaterials()[0].maps[MATERIAL_MAP_METALNESS].value = 0.0f;
    floor.GetMaterials()[0].maps[MATERIAL_MAP_ROUGHNESS].value = 0.0f;
    floor.GetMaterials()[0].maps[MATERIAL_MAP_OCCLUSION].value = 1.0f;
    floor.GetMaterials()[0].maps[MATERIAL_MAP_EMISSION].color = BLACK;

    // connect road textures
    floor.SetMaterialMapTexture(0, MATERIAL_MAP_ALBEDO, LoadTexture("resources/road_a.png"));
    floor.SetMaterialMapTexture(0, MATERIAL_MAP_METALNESS, LoadTexture("resources/road_mra.png"));
    floor.SetMaterialMapTexture(0, MATERIAL_MAP_NORMAL, LoadTexture("resources/road_n.png"));

    // Models texture tiling parameter can be stored in the Material struct if required (CURRENTLY NOT USED)
    // NOTE: Material.params[4] are available for generic parameters storage (float)
    raylib::Vector2 carTextureTiling{Vector2{0.5f, 0.5f}};
    raylib::Vector2 floorTextureTiling{Vector2{0.5f, 0.5f}};

    // Create some lights
    std::array<Light, MaxLights> lights{{
        CreateLight(LightType::Point, Vector3{-1.0f, 1.0f, -2.0f}, Vector3{0.0f, 0.0f, 0.0f}, YELLOW, 4.0f, shader),
        CreateLight(LightType::Point, Vector3{2.0f, 1.0f, 1.0f}, Vector3{0.0f, 0.0f, 0.0f}, GREEN, 3.3f, shader),
        CreateLight(LightType::Point, Vector3{-2.0f, 1.0f, 1.0f}, Vector3{0.0f, 0.0f, 0.0f}, RED, 8.3f, shader),
        CreateLight(LightType::Point, Vector3{1.0f, 1.0f, -2.0f}, Vector3{0.0f, 0.0f, 0.0f}, BLUE, 2.0f, shader),
    }};

    // Setup material texture maps usage in shader
    // NOTE: By default, the texture maps are always used
    int usage = 1;
    shader.SetValueFromLocation("useTexAlbedo", usage);
    shader.SetValueFromLocation("useTexNormal", usage);
    shader.SetValueFromLocation("useTexMRA", usage);
    shader.SetValueFromLocation("useTexEmissive", usage);

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        camera.Update(CAMERA_ORBITAL);

        // Update the shader with the camera view vector (points towards { 0.0f, 0.0f, 0.0f })
        std::array<float, 3> cameraPos{{camera.position.x, camera.position.y, camera.position.z}};
        shader.SetValue(shader.GetLoc(SHADER_LOC_VECTOR_VIEW), cameraPos);

        // Check key inputs to enable/disable lights
        if (IsKeyPressed(KEY_ONE)) {
            lights[2].enabled = !lights[2].enabled;
        }
        if (IsKeyPressed(KEY_TWO)) {
            lights[1].enabled = !lights[1].enabled;
        }
        if (IsKeyPressed(KEY_THREE)) {
            lights[3].enabled = !lights[3].enabled;
        }
        if (IsKeyPressed(KEY_FOUR)) {
            lights[0].enabled = !lights[0].enabled;
        }

        // Update light values on shader (actually, only enable/disable them)
        for (auto& light : lights) UpdateLight(shader, light);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode3D(camera);

        // Set floor model texture tiling and emissive color parameters on shader
        shader.SetValue(textureTilingLoc, floorTextureTiling);
        Vector4 floorEmissiveColor = ColorNormalize(floor.GetMaterials()[0].maps[MATERIAL_MAP_EMISSION].color);
        shader.SetValue(emissiveColorLoc, floorEmissiveColor);

        floor.Draw(Vector3{0.0f, 0.0f, 0.0f}, 5.0f, WHITE); // Draw floor model

        // Set old car model texture tiling, emissive color and emissive intensity parameters on shader
        shader.SetValue(textureTilingLoc, carTextureTiling);
        Vector4 carEmissiveColor = ColorNormalize(car.GetMaterialMap(0, MATERIAL_MAP_EMISSION).color);
        shader.SetValue(emissiveColorLoc, carEmissiveColor);
        float emissiveIntensity = 0.01f;
        shader.SetValue(emissiveIntensityLoc, &emissiveIntensity, SHADER_UNIFORM_FLOAT);

        car.Draw((Vector3){0.0f, 0.0f, 0.0f}, 0.005f, WHITE); // Draw car model

        // Draw spheres to show the lights positions
        for (const auto& light : lights) {
            ::Color lightColor{
                static_cast<unsigned char>(light.color[0] * 255),
                static_cast<unsigned char>(light.color[1] * 255),
                static_cast<unsigned char>(light.color[2] * 255),
                static_cast<unsigned char>(light.color[3] * 255)};

            if (light.enabled)
                DrawSphereEx(light.position, 0.2f, 8, 8, lightColor);
            else
                DrawSphereWires(light.position, 0.2f, 8, 8, ColorAlpha(lightColor, 0.3f));
        }

        EndMode3D();

        DrawText("Toggle lights: [1][2][3][4]", 10, 40, 20, LIGHTGRAY);

        DrawText(
            "(c) Old Rusty Car model by Renafox (https://skfb.ly/LxRy)",
            screenWidth - 320,
            screenHeight - 20,
            10,
            LIGHTGRAY);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unbind (disconnect) shader from car.material[0]
    // to avoid UnloadMaterial() trying to unload it automatically
    // car.GetMaterials()[0].shader = (Shader){ 0 };
    // UnloadMaterial(car.materials[0]);
    // car.GetMaterials()[0].maps = NULL;
    // UnloadModel(car);

    // floor.GetMaterials()[0].shader = (Shader){ 0 };
    // UnloadMaterial(floor.materials[0]);
    // floor.GetMaterials()[0].maps = NULL;
    // UnloadModel(floor);

    //UnloadShader(shader);       // Unload Shader

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Create light with provided data
// NOTE: It updated the global lightCount and it's limited to MAX_LIGHTS
static Light CreateLight(
    LightType type,
    ::Vector3 position,
    ::Vector3 target,
    ::Color color,
    float intensity,
    raylib::Shader& shader) {
    Light light;
    if (lightCount < MaxLights) {
        light.enabled = 1;
        light.type = type;
        light.position = position;
        light.target = target;
        light.color[0] = (float)color.r / 255.0f;
        light.color[1] = (float)color.g / 255.0f;
        light.color[2] = (float)color.b / 255.0f;
        light.color[3] = (float)color.a / 255.0f;
        light.intensity = intensity;

        // NOTE: Shader parameters names for lights must match the requested ones
        light.enabledLoc = shader.GetLocation(TextFormat("lights[%i].enabled", lightCount));
        light.typeLoc = shader.GetLocation(TextFormat("lights[%i].type", lightCount));
        light.positionLoc = shader.GetLocation(TextFormat("lights[%i].position", lightCount));
        light.targetLoc = shader.GetLocation(TextFormat("lights[%i].target", lightCount));
        light.colorLoc = shader.GetLocation(TextFormat("lights[%i].color", lightCount));
        light.intensityLoc = shader.GetLocation(TextFormat("lights[%i].intensity", lightCount));

        UpdateLight(shader, light);

        lightCount++;
    }

    return light;
}

// Send light properties to shader
// NOTE: Light shader locations should be available
static void UpdateLight(raylib::Shader& shader, const Light& light) {
    shader.SetValue(light.enabledLoc, light.enabled);
    shader.SetValue(light.typeLoc, static_cast<int>(light.type));

    // Send to shader light position values
    std::array<float, 3> position{{light.position.x, light.position.y, light.position.z}};
    shader.SetValue(light.positionLoc, position);

    // Send to shader light target position values
    std::array<float, 3> target{{light.target.x, light.target.y, light.target.z}};
    shader.SetValue(light.targetLoc, target);
    shader.SetValue(light.colorLoc, light.color);
    shader.SetValue(light.intensityLoc, light.intensity);
}
