add_subdirectory(raylib)

# use for testing expected in C++20 (expected is an C++23 feature)
cpmaddpackage(
        NAME
        expected
        VERSION
        1.1.0
        GITHUB_REPOSITORY
        TartanLlama/expected
        OPTIONS
        "EXPECTED_BUILD_TESTS OFF"
        "EXPECTED_BUILD_PACKAGE OFF"
)