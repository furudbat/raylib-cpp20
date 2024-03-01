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

/**
 *	Changelog:
 *		- use C++20 filesystem
 *		- use std::string as return type
 *		- change std::map to std::unordered_map
 *      - add RLASContext
 *		- add rlas_IsFileExist, rlas_LoadFile
 *		- change how to add root path a bit
 */


#ifndef RLASSETS_H
#define RLASSETS_H

#if __has_include(<raylib-cpp.hpp>)
#include <raylib-cpp.hpp>
#else
#ifndef RLCPPAPI
#define RLCPPAPI static
#endif
#endif

#include <filesystem>     // for path
#include <memory>         // for shared_ptr
#include <cstddef>       // for size_t
#include <cstdint>       // for uint8_t
#include <string>         // for string, hash, operator==
#include <string_view>
#include <unordered_map>  // for unordered_map
#include <vector>         // for vector
#include <optional>

#ifdef __cplusplus
extern "C" {
#endif
#include <raylib.h>
#ifdef __cplusplus
}
#endif

#ifndef MINIZ_NO_EXCEPTIONS
#define MINIZ_NO_EXCEPTIONS
#endif
#ifndef MINIZ_HEADER_FILE_ONLY
#define MINIZ_HEADER_FILE_ONLY
#endif
#include "zip_file.hpp" // NOLINT

namespace raylib::rlas {

struct AssetMeta {
  std::filesystem::path RelativeName;
  std::filesystem::path PathOnDisk;
  std::shared_ptr<miniz_cpp::zip_file> ArchiveFile;
  miniz_cpp::zip_info ArchiveInfo;
};

using MetaMap = std::unordered_map<std::string, AssetMeta>;
using TempMap = std::unordered_map<std::string, std::filesystem::path>;

struct RLASContext {
  MetaMap AssetMap;
  TempMap TempFiles;

  std::vector<std::filesystem::path> AssetRootPaths;
  std::filesystem::path AssetTempPath;
};

RLASContext &GetRLASContext();

void InitLoadFileCallback();

/// <summary>
/// Gets the application (exe) directory for the currently running program
/// </summary>
/// <returns>The path on disk in the current OSs format</returns>
std::filesystem::path GetApplicationBasePath();

/// <summary>
/// adds a subpath (folder or file) to the specified path with the correct
/// characters for the current OS
/// </summary>
/// <param name="path">the root path</param>
/// <param name="subpath">the sub path to append</param>
std::filesystem::path AppendPath(std::filesystem::path path, std::filesystem::path subpath);

enum class relative_to_app_t : bool { Yes = true, No = false};
/// <summary>
/// Sets the initial asset path on disk
/// </summary>
/// <param name="path">The path to use as the asset root</param>
/// <param name="relativeToApp">When true the specified path will be used
/// relative to the application root and should be in unix (/) format, when
/// false the path specified is in the OSs format</param>
void SetAssetRootPath(std::filesystem::path path, relative_to_app_t relativeToApp);

void AddAssetRootPath(std::filesystem::path path, relative_to_app_t relativeToApp);

/// <summary>
/// Adds zip file as if it was an extracted resource path
/// All files in the zip will be added to the root of the virtual path
/// Any files that are duplicated in resource paths will be 'merged' into the
/// virtual file structure and override older paths
/// </summary>
/// <param name="path">The path of the archive to add</param>
/// <param name="relativeToApp">When true the specified path will be used
/// relative to the application root and should be in unix (/) format, when
/// false the path specified is in the OSs format</param>
void AddAssetResourceArchive(std::filesystem::path path, relative_to_app_t relativeToApp);

/// <summary>
/// Adds an additional asset path to the search path for assets
/// The the specified path will be treated as '/' for relative paths
/// Any files that are duplicated in resource paths will be 'merged' into the
/// virtual file structure and override older paths
/// </summary>
/// <param name="path">The resource path root to add</param>
void AddAssetResourcePath(RLASContext& rlasContext, std::filesystem::path curPath, std::filesystem::path path);

/// <summary>
/// Resets the virtual path system and cleans up any temporary files
/// </summary>
void Cleanup(RLASContext& rlasContext);

/// <summary>
/// Sets the temporary directory used when a file name is requested for assets
/// in an archive
/// </summary>
/// <param name="path">The absolute path to use in OS format</param>
void SetTempPath(RLASContext& rlasContext, std::optional<std::filesystem::path> path);

/// <summary>
/// Returns the top level asset root path
/// </summary>
/// <returns>The path on dis (OS format) of the inital asset root</returns>
std::filesystem::path GetAssetRootPath(const RLASContext& rlasContext);

/// <summary>
/// Gets the path on disk for an assets relative path
/// If multiple resource paths exist with the asset, the one added last will be
/// returned. If the asset is in an archive, and a temp directory is set, the
/// asset will be extracted to the temp folder and that path will be returned
/// Temp files are deleted during cleanup
/// </summary>
/// <param name="path">The relative path of the asset to look up</param>
/// <returns>The path on disk of the asset</returns>
std::filesystem::path GetAssetPath(RLASContext& rlasContext, std::filesystem::path path);

enum class include_sub_directories_t : bool { Yes = true, No = false };
/// <summary>
/// Returns a list of all relative asset names in a resource path
/// Call once with results as NULL to get the count to allocate a result buffer
/// large enough Then call again with buffer to get results.
/// </summary>
/// <param name="path">The relative path to search </param>
/// <param name="includeSubDirectories">Search into subdirectories</param>
/// 
std::vector<std::filesystem::path> GetAssetsInPath(std::filesystem::path path, include_sub_directories_t includeSubDirectories);

/// <summary>
/// Returns true if the asset is part of an archive (zip) file
/// </summary>
/// <param name="path">The relative virtual path to the asset</param>
/// <returns>True if the asset is contained in an archive.</returns>
bool FileIsArchive(const RLASContext& rlasContext, std::filesystem::path path);

/// <summary>
/// Gets the file size of an asset from any source
/// </summary>
/// <param name="path">The relative virtual path to the asset</param>
/// <returns>The file size in bytes</returns>
size_t GetFileSize(const RLASContext& rlasContext, std::filesystem::path path);

bool IsFileExist(const RLASContext& rlasContext, std::filesystem::path path);

std::vector<uint8_t> LoadFile(const RLASContext& rlasContext, const char *fileName);

}

#endif // RLASSETS_H
