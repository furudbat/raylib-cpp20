#ifndef RAYLIB_CPP_INCLUDE_SHADERUNMANAGED_HPP_
#define RAYLIB_CPP_INCLUDE_SHADERUNMANAGED_HPP_

#include <string>
#include <filesystem>
#include <optional>
#include <utility>
#include <variant>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "./Texture.hpp"
#include <rlgl.h>

namespace raylib {

class Shader;
class Material;

/**
 * Shader type (generic)
 */
class ShaderUnmanaged {
 public:
    constexpr ShaderUnmanaged() : m_data{NullShader} {}

    constexpr explicit ShaderUnmanaged(owner<const ::Shader&> shader) = delete;
    constexpr explicit ShaderUnmanaged(owner<::Shader&&> shader) noexcept {
        set(shader);

        shader.id = 0;
        shader.locs = nullptr;
    }

    constexpr ShaderUnmanaged(owner<unsigned int> id, owner<int*> locs = nullptr) : m_data{id, locs} {}

    struct LoadShaderOptions {
        std::optional<std::filesystem::path> vsFileName;
        std::optional<std::filesystem::path> fsFileName;
    };
    explicit ShaderUnmanaged(LoadShaderOptions options) {
        set(::LoadShader(options.vsFileName ? options.vsFileName.value().c_str() : nullptr, options.fsFileName ? options.fsFileName.value().c_str() : nullptr));
    }

    struct LoadShaderOptionsC {
        const char* vsFileName{nullptr};
        const char* fsFileName{nullptr};
    };
    explicit ShaderUnmanaged(LoadShaderOptionsC options) {
        set(::LoadShader(options.vsFileName, options.fsFileName));
    }

    /**
     * Load shader from files and bind default locations.
     *
     * @see ::LoadShader
     */
    static ShaderUnmanaged Load(LoadShaderOptions options) {
        return ShaderUnmanaged(std::move(options));
    }

    static ShaderUnmanaged Load(LoadShaderOptionsC options) {
        return ShaderUnmanaged(options);
    }

    /**
     * Load a shader from memory.
     *
     * @see ::LoadShaderFromMemory
     */
    struct LoadFromMemoryOptions {
        std::reference_wrapper<const std::string> vsCode;
        std::reference_wrapper<const std::string> fsCode;
    };
    static RAYLIB_CPP_EXPECTED_RESULT(ShaderUnmanaged) LoadFromMemory(LoadFromMemoryOptions options) RAYLIB_CPP_THROWS {
        auto shader = ::LoadShaderFromMemory(options.vsCode.get().c_str(), options.fsCode.get().c_str());
        if(!::IsShaderReady(shader)) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Shader from memory: " + options.vsCode.get() + ", " + options.fsCode.get()));
        }
        RAYLIB_CPP_RETURN_EXPECTED_VALUE(ShaderUnmanaged(std::move(shader)));
    }

    struct LoadFromMemoryOptionsC {
        const char* vsCode;
        const char* fsCode;
    };
    static RAYLIB_CPP_EXPECTED_RESULT(ShaderUnmanaged) LoadFromMemory(LoadFromMemoryOptionsC options) RAYLIB_CPP_THROWS {
        auto shader = ::LoadShaderFromMemory(options.vsCode, options.fsCode);
        if(!::IsShaderReady(shader)) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Shader from memory: " + std::string{options.vsCode} + ", " + std::string{options.fsCode}));
        }
        RAYLIB_CPP_RETURN_EXPECTED_VALUE(ShaderUnmanaged(std::move(shader)));
    }

    GETTERSETTER(unsigned int, Id, m_data.id)
    CONST_GETTER(int*, Locs, m_data.locs)
    //SPAN_GETTER(int, Locs, m_data.locs, RL_MAX_SHADER_LOCATIONS)
    int GetLoc(size_t index) const {
        if(m_data.locs != nullptr) {
            return m_data.locs[index];
        }
        return -1;
    }
    std::span<const int> GetLocsSpan() const { return { m_data.locs, m_data.locs != nullptr ? static_cast<size_t>(RL_MAX_SHADER_LOCATIONS) : 0}; }
    std::span<int> GetLocsSpan() { return { m_data.locs, m_data.locs != nullptr ?static_cast<size_t>(RL_MAX_SHADER_LOCATIONS) : 0}; }
    void SetLocs(std::span<const int> value) {
        if(m_data.id != 0) {
            if (m_data.locs == nullptr) {
                m_data.locs = (int *) RL_CALLOC(RL_MAX_SHADER_LOCATIONS, sizeof(int));
                // All locations reset to -1 (no location)
                for (size_t i = 0; i < RL_MAX_SHADER_LOCATIONS; i++) {
                    m_data.locs[i] = -1;
                }
            }
            for (size_t i = 0; i < value.size() && i < RL_MAX_SHADER_LOCATIONS; i++) {
                m_data.locs[i] = value[i];
            }
        }
    }
    void SetLoc(size_t index, int value) {
        if(m_data.locs != nullptr) {
            m_data.locs[index] = value;
        }
    }
    void SetLocFromUniform(size_t index, const char *uniformName) {
        if(m_data.locs != nullptr) {
            m_data.locs[index] = rlGetLocationUniform(m_data.id, uniformName);
        }
    }

    constexpr ShaderUnmanaged& operator=(const ::Shader& shader) noexcept {
        set(shader);
        return *this;
    }

    explicit operator ::Shader() const noexcept {
        return m_data;
    }
    [[nodiscard]] ::Shader c_raylib() const & noexcept {
        return m_data;
    }

    /**
     * Begin custom shader drawing.
     */
    ShaderUnmanaged& BeginMode() noexcept {
        ::BeginShaderMode(m_data);
        return *this;
    }

    /**
     * End custom shader drawing (use default shader).
     */
    ShaderUnmanaged& EndMode() noexcept {
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
    [[nodiscard]] int GetLocation(const char* uniformName) const {
        return ::GetShaderLocation(m_data, uniformName);
    }

    /**
     * Get shader attribute location
     *
     * @see GetShaderLocationAttrib()
     */
    [[nodiscard]] int GetLocationAttrib(const std::string& attribName) const {
        return ::GetShaderLocationAttrib(m_data, attribName.c_str());
    }
    [[nodiscard]] int GetLocationAttrib(const char* attribName) const {
        return ::GetShaderLocationAttrib(m_data, attribName);
    }

    /**
     * Set shader uniform value
     *
     * @see SetShaderValue()
     */
    [[deprecated("Use SetValue<T>(...) or SetValue(..., variant)")]]
    ShaderUnmanaged& SetValue(int uniformLoc, const void* value, int uniformType) {
        ::SetShaderValue(m_data, uniformLoc, value, uniformType);
        return *this;
    }
    template<typename T>
    /// @TODO: requires T can only be vec, ivec, int, float, ... (SHADER_UNIFORM)
    ShaderUnmanaged& SetValue(int uniformLoc, const T& value, int uniformType) {
        ::SetShaderValue(m_data, uniformLoc, &value, uniformType);
        return *this;
    }
    ShaderUnmanaged& SetValue(int uniformLoc, std::variant<float,
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
    ShaderUnmanaged& SetValue(const char* uniformName, std::variant<float,
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
        return SetValue(GetLocation(uniformName), value);
    }

    /**
     * Set shader uniform value vector
     *
     * @see SetShaderValueV()
     */
    [[deprecated("Use SetValue<T>(..., span) or SetValueV(..., variant)")]]
    ShaderUnmanaged& SetValue(int uniformLoc, const void* value, int uniformType, int count) {
        ::SetShaderValueV(m_data, uniformLoc, value, uniformType, count);
        return *this;
    }
    template<typename T>
    /// @TODO: requires T can only be vec, ivec, int, float, ... (SHADER_UNIFORM)
    ShaderUnmanaged& SetValue(int uniformLoc, std::span<const T> value, int uniformType) {
        ::SetShaderValueV(m_data, uniformLoc, value.data(), uniformType, value.size());
        return *this;
    }
    ShaderUnmanaged& SetValueV(int uniformLoc, std::variant<std::span<float>,
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
    ShaderUnmanaged& SetValue(int uniformLoc, const ::Matrix& mat) noexcept {
        ::SetShaderValueMatrix(m_data, uniformLoc, mat);
        return *this;
    }

    /**
     * Set shader uniform value for texture
     *
     * @see SetShaderValueTexture()
     */
    ShaderUnmanaged& SetValue(int uniformLoc, const ::Texture2D& texture) noexcept {
        ::SetShaderValueTexture(m_data, uniformLoc, texture);
        return *this;
    }
    ShaderUnmanaged& SetValue(int uniformLoc, const raylib::Texture& texture) {
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

    friend class Shader;
    friend class Material;
};

}  // namespace raylib

using RShaderUnmanaged = raylib::ShaderUnmanaged;

#endif  // RAYLIB_CPP_INCLUDE_SHADERUNMANAGED_HPP_
