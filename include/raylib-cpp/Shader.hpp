#ifndef RAYLIB_CPP_INCLUDE_SHADER_HPP_
#define RAYLIB_CPP_INCLUDE_SHADER_HPP_

#include <string>
#include <filesystem>
#include <optional>
#include <utility>
#include <variant>

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
    constexpr explicit Shader(owner<::Shader&&> shader) noexcept {
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
    explicit constexpr Shader(Shader&& other) noexcept {
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
    constexpr Shader& operator=(owner<::Shader&&> shader) noexcept {
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

    explicit operator ::Shader() const noexcept {
        return m_data;
    }
    [[nodiscard]] ::Shader c_raylib() const & noexcept {
        return m_data;
    }

    /**
     * Unload shader from GPU memory (VRAM)
     */
    void Unload() noexcept {
        if (m_data.locs != nullptr) {
            ::UnloadShader(m_data);
            m_data.id = 0;
            m_data.locs = nullptr;
        }
    }

    /**
     * Begin custom shader drawing.
     */
    Shader& BeginMode() noexcept {
        ::BeginShaderMode(m_data);
        return *this;
    }

    /**
     * End custom shader drawing (use default shader).
     */
    Shader& EndMode() noexcept {
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
    [[deprecated("Use SetValue<T>(...) or SetValue(..., variant)")]]
    Shader& SetValue(int uniformLoc, const void* value, int uniformType) {
        ::SetShaderValue(m_data, uniformLoc, value, uniformType);
        return *this;
    }
    template<typename T>
    /// @TODO: requires T can only be vec, ivec, int, float, ... (SHADER_UNIFORM)
    Shader& SetValue(int uniformLoc, const T& value, int uniformType) {
        ::SetShaderValue(m_data, uniformLoc, &value, uniformType);
        return *this;
    }
    Shader& SetValue(int uniformLoc, std::variant<float,
                     std::array<float, 2>, ///< vec2 (2 float)
                     std::array<float, 3>, ///< vec3 (3 float)
                     std::array<float, 4>, ///< vec4 (4 float)
                     ::Vector2,            ///< vec2 (2 float)
                     ::Vector3,            ///< vec3 (3 float)
                     ::Vector4,            ///< vec4 (4 float)
                     int,
                     std::array<int, 2>,   ///< ivec2 (2 int)
                     std::array<int, 3>,   ///< ivec3 (3 int)
                     std::array<int, 4>,   ///< ivec4 (4 int)
                     ::Texture2D> value) {
        std::visit([&](auto&& val) {
           using T = std::decay_t<decltype(val)>;
           if constexpr (std::is_same_v<T, float>)
               ::SetShaderValue(m_data, uniformLoc, &val, SHADER_UNIFORM_FLOAT);
           else if constexpr (std::is_same_v<T, ::Vector2> || std::is_same_v<T, std::array<float, 2>>)
               ::SetShaderValue(m_data, uniformLoc, &val, SHADER_UNIFORM_VEC2);
           else if constexpr (std::is_same_v<T, ::Vector3> || std::is_same_v<T, std::array<float, 3>>)
               ::SetShaderValue(m_data, uniformLoc, &val, SHADER_UNIFORM_VEC3);
           else if constexpr (std::is_same_v<T, ::Vector4> || std::is_same_v<T, std::array<float, 4>>)
               ::SetShaderValue(m_data, uniformLoc, &val, SHADER_UNIFORM_VEC4);
           else if constexpr (std::is_same_v<T, int>)
               ::SetShaderValue(m_data, uniformLoc, &val, SHADER_UNIFORM_INT);
           else if constexpr (std::is_same_v<T, std::array<int, 2>>)
               ::SetShaderValue(m_data, uniformLoc, &val, SHADER_UNIFORM_IVEC2);
           else if constexpr (std::is_same_v<T, std::array<int, 3>>)
               ::SetShaderValue(m_data, uniformLoc, &val, SHADER_UNIFORM_IVEC3);
           else if constexpr (std::is_same_v<T, std::array<int, 4>>)
               ::SetShaderValue(m_data, uniformLoc, &val, SHADER_UNIFORM_IVEC4);
           else if constexpr (std::is_same_v<T, ::Texture2D>)
               ::SetShaderValue(m_data, uniformLoc, &val.id, SHADER_UNIFORM_SAMPLER2D);
       }, value);
       return *this;
    }

    /**
     * Set shader uniform value vector
     *
     * @see SetShaderValueV()
     */
    [[deprecated("Use SetValue<T>(..., span) or SetValueV(..., variant)")]]
    Shader& SetValue(int uniformLoc, const void* value, int uniformType, int count) {
        ::SetShaderValueV(m_data, uniformLoc, value, uniformType, count);
        return *this;
    }
    template<typename T>
    /// @TODO: requires T can only be vec, ivec, int, float, ... (SHADER_UNIFORM)
    Shader& SetValue(int uniformLoc, std::span<const T> value, int uniformType) {
        ::SetShaderValueV(m_data, uniformLoc, value.data(), uniformType, value.size());
        return *this;
    }
    Shader& SetValueV(int uniformLoc, std::variant<std::span<float>,
            std::span<std::array<float, 2>>, ///< vec2 (2 float)
            std::span<std::array<float, 3>>, ///< vec3 (3 float)
            std::span<std::array<float, 4>>, ///< vec4 (4 float)
            std::span<::Vector2>,            ///< vec2 (2 float)
            std::span<::Vector3>,            ///< vec3 (3 float)
            std::span<::Vector4>,            ///< vec4 (4 float)
            std::span<int>,
            std::span<std::array<int, 2>>,   ///< ivec2 (2 int)
            std::span<std::array<int, 3>>,   ///< ivec3 (3 int)
            std::span<std::array<int, 4>>    ///< ivec4 (4 int)
            /// @TODO: add mdspan for vecX and ivecX
            /// @TODO: Shader uniform type: sampler2d
            > value) {
        std::visit([&](auto&& val) {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, std::span<float>>)
                ::SetShaderValueV(m_data, uniformLoc, val.data(), SHADER_UNIFORM_FLOAT, static_cast<int>(val.size()));
            else if constexpr (std::is_same_v<T, std::span<::Vector2>> || std::is_same_v<T, std::span<std::array<float, 2>>>)
                ::SetShaderValueV(m_data, uniformLoc, val.data(), SHADER_UNIFORM_VEC2, static_cast<int>(val.size()));
            else if constexpr (std::is_same_v<T, std::span<::Vector3>> || std::is_same_v<T, std::span<std::array<float, 3>>>)
                ::SetShaderValueV(m_data, uniformLoc, val.data(), SHADER_UNIFORM_VEC3, static_cast<int>(val.size()));
            else if constexpr (std::is_same_v<T, std::span<::Vector4>> || std::is_same_v<T, std::span<std::array<float, 4>>>)
                ::SetShaderValueV(m_data, uniformLoc, val.data(), SHADER_UNIFORM_VEC4, static_cast<int>(val.size()));
            else if constexpr (std::is_same_v<T, std::span<int>>)
                ::SetShaderValueV(m_data, uniformLoc, val.data(), SHADER_UNIFORM_INT, static_cast<int>(val.size()));
            else if constexpr (std::is_same_v<T, std::span<std::array<int, 2>>>)
                ::SetShaderValueV(m_data, uniformLoc, val.data(), SHADER_UNIFORM_IVEC2, static_cast<int>(val.size()));
            else if constexpr (std::is_same_v<T, std::span<std::array<int, 3>>>)
                ::SetShaderValueV(m_data, uniformLoc, val.data(), SHADER_UNIFORM_IVEC3, static_cast<int>(val.size()));
            else if constexpr (std::is_same_v<T, std::span<std::array<int, 4>>>)
                ::SetShaderValueV(m_data, uniformLoc, val.data(), SHADER_UNIFORM_IVEC4, static_cast<int>(val.size()));
        }, value);
        return *this;
    }

    /**
     * Set shader uniform value (matrix 4x4)
     *
     * @see SetShaderValueMatrix()
     */
    Shader& SetValue(int uniformLoc, const ::Matrix& mat) noexcept {
        ::SetShaderValueMatrix(m_data, uniformLoc, mat);
        return *this;
    }

    /**
     * Set shader uniform value for texture
     *
     * @see SetShaderValueTexture()
     */
    Shader& SetValue(int uniformLoc, const ::Texture2D& texture) noexcept {
        ::SetShaderValueTexture(m_data, uniformLoc, texture);
        return *this;
    }
    Shader& SetValue(int uniformLoc, const raylib::Texture& texture) {
        ::SetShaderValueTexture(m_data, uniformLoc, texture.c_raylib());
        return *this;
    }

    /**
     * Retrieves whether or not the shader is ready.
     */
    [[nodiscard]] constexpr bool IsReady() const noexcept {
        return m_data.id != 0 && m_data.locs != nullptr;
    }

 protected:
    constexpr void set(const ::Shader& shader) noexcept {
        m_data.id = shader.id;
        m_data.locs = shader.locs;
    }

    ::Shader m_data;
};

}  // namespace raylib

using RShader = raylib::Shader;

#endif  // RAYLIB_CPP_INCLUDE_SHADER_HPP_
