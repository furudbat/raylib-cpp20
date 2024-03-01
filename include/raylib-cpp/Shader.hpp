#ifndef RAYLIB_CPP_INCLUDE_SHADER_HPP_
#define RAYLIB_CPP_INCLUDE_SHADER_HPP_

#include <string>
#include <filesystem>
#include <optional>
#include <utility>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "Texture.hpp"

namespace raylib {

/**
 * Shader type (generic)
 */
class Shader : public ::Shader {
 public:
    constexpr Shader() : ::Shader{NullShader} {}

    constexpr explicit Shader(const ::Shader& shader) = delete;
    constexpr explicit Shader(::Shader&& shader) {
        set(shader);

        shader.id = 0;
        shader.locs = nullptr;
    }

    constexpr Shader(owner<unsigned int> _id, owner<int*> _locs = nullptr) : ::Shader{_id, _locs} {}

    struct LoadShaderOptions {
        std::optional<std::filesystem::path> vsFileName;
        std::optional<std::filesystem::path> fsFileName;
    };
    explicit Shader(LoadShaderOptions options) {
        set(::LoadShader(options.vsFileName ? options.vsFileName.value().c_str() : nullptr, options.fsFileName ? options.fsFileName.value().c_str() : nullptr));
    }

    struct LoadShaderOptionsC {
        const char* vsFileName{nullptr};
        const char* fsFileName{nullptr};
    };
    explicit Shader(LoadShaderOptionsC options) {
        set(::LoadShader(options.vsFileName, options.fsFileName));
    }

    explicit constexpr Shader(const Shader&) = delete;
    explicit constexpr Shader(Shader&& other) {
        set(other);

        other.id = 0;
        other.locs = nullptr;
    }

    /**
     * Load shader from files and bind default locations.
     *
     * @see ::LoadShader
     */
    static Shader Load(LoadShaderOptions options) {
        return Shader(std::move(options));
    }

    static Shader Load(LoadShaderOptionsC options) {
        return Shader(options);
    }

    /**
     * Load a shader from memory.
     *
     * @see ::LoadShaderFromMemory
     */
    struct LoadFromMemoryOptions {
        const std::string& vsCode;
        const std::string& fsCode;
    };
    static Shader LoadFromMemory(LoadFromMemoryOptions options) {
        return Shader{::LoadShaderFromMemory(options.vsCode.c_str(), options.fsCode.c_str())};
    }

    struct LoadFromMemoryOptionsC {
        const char* vsCode;
        const char* fsCode;
    };
    static Shader LoadFromMemory(LoadFromMemoryOptionsC options) {
        return Shader{::LoadShaderFromMemory(options.vsCode, options.fsCode)};
    }

    GETTERSETTER(unsigned int, Id, id)
    GETTERSETTER(int*, Locs, locs)

    constexpr Shader& operator=(const ::Shader& shader) = delete;
    constexpr Shader& operator=(::Shader&& shader) {
        set(shader);

        shader.id = 0;
        shader.locs = nullptr;

        return *this;
    }

    constexpr Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other);

        other.id = 0;
        other.locs = nullptr;

        return *this;
    }

    /**
     * Unload shader from GPU memory (VRAM)
     */
    ~Shader() {
        Unload();
    }

    /**
     * Unload shader from GPU memory (VRAM)
     */
    void Unload() {
        if (locs != nullptr) {
            ::UnloadShader(*this);
            id = 0;
            locs = nullptr;
        }
    }

    /**
     * Begin custom shader drawing.
     */
    Shader& BeginMode() {
        ::BeginShaderMode(*this);
        return *this;
    }

    /**
     * End custom shader drawing (use default shader).
     */
    Shader& EndMode() {
        ::EndShaderMode();
        return *this;
    }

    /**
     * Get shader uniform location
     *
     * @see GetShaderLocation()
     */
    [[nodiscard]] int GetLocation(const std::string& uniformName) const {
        return ::GetShaderLocation(*this, uniformName.c_str());
    }

    /**
     * Get shader attribute location
     *
     * @see GetShaderLocationAttrib()
     */
    [[nodiscard]] int GetLocationAttrib(const std::string& attribName) const {
        return ::GetShaderLocationAttrib(*this, attribName.c_str());
    }

    /**
     * Set shader uniform value
     *
     * @see SetShaderValue()
     */
    Shader& SetValue(int uniformLoc, const void* value, int uniformType) {
        ::SetShaderValue(*this, uniformLoc, value, uniformType);
        return *this;
    }

    /**
     * Set shader uniform value vector
     *
     * @see SetShaderValueV()
     */
    Shader& SetValue(int uniformLoc, const void* value, int uniformType, int count) {
        ::SetShaderValueV(*this, uniformLoc, value, uniformType, count);
        return *this;
    }

    /**
     * Set shader uniform value (matrix 4x4)
     *
     * @see SetShaderValueMatrix()
     */
    Shader& SetValue(int uniformLoc, const ::Matrix& mat) {
        ::SetShaderValueMatrix(*this, uniformLoc, mat);
        return *this;
    }

    /**
     * Set shader uniform value for texture
     *
     * @see SetShaderValueTexture()
     */
    Shader& SetValue(int uniformLoc, const ::Texture2D& texture) {
        ::SetShaderValueTexture(*this, uniformLoc, texture);
        return *this;
    }

    /**
     * Retrieves whether or not the shader is ready.
     */
    [[nodiscard]] constexpr bool IsReady() const {
        return id != 0 && locs != nullptr;
    }

 protected:
    constexpr void set(const ::Shader& shader) {
        id = shader.id;
        locs = shader.locs;
    }
};

}  // namespace raylib

using RShader = raylib::Shader;

#endif  // RAYLIB_CPP_INCLUDE_SHADER_HPP_
