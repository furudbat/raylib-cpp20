#include "raylib-cpp.hpp"
#include "raylib-assert.h"
#include <string>
#include <vector>
#include <filesystem>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

/// @TODO: split into more test cases
TEST_CASE( "Vector", "[core]" ) {
    raylib::Vector2 position({.x = 50, .y = 100});
    REQUIRE(position.GetX() == position.x);
    position.x = 150;
    REQUIRE(position.GetX() == 150);
    position.SetX(300);
    REQUIRE(position.GetX() == 300);

    raylib::Vector2 speed{{.x = 10, .y = 10}};
    position += speed;
    REQUIRE(position.x == 310);
    REQUIRE(raylib::Window::IsReady() == false);

    raylib::Vector2 size{{.x = 50, .y = 100}};
    raylib::Vector2 halfsize = size / 2.0F;

    REQUIRE(size.x == 50);
    REQUIRE(size.y == 100);
    REQUIRE(halfsize.x == 25);
    REQUIRE(halfsize.y == 50);

    raylib::Vector2 doublesize = size * 2.0F;
    REQUIRE(size.x == 50);
    REQUIRE(doublesize.x == 100);
}

TEST_CASE( "Color", "[core]" ) {
    raylib::Color color {RED};
    REQUIRE(color.ToInt() == ::ColorToInt(RED));

    color = RAYWHITE;
    auto raylibColor = RAYWHITE;
    REQUIRE(color.r == raylibColor.r);
}

TEST_CASE( "Math", "[core]" ) {
    raylib::Vector2 direction({.x = 50, .y = 50});
    raylib::Vector2 newDirection = direction.Rotate(30);
    REQUIRE(static_cast<int>(newDirection.x) == 57);
}

TEST_CASE( "Image", "[image]" ) {
    // Get a path to where the executable is so file loading is relative.
    //std::string path = (argc > 0) ? GetDirectoryPath(argv[0]) : 0;

    SECTION("Loading") {
        raylib::Image image("resources/feynman.png");
        REQUIRE(image.IsReady());

        SECTION("Chaining") {
            image.Crop(100, 100)
                .Resize(50, 50);
            REQUIRE(image.GetWidth() == 50);
            REQUIRE(image.GetHeight() == 50);
        }
    }
}

TEST_CASE( "Keyboard", "[core]" ) {
    REQUIRE(!raylib::Keyboard::IsKeyPressed(KEY_MINUS));
}


TEST_CASE( "raylib::LoadDirectoryFiles", "[core][raylib-cpp]" ) {
    auto files = raylib::LoadDirectoryFiles(::GetWorkingDirectory());
    REQUIRE(files.size() > 3);
}

TEST_CASE( "raylib::TextReplace", "[core][raylib-cpp]" ) {
    std::string input = "Hello World!";
    std::string output = raylib::TextReplace(input, "World", "Moon");
    REQUIRE(output == "Hello Moon!");
}

TEST_CASE( "raylib::TextInsert", "[core][raylib-cpp]" ) {
    std::string input = "Hello World!";
    std::string output = raylib::TextInsert(input, "Good!", 0);
    REQUIRE(output == "Good! World!");
}

TEST_CASE( "raylib::TextSubtext", "[core][raylib-cpp]" ) {
    std::string input = "Hello World!";
    std::string output = raylib::TextSubtext(input, 6, 5);
    REQUIRE(output == "World");
}

TEST_CASE( "raylib::TextSplit", "[core][raylib-cpp]" ) {
    std::vector<std::string> output = raylib::TextSplit("Hello|How|Are|You", '|');
    REQUIRE(output.size() == 4);
    REQUIRE(output[1] == "How");
}

TEST_CASE( "Wave", "[audio]" ) {
    // Get a path to where the executable is so file loading is relative.
    //std::string path = (argc > 0) ? GetDirectoryPath(argv[0]) : 0;

    raylib::Wave wave("resources/weird.wav");
    REQUIRE(wave.IsReady()); // "Expected wave to be loaded correctly"
}

TEST_CASE( "Texture", "[texture]" ) {
    REQUIRE_THROWS([](){
        raylib::Texture texture("notfound.png");
    }());
}

TEST_CASE( "Load FileData", "[file]" ) {
    raylib::FileData file ("resources/weird.wav");
    REQUIRE(file.GetBytesRead() > 0); // "Expected file to be loaded correctly"
}

TEST_CASE( "Load FileText", "[file]" ) {
    raylib::FileText file ("resources/lorem.txt");
    REQUIRE(file.GetLength() > 0); // "Expected file to be loaded correctly"
    REQUIRE(file.ToStringView().substr(0, 5) == "Lorem");
}