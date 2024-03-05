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
class Shader {
 public:
    constexpr Shader() : m_data{NullShader} {}

    constexpr explicit Shader(owner<const ::Shader&> shader) = delete;
    constexpr explicit Shader(owner<::Shader&&> shader) {
        set(shader);

        shader.id = 0;
        shader.locs = nullptr;
    }

    constexpr Shader(owner<unsigned int> id, owner<int*> locs = nullptr) : m_data{id, locs} {}

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
        set(other.m_data);

        other.m_data.id = 0;
        other.m_data.locs = nullptr;
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

    GETTERSETTER(unsigned int, Id, m_data.id)
    CONST_GETTER(int*, Locs, m_data.locs)
    //SPAN_GETTER(int, Locs, locs, RL_MAX_SHADER_LOCATIONS)

    constexpr Shader& operator=(owner<const ::Shader&> shader) = delete;
    constexpr Shader& operator=(owner<::Shader&&> shader) {
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
        set(other.m_data);

        other.m_data.id = 0;
        other.m_data.locs = nullptr;

        return *this;
    }

    /**
     * Unload shader from GPU memory (VRAM)
     */
    ~Shader() {
        Unload();
    }

    explicit operator ::Shader() const {
        return m_data;
    }
    [[nodiscard]] ::Shader c_raylib() const & {
        return m_data;
    }

    /**
     * Unload shader from GPU memory (VRAM)
     */
    void Unload() {
        if (m_data.locs != nullptr) {
            ::UnloadShader(m_data);
            m_data.id = 0;
            m_data.locs = nullptr;
        }
    }

    /**
     * Begin custom shader drawing.
     */
    Shader& BeginMode() {
        ::BeginShaderMode(m_data);
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
        return ::GetShaderLocation(m_data, uniformName.c_str());
    }

    /**
     * Get shader attribute location
     *
     * @see GetShaderLocationAttrib()
     */
    [[nodiscard]] int GetLocationAttrib(const std::string& attribName) const {
        return ::GetShaderLocationAttrib(m_data, attribName.c_str());
    }

    /**
     * Set shader uniform value
     *
     * @see SetShaderValue()
     */
    Shader& SetValue(int uniformLoc, const void* value, int uniformType) {
        ::SetShaderValue(m_data, uniformLoc, value, uniformType);
        return *this;
    }

    /**
     * Set shader uniform value vector
     *
     * @see SetShaderValueV()
     */
    Shader& SetValue(int uniformLoc, const void* value, int uniformType, int count) {
        ::SetShaderValueV(m_data, uniformLoc, value, uniformType, count);
        return *this;
    }

    /**
     * Set shader uniform value (matrix 4x4)
     *
     * @see SetShaderValueMatrix()
     */
    Shader& SetValue(int uniformLoc, const ::Matrix& mat) {
        ::SetShaderValueMatrix(m_data, uniformLoc, mat);
        return *this;
    }

    /**
     * Set shader uniform value for texture
     *
     * @see SetShaderValueTexture()
     */
    Shader& SetValue(int uniformLoc, const ::Texture2D& texture) {
        ::SetShaderValueTexture(m_data, uniformLoc, texture);
        return *this;
    }

    /**
     * Retrieves whether or not the shader is ready.
     */
    [[nodiscard]] constexpr bool IsReady() const {
        return m_data.id != 0 && m_data.locs != nullptr;
    }

 protected:
    constexpr void set(const ::Shader& shader) {
        m_data.id = shader.id;
        m_data.locs = shader.locs;
    }

    ::Shader m_data;
};

}  // namespace raylib

using RShader = raylib::Shader;

#endif  // RAYLIB_CPP_INCLUDE_SHADER_HPP_
