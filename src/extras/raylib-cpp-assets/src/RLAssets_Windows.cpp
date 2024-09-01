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
#if defined(_WIN32)
// To avoid conflicting windows.h symbols with raylib, some flags are defined
// WARNING: Those flags avoid inclusion of some Win32 headers that could be required
// by user at some point and won't be included...
//-------------------------------------------------------------------------------------

// If defined, the following flags inhibit definition of the indicated items.
#define NOGDICAPMASKS // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#define NOVIRTUALKEYCODES // VK_*
#define NOWINMESSAGES // WM_*, EM_*, LB_*, CB_*
#define NOWINSTYLES // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#define NOSYSMETRICS // SM_*
#define NOMENUS // MF_*
#define NOICONS // IDI_*
#define NOKEYSTATES // MK_*
#define NOSYSCOMMANDS // SC_*
#define NORASTEROPS // Binary and Tertiary raster ops
#define NOSHOWWINDOW // SW_*
#define OEMRESOURCE // OEM Resource values
#define NOATOM // Atom Manager routines
#define NOCLIPBOARD // Clipboard routines
#define NOCOLOR // Screen colors
#define NOCTLMGR // Control and Dialog routines
#define NODRAWTEXT // DrawText() and DT_*
#define NOGDI // All GDI defines and routines
#define NOKERNEL // All KERNEL defines and routines
#define NOUSER // All USER defines and routines
//#define NONLS             // All NLS defines and routines
#define NOMB // MB_* and MessageBox()
#define NOMEMMGR // GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE // typedef METAFILEPICT
#define NOMINMAX // Macros min(a,b) and max(a,b)
#define NOMSG // typedef MSG and associated routines
#define NOOPENFILE // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL // SB_* and scrolling routines
#define NOSERVICE // All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND // Sound driver routines
#define NOTEXTMETRIC // typedef TEXTMETRIC and associated routines
#define NOWH // SetWindowsHook and WH_*
#define NOWINOFFSETS // GWL_*, GCL_*, associated routines
#define NOCOMM // COMM driver routines
#define NOKANJI // Kanji support stuff.
#define NOHELP // Help engine interface.
#define NOPROFILER // Profiler interface.
#define NODEFERWINDOWPOS // DeferWindowPos routines
#define NOMCX // Modem Configuration Extensions

// Type required before windows.h inclusion
typedef struct tagMSG* LPMSG;

#include <windows.h>
#endif


namespace raylib::rlas {

constexpr char PathDelim = '\\';

std::filesystem::path GetApplicationBasePath() {
    static const std::filesystem::path appDir = []()
    {
        std::filesystem::path appDir = "/"; // default for everyone to start out with

        typedef DWORD(WINAPI * GetModuleFileNameFunc)(HANDLE, HMODULE, LPSTR, DWORD);
        GetModuleFileNameFunc getModuleFileNameExWPtr = nullptr;
#ifdef UNICODE
        HMODULE lib = LoadLibrary(L"psapi.dll");
#else
        HMODULE lib = LoadLibrary("psapi.dll");
#endif
        if (lib == nullptr) {
            appDir = "\\";
        }
        else {
            getModuleFileNameExWPtr = (GetModuleFileNameFunc)GetProcAddress(lib, "GetModuleFileNameExA");

            if (getModuleFileNameExWPtr == nullptr) {
                appDir = "\\";
            }
            else {
                std::array<CHAR, MAX_PATH> path;
                const int len = getModuleFileNameExWPtr(GetCurrentProcess(), nullptr, path.data(), path.size());
                if (len > 0) {
                    for (int i = len; i >= 0; --i) {
                        if (path[i] == '\\') {
                            path[i + 1] = '\0';
                            i = -1;
                        }
                    }
                    appDir = std::string_view{path.data(), path.size()};
                }
            }
            FreeLibrary(lib);
        }
        return appDir;
    }();

    return appDir;
}

} // namespace raylib::rlas