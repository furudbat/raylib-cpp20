#ifndef RAYLIB_CPP_INCLUDE_SHADER_HPP_
#define RAYLIB_CPP_INCLUDE_SHADER_HPP_


#include "raylib.hpp"
#include "raylib-cpp-utils.hpp"
#include "Texture.hpp"
#include "ShaderUnmanaged.hpp"

#include <string>
#include <filesystem>
#include <optional>
#include <utility>
#include <variant>

namespace raylib {

class Material;
class Model;

/**
 * Shader type (generic)
 */
class Shader {
public:
    using LoadShaderOptions = ShaderUnmanaged::LoadShaderOptions;
    using LoadShaderOptionsC = ShaderUnmanaged::LoadShaderOptionsC;

    constexpr Shader() = default;

    constexpr explicit Shader(const ::Shader& shader) = delete;
    constexpr explicit Shader(::Shader&& shader) noexcept {
        m_shader.set(shader);

        shader.id = 0;
        shader.locs = nullptr;
    }

    constexpr Shader(unsigned int id, owner<int*> locs = nullptr) : m_shader(id, locs) {}
    explicit Shader(LoadShaderOptions options) : m_shader(std::move(options)) {}
    explicit Shader(LoadShaderOptionsC options) : m_shader(options) {}

    [[deprecated("Use Shader(LoadShaderOptionsC), named and strong typed parameters")]]
    Shader(czstring vsFileName, czstring fsFileName) : m_shader(ShaderUnmanaged::LoadShaderOptionsC{ .vsFileName = vsFileName, .fsFileName = fsFileName }) {}


    constexpr Shader(const Shader&) = delete;
    Shader(Shader&& other) noexcept {
        m_shader.set(other.m_shader.m_data);

        other.m_shader.m_data.id = rlGetShaderIdDefault();
        other.m_shader.m_data.locs = nullptr;
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
    static Shader LoadFromMemory(ShaderUnmanaged::LoadFromMemoryOptions options) {
        return Shader{::LoadShaderFromMemory(options.vsCode.get().c_str(), options.fsCode.get().c_str())};
    }

    static Shader LoadFromMemory(ShaderUnmanaged::LoadFromMemoryOptionsC options) {
        return Shader{::LoadShaderFromMemory(options.vsCode, options.fsCode)};
    }

    CONST_COMPOSITION_METHODE_CALL(GetId, m_shader)
    COMPOSITION_METHODE_CALL_RETURN_THIS(SetId, m_shader)

    CONST_COMPOSITION_METHODE_CALL(GetLocs, m_shader)
    CONST_COMPOSITION_METHODE_CALL(GetLocsSpan, m_shader)
    COMPOSITION_METHODE_CALL(GetLocsSpan, m_shader)
    CONST_COMPOSITION_METHODE_CALL(GetLoc, m_shader)

    COMPOSITION_METHODE_CALL(SetLocs, m_shader)
    COMPOSITION_METHODE_CALL(SetLoc, m_shader)
    COMPOSITION_METHODE_CALL(SetLocFromLocation, m_shader)

    constexpr Shader& operator=(const ::Shader& shader) = delete;
    Shader& operator=(::Shader&& shader) noexcept {
        m_shader.set(shader);

        shader.id = rlGetShaderIdDefault();
        shader.locs = nullptr;

        return *this;
    }

    constexpr Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        m_shader.set(other.m_shader.m_data);

        other.m_shader.m_data.id = rlGetShaderIdDefault();
        other.m_shader.m_data.locs = nullptr;

        return *this;
    }

    /**
     * Unload shader from GPU memory (VRAM)
     */
    ~Shader() {
        Unload();
    }

    explicit operator ::Shader() const noexcept {
        return m_shader.m_data;
    }
    [[nodiscard]] ::Shader c_raylib() const & noexcept {
        return m_shader.m_data;
    }

    /**
     * Unload shader from GPU memory (VRAM)
     */
    void Unload() noexcept {
        if (m_shader.m_data.locs != nullptr) {
            if(m_shader.m_data.id != rlGetShaderIdDefault()) {
                ::UnloadShader(m_shader.m_data);
            } else {
                RL_FREE(m_shader.m_data.locs);
            }
            m_shader.m_data.id = rlGetShaderIdDefault();
            m_shader.m_data.locs = nullptr;
        }
    }

    COMPOSITION_METHODE_CALL_RETURN_THIS(BeginMode, m_shader)
    COMPOSITION_METHODE_CALL_RETURN_THIS(EndMode, m_shader)

    CONST_COMPOSITION_METHODE_CALL(GetLocation, m_shader)
    CONST_COMPOSITION_METHODE_CALL(GetLocationAttrib, m_shader)

    COMPOSITION_METHODE_CALL_RETURN_THIS(SetValue, m_shader)
    COMPOSITION_METHODE_CALL_RETURN_THIS(SetValueFromLocation, m_shader)
    COMPOSITION_METHODE_CALL_RETURN_THIS(SetValueFromLoc, m_shader)

    CONST_COMPOSITION_METHODE_CALL(IsReady, m_shader)

private:
    ShaderUnmanaged m_shader;

    friend class Material;
    friend class Model;
};

}  // namespace raylib

using RShader = raylib::Shader;

#endif  // RAYLIB_CPP_INCLUDE_SHADER_HPP_
