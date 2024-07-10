add_subdirectory(raylib)

if (BUILD_RAYLIB_CPP_EXAMPLES OR BUILD_RAYLIB_CPP_TESTS OR BUILD_RAYLIB_CPP_EXTRAS)
    cpmaddpackage(
            NAME
            GSL
            VERSION
            4.0.0
            GITHUB_REPOSITORY
            microsoft/GSL
            OPTIONS
            "GSL_TEST OFF"
            "GSL_INSTALL OFF"
    )
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
endif()