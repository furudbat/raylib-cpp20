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

#include "RLAssets.hpp"
#include <cstddef>
#include <raylib.h>     // for FileExists, MemAlloc, SetLoadFileDataCallback
#if __has_include(<raylib-cpp.hpp>)
#include <raylib-cpp.hpp>
#endif

#include <algorithm>    // for replace
#include <cctype>      // for toupper
#include <cstdint>      // for uint8_t
#include <cstdio>      // for fseek, fclose, fopen, ftell, fread, remove, FILE
#include <cstring>     // for memcpy, size_t
#include <filesystem>   // for path, operator/, is_regular_file, current_path
#include <iterator>     // for istream_iterator, operator!=
#include <optional>
#include <ostream>      // for ifstream, ios, streampos, basic_ostream::oper...
#include <string>       // for string, basic_string, allocator
#include <variant>
#include <vector>       // for vector
#include <memory>

namespace fs = std::filesystem;

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <fcntl.h> // For O_BINARY
#include <io.h>    // For _setmode, _fileno, _isatty
#else
#include <unistd.h> // For fileno, isatty
#endif              // OSs

namespace raylib::rlas {

template<class T>
struct RLUniquePtrDeleter {
    void operator()(T* ptr) const noexcept {
        RL_FREE(ptr);
    }
};
template<class T>
using RLUniquePtr = std::unique_ptr<T[], RLUniquePtrDeleter<T>>;

class RLASContextMonostate {
public:
    void operator=(const RLASContext& val) const noexcept {
        value = val;
    }

    operator RLASContext&() const noexcept {
        return value;
    }

private:
    inline static RLASContext value{};
};

/// @NOTE: need global state for raylib LoadBinFile/LoadTextFile
static RLASContextMonostate raylib_rlas_ctx {};

RLASContext &GetRLASContext() {
    return raylib_rlas_ctx;
}

#if __has_include(<raylib-cpp.hpp>)
template<class T>
using owner = raylib::owner<T>;
#else
template<class T>
using owner = T;
#endif

owner<unsigned char *> LoadBinFile(const RLASContext& rlasContext, const char *fileName, int *bytesRead); // FileIO: Load binary data
owner<char *> LoadTextFile(const RLASContext& rlasContext, const char *fileName);                         // FileIO: Load text data

static LoadFileDataCallback RLASLoadFileData = [](const char *fileName, int *bytesRead) noexcept {
    return LoadBinFile(GetRLASContext(), fileName, bytesRead);
};
static LoadFileTextCallback RLASLoadFileText = [](const char *fileName) noexcept {
    return LoadTextFile(GetRLASContext(), fileName);
};

std::string ToMapKey(std::string_view str) {
  if (str.empty()) {
    return {};
  }

  std::string ret (str);
  for (auto &chr: ret) {
    chr = ::toupper(chr);
  }
  std::replace(ret.begin(), ret.end(), '\\', '/');

  return ret;
}

void Cleanup(RLASContext& rlasContext) {
  rlasContext.AssetRootPaths.clear();
  rlasContext.AssetMap.clear();

  for (const auto &[name, file]: rlasContext.TempFiles) {
    remove(file.string().c_str());
  }

  rlasContext.TempFiles.clear();
}

void SetTempPath(RLASContext& rlasContext, std::optional<std::filesystem::path> path) {
  if (!path.has_value()) {
    rlasContext.AssetTempPath.clear();
  } else {
    rlasContext.AssetTempPath = path.value_or("");
  }
}

void InitLoadFileCallback() {
  SetLoadFileDataCallback(RLASLoadFileData);
  SetLoadFileTextCallback(RLASLoadFileText);
}

void SetAssetRootPath(std::filesystem::path path, relative_to_app_t relativeToApp) {
  InitLoadFileCallback();

  GetRLASContext().AssetRootPaths.clear();

  if (relativeToApp == relative_to_app_t::Yes) {
    const std::filesystem::path appPath = GetApplicationBasePath();
    const std::filesystem::path root = appPath / path;
    AddAssetResourcePath(GetRLASContext(), "", root);
  } else {
    AddAssetResourcePath(GetRLASContext(), std::filesystem::current_path(),
                         path);
  }
}

void AddAssetRootPath(std::filesystem::path path, relative_to_app_t relativeToApp) {
  InitLoadFileCallback();

  if (relativeToApp == relative_to_app_t::Yes) {
    const std::filesystem::path appPath = GetApplicationBasePath();
    const std::filesystem::path root = appPath / path;
    AddAssetResourcePath(GetRLASContext(), "", root);
  } else {
    AddAssetResourcePath(GetRLASContext(), std::filesystem::current_path(),
                         path);
  }
}


std::filesystem::path GetAssetRootPath(const RLASContext& rlasContext) {
  if (rlasContext.AssetRootPaths.empty()) {
    return "";
  }
  return rlasContext.AssetRootPaths.front();
}

void AddZipArchive(RLASContext& rlasContext, std::optional<std::string_view> archiveName, std::filesystem::path curPath, std::filesystem::path archivePath) {
  const std::filesystem::path archiveRelPath = (archiveName.has_value())? archivePath / std::filesystem::path(archiveName.value_or("")) : "";

  const auto archiveFullPath = curPath / archivePath;
  const auto archiveFullPathStr = archiveFullPath.string();
  auto archive = std::make_shared<miniz_cpp::zip_file>(archiveFullPathStr);
  for (const auto &info: archive->infolist()) {
    if (info.file_size == 0) {
      continue;
    }

    const std::filesystem::path assetRelPath = archiveRelPath / info.filename;

    const auto assetRelPathStr = assetRelPath.string();
    const std::string upperPath = ToMapKey(assetRelPathStr);

    rlasContext.AssetMap[upperPath] = AssetMeta{
        .RelativeName = assetRelPath,
        .PathOnDisk = curPath / archivePath,
        .ArchiveFile = archive,
        .ArchiveInfo = info,
    };
  }
}

void AddAssetResourceArchive(std::filesystem::path path, relative_to_app_t relativeToApp) {
  InitLoadFileCallback();

  if (relativeToApp == relative_to_app_t::Yes) {
    const std::filesystem::path appPath = GetApplicationBasePath();
    const std::filesystem::path root = appPath / path;
    AddZipArchive(GetRLASContext(), std::nullopt, "", root);
  } else {
    AddZipArchive(GetRLASContext(), std::nullopt, std::filesystem::current_path(), path);
  }
}

void RecurseAddFiles(RLASContext& rlasContext, std::filesystem::path curPath, std::filesystem::path root) {
  for (auto const &dir_entry: std::filesystem::recursive_directory_iterator{curPath / root}) {
    const std::filesystem::path path = std::filesystem::relative(dir_entry, curPath / root);
    const std::filesystem::path relPath = path;
    const std::filesystem::path fullPath = curPath / root / path;

    if (!std::filesystem::is_directory(fullPath) && std::filesystem::is_regular_file(fullPath)) {
      if (path.extension() == ".zip") {
        const auto pathWithoutExt = path.filename().replace_extension("").string();
        AddZipArchive(rlasContext, pathWithoutExt, fullPath, path);
      } else {
        const std::string upperPath = ToMapKey(relPath.string());

        rlasContext.AssetMap[upperPath] = AssetMeta{
            .RelativeName = relPath,
            .PathOnDisk = fullPath,
            .ArchiveFile = nullptr,
        };
      }
    }
  }
}

void AddAssetResourcePath(RLASContext& rlasContext, std::filesystem::path curPath, std::filesystem::path path) {
  if (path.empty()) {
    return;
  }

  rlasContext.AssetRootPaths.emplace_back(path);
  RecurseAddFiles(rlasContext, curPath, path);
}


std::filesystem::path GetAssetPath(RLASContext& rlasContext, std::filesystem::path path) {
  const std::string upperPath = ToMapKey(path.string());
  auto itr = rlasContext.AssetMap.find(upperPath);
  if (itr == rlasContext.AssetMap.end()) {
    return "";
  }

  if (itr->second.ArchiveFile != nullptr) {
    if (rlasContext.TempFiles.find(upperPath) == rlasContext.TempFiles.end()) {
      if (rlasContext.AssetTempPath.empty()) { // no place to extract, return null
        return "";
      }

      std::string tempName = itr->second.RelativeName.string();
      std::replace(tempName.begin(), tempName.end(), '/', '_');
      const auto newTempName = rlasContext.AssetTempPath / tempName;

      std::fstream stream(newTempName, std::ios::binary | std::ios::out);
      stream << itr->second.ArchiveFile->open(itr->second.ArchiveInfo).rdbuf();

      rlasContext.TempFiles[upperPath] = newTempName;
    }

    return rlasContext.TempFiles[upperPath];
  }

  return itr->second.PathOnDisk;
}

std::filesystem::path AppendPath(std::filesystem::path path, std::filesystem::path subpath) {
  return path / subpath;
}

std::vector<std::filesystem::path> GetAssetsInPath(const RLASContext& rlasContext, std::filesystem::path path, include_sub_directories_t includeSubDirectories) {
  const std::string upperPath = ToMapKey(path.string());

  std::vector<std::filesystem::path> ret;
  for (const auto &[key, meta]: rlasContext.AssetMap) {
    if (key.rfind(upperPath) == 0) {
      if (std::filesystem::is_regular_file(meta.RelativeName) || includeSubDirectories == include_sub_directories_t::Yes) {
        ret.push_back(meta.RelativeName);
      }
    }
  }

  return ret;
}

bool FileIsArchive(const RLASContext& rlasContext, std::filesystem::path path) {
  const auto itr = rlasContext.AssetMap.find(ToMapKey(path.string()));
  if (itr != rlasContext.AssetMap.cend()) {
    return itr->second.ArchiveFile != nullptr;
  }
  return false;
}

/// @TODO(C++23): use std::expected for error handling
enum class ReadFileContentsError : uint8_t {
    StandardFileIONotSupported,     ///< FILEIO: Standard file io not supported, use custom file callback
    InvalidFilename,                ///< FILEIO: File name provided is not valid
    FilePartiallyLoaded,            ///< FILEIO: [%s] File partially loaded (%i bytes out of %i)
    FileIsBiggerThanInt32Max,       ///< FILEIO: [%s] File is bigger than 2147483647 bytes, avoid using LoadFileData()
    FailedToAllocatedMemory,        ///< FILEIO: [%s] Failed to allocated memory for file reading
    FailedToReadFile,               ///< FILEIO: [%s] Failed to read file
    FailedToOpenFile,               ///< FILEIO: [%s] Failed to open file
    FileNotFound,
};
using ReadFileContentsResultDataBinary = RLUniquePtr<unsigned char>;
using ReadFileContentsResultDataText = RLUniquePtr<char>;
using ReadFileContentsResultData = std::variant<std::nullptr_t, ReadFileContentsResultDataBinary, ReadFileContentsResultDataText>;
struct ReadFileContentsResult {
    ReadFileContentsResultData data{nullptr};
    size_t bytesRead{0};
    std::optional<ReadFileContentsError> error{std::nullopt};
};
enum class read_file_content_modes_t : bool { text = false, binary = true };
ReadFileContentsResult ReadFileContents(const char *fileName, read_file_content_modes_t mode) {
    ReadFileContentsResult ret;

    if (fileName != nullptr) {
#if defined(SUPPORT_STANDARD_FILEIO)
#ifdef _WIN32
        FILE* file = nullptr;
        fopen_s(&file, fileName, mode == read_file_content_modes_t::binary ? "rb" : "rt");
#else
        FILE *file = fopen(fileName, mode == read_file_content_modes_t::binary ? "rb" : "rt");
#endif

        if (file != nullptr) {
            const auto size = [&](){
                size_t ret{0};
                fseek(file, 0, SEEK_END);
                switch(mode) {
                    case read_file_content_modes_t::binary:
                    // WARNING: On binary streams SEEK_END could not be found,
                    // using fseek() and ftell() could not work in some (rare) cases
                    ret = ftell(file);     // WARNING: ftell() returns 'long int', maximum size returned is INT_MAX (2147483647 bytes)
                    break;
                    case read_file_content_modes_t::text:
                    // WARNING: When reading a file as 'text' file,
                    // text mode causes carriage return-linefeed translation...
                    // ...but using fseek() should return correct byte-offset
                    ret = (unsigned int)ftell(file);
                }
                fseek(file, 0, SEEK_SET);
                return ret;
            }();

            if (size > 0) {
                const auto GetBinaryData = [&]() -> ReadFileContentsResultDataBinary {
                    auto data = ReadFileContentsResultDataBinary((unsigned char *)RL_MALLOC(size*sizeof(unsigned char)));
                    if (data != nullptr) {
                        // NOTE: fread() returns number of read elements instead of bytes, so we read [1 byte, size elements]
                        auto count = fread(data.get(), sizeof(unsigned char), size, file);

                        // WARNING: fread() returns a size_t value, usually 'unsigned int' (32bit compilation) and 'unsigned long long' (64bit compilation)
                        // dataSize is unified along raylib as a 'int' type, so, for file-sizes > INT_MAX (2147483647 bytes) we have a limitation
                        if (count > 2147483647L) {
                            TraceLog(LOG_WARNING, "FILEIO: [%s] File is bigger than 2147483647 bytes, avoid using LoadFileData()", fileName);
                            ret.error = ReadFileContentsError::FileIsBiggerThanInt32Max;
                            return nullptr;
                        } else {
                            ret.bytesRead = count;
                            if (ret.bytesRead != size) {
                                TraceLog(LOG_WARNING, "FILEIO: [%s] File partially loaded (%i bytes out of %i)", fileName, ret.bytesRead, count);
                                ret.error = ReadFileContentsError::FilePartiallyLoaded;
                            } else {
                                TraceLog(LOG_INFO, "FILEIO: [%s] File loaded successfully", fileName);
                                ret.error = std::nullopt;
                            }
                        }
                    } else {
                        TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to allocated memory for file reading", fileName);
                        ret.error = ReadFileContentsError::FailedToAllocatedMemory;
                    }
                    return data;
                };
                const auto GetTextData = [&]() -> ReadFileContentsResultDataText {
                    auto data = ReadFileContentsResultDataText((char *)RL_MALLOC((size+1)*sizeof(char)));
                    if (data != nullptr) {
                        const auto count = (unsigned int)fread(data.get(), sizeof(char), size, file);

                        // WARNING: \r\n is converted to \n on reading, so,
                        // read bytes count gets reduced by the number of lines
                        if (count < size) {
                            data = ReadFileContentsResultDataText((char*)RL_REALLOC(data.release(), count + 1));
                        }

                        ret.bytesRead = count;

                        // Zero-terminate the string
                        data[count] = '\0';

                        TraceLog(LOG_INFO, "FILEIO: [%s] Text file loaded successfully", fileName);
                    } else {
                        TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to allocated memory for file reading", fileName);
                        ret.error = ReadFileContentsError::FailedToAllocatedMemory;
                    }
                    return data;
                };
                ret.data = (mode == read_file_content_modes_t::binary) ? ReadFileContentsResultData{GetBinaryData()} : ReadFileContentsResultData{GetTextData()};
            } else {
                TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to read file", fileName);
                ret.error = ReadFileContentsError::FailedToReadFile;
            }

            fclose(file);
        } else {
            TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to open file", fileName);
            ret.error = ReadFileContentsError::FailedToOpenFile;
        }
#else
        TraceLog(LOG_WARNING, "FILEIO: Standard file io not supported");
        ret.error = ReadFileContentsError::StandardFileIONotSupported;
#endif
    } else {
        TraceLog(LOG_WARNING, "FILEIO: File name provided is not valid");
        ret.error = ReadFileContentsError::InvalidFilename;
    }

    return ret;
}


bool IsFileExist(const RLASContext& rlasContext, std::filesystem::path path) {
  const std::string pathStr = path.string();
  const auto itr = rlasContext.AssetMap.find(ToMapKey(pathStr));
  return itr != rlasContext.AssetMap.cend() || FileExists(pathStr.c_str());
}

size_t GetFileSize(const RLASContext& rlasContext, std::filesystem::path path) {
  const auto itr = rlasContext.AssetMap.find(ToMapKey(path.string()));
  if (itr == rlasContext.AssetMap.cend()) {
    return 0;
  }

  if (itr->second.ArchiveFile != nullptr) {
    return (unsigned int) itr->second.ArchiveInfo.file_size;
  }

  size_t size = 0;

  if (!itr->second.PathOnDisk.empty()) {
#ifdef _WIN32
    FILE* file = nullptr;
    fopen_s(&file, itr->second.PathOnDisk.string().c_str(), "rb");
#else
    FILE *file = fopen(itr->second.PathOnDisk.c_str(), "rb");
#endif //_WIN32

    if (file != nullptr) {
      // WARNING: On binary streams SEEK_END could not be found,
      // using fseek() and ftell() could not work in some (rare) cases
      fseek(file, 0, SEEK_END);
      size = ftell(file);
      fseek(file, 0, SEEK_SET);
      fclose(file);
    }
  }

  return size;
}


ReadFileContentsResult LoadBinFilePP(const RLASContext& rlasContext, const char *fileName) {
  const auto itr = rlasContext.AssetMap.find(ToMapKey(fileName));
  if (itr == rlasContext.AssetMap.cend()) {
    if (FileExists(fileName)) {
      return ReadFileContents(fileName, read_file_content_modes_t::binary);
    }
    return ReadFileContentsResult{ .error = ReadFileContentsError::FileNotFound };
  }

  ReadFileContentsResult ret;
  if (itr->second.ArchiveFile != nullptr) {
    ret.bytesRead = static_cast<unsigned int>(itr->second.ArchiveInfo.file_size);
    ret.data = ReadFileContentsResultData{ReadFileContentsResultDataBinary(static_cast<unsigned char *>(RL_MALLOC((unsigned int) itr->second.ArchiveInfo.file_size)))};
    std::visit([&](auto&& data) {
        using T = std::decay_t<decltype(data)>;
        if constexpr (std::is_same_v<T, ReadFileContentsResultDataBinary>) {
            itr->second.ArchiveFile->readBin(itr->second.ArchiveInfo, data.get());
        }
    }, ret.data);
    return ret;
  }

  return ReadFileContents(itr->second.PathOnDisk.string().c_str(), read_file_content_modes_t::binary);
}
owner<unsigned char *> LoadBinFile(const RLASContext& rlasContext, const char *fileName, int *bytesRead) {
    auto res = LoadBinFilePP(rlasContext, fileName);
    if (bytesRead != nullptr) { *bytesRead = static_cast<int>(res.bytesRead); }
    if (!res.error) {
        return std::visit([&](auto&& data) -> owner<unsigned char *> {
            using T = std::decay_t<decltype(data)>;
            if constexpr (std::is_same_v<T, ReadFileContentsResultDataBinary>) {
                return data.release();
            }
            return nullptr;
        }, res.data);
    }
    return nullptr;
}

ReadFileContentsResult LoadTextFilePP(const RLASContext& rlasContext, const char *fileName) {
  const auto itr = rlasContext.AssetMap.find(ToMapKey(fileName));
  if (itr == rlasContext.AssetMap.cend()) {
    if (FileExists(fileName)) {
      return ReadFileContents(fileName, read_file_content_modes_t::text);
    }
    return ReadFileContentsResult{ .error = ReadFileContentsError::FileNotFound };
  }

  if (itr->second.ArchiveFile != nullptr) {
    std::string data = itr->second.ArchiveFile->read(itr->second.ArchiveInfo);
    auto buffer = ReadFileContentsResultDataText ((char*)RL_MALLOC((unsigned int) data.size() + 1));
    std::memcpy(buffer.get(), data.c_str(), data.size());
    buffer[data.size()] = '\0';

    return ReadFileContentsResult{ .data = std::move(buffer), .bytesRead = data.size() };
  }

  return ReadFileContents(itr->second.PathOnDisk.string().c_str(), read_file_content_modes_t::text);
}
owner<char *> LoadTextFile(const RLASContext& rlasContext, const char *fileName) {
    auto res = LoadTextFilePP(rlasContext, fileName);
    if (!res.error) {
        return std::visit([&](auto&& data) -> owner<char *> {
            using T = std::decay_t<decltype(data)>;
            if constexpr (std::is_same_v<T, ReadFileContentsResultDataText>) {
                return data.release();
            }
            return nullptr;
        }, res.data);
    }
    return nullptr;
}

std::vector<uint8_t> readFilePP(const char *filename) {
  std::ifstream file(filename, std::ios::binary);

  // Stop eating new lines in binary mode!!!
  file.unsetf(std::ios::skipws);

  std::streampos fileSize;

  file.seekg(0, std::ios::end);
  fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<uint8_t> vec;
  vec.reserve(fileSize);

  vec.insert(vec.begin(), std::istream_iterator<uint8_t>(file), std::istream_iterator<uint8_t>());

  return vec;
}

std::vector<uint8_t> LoadFile(const RLASContext& rlasContext, const char *fileName) {
  std::vector<uint8_t> ret;
  std::filesystem::path filename = fileName;

  const auto itr = rlasContext.AssetMap.find(ToMapKey(fileName));
  if (itr == rlasContext.AssetMap.cend()) {
    if (std::filesystem::exists(filename) && std::filesystem::is_regular_file(filename)) {
      return readFilePP(fileName);
    }
    return ret;
  }

  if (itr->second.ArchiveFile != nullptr) {
    ret.resize(itr->second.ArchiveInfo.file_size);
    itr->second.ArchiveFile->readBin(itr->second.ArchiveInfo, ret.data());
    return ret;
  }

  if (std::filesystem::exists(itr->second.PathOnDisk) && std::filesystem::is_regular_file(itr->second.PathOnDisk)) {
    return readFilePP(itr->second.PathOnDisk.string().c_str());
  }

  return ret;
}

}