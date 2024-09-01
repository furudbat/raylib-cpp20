#include <tl/expected.hpp>

#include "raylib-assert.h"
#include "raylib-cpp.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <string>
#include <vector>

TEST_CASE("Image", "[image]") {
    // Get a path to where the executable is so file loading is relative.
    // std::string path = (argc > 0) ? GetDirectoryPath(argv[0]) : 0;

    SECTION("Loading") {
        raylib::Image image;
        REQUIRE(image.Load("resources/feynman.png"));
        REQUIRE(image.IsReady());

        SECTION("Chaining") {
            image.Crop(100, 100).Resize(50, 50);
            REQUIRE(image.GetWidth() == 50);
            REQUIRE(image.GetHeight() == 50);
        }
    }
}

TEST_CASE("Wave", "[audio]") {
    // Get a path to where the executable is so file loading is relative.
    // std::string path = (argc > 0) ? GetDirectoryPath(argv[0]) : 0;

    raylib::Wave wave;
    REQUIRE(wave.Load("resources/weird.wav"));
    REQUIRE(wave.IsReady()); // "Expected wave to be loaded correctly"
}

TEST_CASE("Texture", "[texture]") {
    raylib::Texture texture;
    REQUIRE_FALSE(texture.Load("notfound.png"));
}
