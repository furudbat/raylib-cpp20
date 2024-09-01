/**********************************************************************************************
 *
 *   raylibExtras * Utilities and Shared Components for Raylib
 *
 *   RLAssets * Simple Asset Managment System for Raylib
 *
 *   LICENSE: MIT
 *
 *   Copyright (c) 2020 Jeffery Myers
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a
 *   copy of this software and associated documentation files (the "Software"),
 *   to deal in the Software without restriction, including without limitation the
 *   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *   sell copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *   DEALINGS IN THE SOFTWARE.
 *
 **********************************************************************************************/

#include <array>
#include <cstdint> // for uint32_t
#include <filesystem> // for path
#include <string> // for allocator, string

#include <mach-o/dyld.h>
#include <sys/syslimits.h>

namespace raylib::rlas {

constexpr char PathDelim = '/';

std::filesystem::path GetApplicationBasePath() {
    static const std::filesystem::path appDir = []()
    {
        std::filesystem::path appDir = "/"; // default for everyone to start out with

        std::array<char, PATH_MAX + 1> path;
        uint32_t size = path.size();

        if (_NSGetExecutablePath(path, &size) == 0) {
            uint32_t len = strlen(path);
            for (uint32_t i = len; i >= 0; i--) {
                if (path[i] == '/' && path[i + 1] == '.') {
                    path[i + 1] = 0;
                    break;
                }
            }
            appDir = path;
        }
    }();

    return appDir;
}

} // namespace raylib::rlas