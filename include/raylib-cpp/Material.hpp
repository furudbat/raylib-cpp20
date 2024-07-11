#ifndef RAYLIB_CPP_INCLUDE_MATERIAL_HPP_
#define RAYLIB_CPP_INCLUDE_MATERIAL_HPP_

#include "Mesh.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "raylib.hpp"
#include "raylib-cpp-utils.hpp"

#include <rlgl.h>

#include <string>
#include <vector>
#include <array>

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#ifndef MAX_MATERIAL_MAPS
#define MAX_MATERIAL_MAPS       12    // Maximum number of maps supported
#endif
#ifndef MAX_MESH_VERTEX_BUFFERS
#define MAX_MESH_VERTEX_BUFFERS  7    // Maximum vertex buffers (VBO) per mesh
#endif

namespace raylib {

enum class MaterialShaderOption : uint8_t {
    UnloadShaderWhenUnloadingMaterial = 0,             ///< Unloads Shader when UnloadingMaterial
    UnbindShaderWhenUnload = 2,                        ///< Unbind (disconnect) shader from Material, to avoid UnloadMaterial() trying to unload it automatically (NO UnloadShader in UnloadingMaterial)
};

struct RayMaterials {
    ::Material* data{nullptr};
    size_t size{0};

    RayMaterials(owner<::Material*> _data, size_t _size) : data(_data), size(_size) {}
    ~RayMaterials() noexcept {
        if (data != nullptr) {
            for (auto &mat: AsSpan()) {
                if (mat.maps != nullptr) {
                    ::UnloadMaterial(mat);
                    mat.maps = nullptr;
                }
            }
        }
    }

    operator std::span<::Material>() {
        return {data, size};
    }
    operator std::span<const ::Material>() const {
        return {data, size};
    }

    std::span<::Material> AsSpan() { return {data, size}; }
    std::span<const ::Material> AsSpan() const { return {data, size}; }
};

[[maybe_unused]] RLCPPAPI inline ::Shader DefaultMaterialShader() {
    return ::Shader {
        .id = rlGetShaderIdDefault(),
        .locs = rlGetShaderLocsDefault(),
    };
};

/**
 * Material type (generic)
 */
class Material {
 public:
    /**
     * Load default material (Supports: DIFFUSE, SPECULAR, NORMAL maps)
     */
    Material() {
        set(LoadMaterialDefault());
    }

    explicit constexpr Material(const ::Material& material) = delete;
    explicit Material(::Material&& material) noexcept {
        set(material);

        material.maps = nullptr;
        material.shader = NullShader;
        material.params[0] = 0.0f;
        material.params[1] = 0.0f;
        material.params[2] = 0.0f;
        material.params[3] = 0.0f;
    }

    Material(const Material&) = delete;
    Material(Material&& other) noexcept {
        set(other.m_data);

        other.m_data.maps = nullptr;
        other.m_data.shader = NullShader;
        other.m_data.params[0] = 0.0f;
        other.m_data.params[1] = 0.0f;
        other.m_data.params[2] = 0.0f;
        other.m_data.params[3] = 0.0f;
    }

    ~Material() noexcept {
        Unload();
    }

    explicit operator ::Material() const noexcept {
        return m_data;
    }
    [[nodiscard]] ::Material c_raylib() const & noexcept {
        return m_data;
    }

    constexpr Material& operator=(const ::Material& material) = delete;
    Material& operator=(::Material&& material) noexcept {
        set(material);

        material.maps = nullptr;
        material.shader = NullShader;

        return *this;
    }

    constexpr Material& operator=(const Material&) = delete;
    Material& operator=(Material&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other.m_data);

        other.m_data.maps = nullptr;
        other.m_data.shader = NullShader;

        return *this;
    }

    /**
     * Load materials from model file
     */
    [[nodiscard]] static std::vector<Material> LoadMaterialsFromModel(czstring fileName) {
        RayMaterials materials = [&]() {
            int count = 0;
            ::Material* materials_data = ::LoadMaterials(fileName, &count);
            assert(count >= 0);
            return RayMaterials(materials_data, static_cast<size_t>(count));
        }();

        std::vector<Material> ret;
        ret.reserve(materials.size);
        for (auto& mat : materials.AsSpan()) {
            ret.emplace_back(std::move(mat));
            mat.maps = nullptr;
        }
        // data has been moved
        materials.data = nullptr;
        materials.size = 0;

        return ret;
    }
    [[nodiscard]] static std::vector<Material> LoadMaterialsFromModel(const std::string& fileName) {
        return LoadMaterialsFromModel(fileName.c_str());
    }

    //CONST_GETTER(::Shader, Shader, m_data.shader
    [[nodiscard]] constexpr ::Shader& GetShaderC() & { return m_data.shader; }
    [[nodiscard]] constexpr const ::Shader& GetShaderC() const & { return m_data.shader; }
    [[nodiscard]] Shader GetShader() && { return Shader(std::move(m_data.shader)); }
    template<class... Args>
    void EmplaceShader(Args&&... args) {
        SetShader(Shader(std::forward<Args>(args)...));
    }
    template<class... Args>
    void LoadShader(Args&&... args) {
        Shader shader;
        Shader::Load(std::forward<Args>(args)...);
        SetShader(std::move(shader), MaterialShaderOption::UnloadShaderWhenUnloadingMaterial);
    }

    void SetShader(::Shader shader, MaterialShaderOption option) {
        m_data.shader = shader;
        m_trackShaderManagement = option;
    }
    void SetShader(const Shader& shader, MaterialShaderOption option) {
        m_data.shader = shader.c_raylib();
        m_trackShaderManagement = option;
    }

    void SetShader(Shader&& shader, MaterialShaderOption option = MaterialShaderOption::UnloadShaderWhenUnloadingMaterial) {
        m_data.shader = shader.c_raylib();
        m_trackShaderManagement = option;
        switch (m_trackShaderManagement) {
            case MaterialShaderOption::UnloadShaderWhenUnloadingMaterial:
                shader.m_shader.m_data.id = rlGetShaderIdDefault();
                shader.m_shader.m_data.locs = nullptr;
                break;
            case MaterialShaderOption::UnbindShaderWhenUnload:
                break;
        }
    }

    /// @TODO: concept Func = (Shader&) => void
    void UpdateShader(auto&& Func) {
        Shader shader (std::move(m_data.shader));
        Func(shader);
        SetShader(std::move(shader));
    }

    Material& SetShaderValue(int uniformLoc, std::variant<float,
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
                ::SetShaderValue(m_data.shader, uniformLoc, &val, SHADER_UNIFORM_FLOAT);
            else if constexpr (std::is_same_v<T, ::Vector2>)
                ::SetShaderValue(m_data.shader, uniformLoc, &val, SHADER_UNIFORM_VEC2);
            else if constexpr (std::is_same_v<T, std::array<float, 2>>)
                ::SetShaderValue(m_data.shader, uniformLoc, val.data(), SHADER_UNIFORM_VEC2);
            else if constexpr (std::is_same_v<T, ::Vector3>)
                ::SetShaderValue(m_data.shader, uniformLoc, &val, SHADER_UNIFORM_VEC3);
            else if constexpr (std::is_same_v<T, std::array<float, 3>>)
                ::SetShaderValue(m_data.shader, uniformLoc, val.data(), SHADER_UNIFORM_VEC3);
            else if constexpr (std::is_same_v<T, ::Vector4>)
                ::SetShaderValue(m_data.shader, uniformLoc, &val, SHADER_UNIFORM_VEC4);
            else if constexpr (std::is_same_v<T, std::array<float, 4>>)
                ::SetShaderValue(m_data.shader, uniformLoc, val.data(), SHADER_UNIFORM_VEC4);
            else if constexpr (std::is_same_v<T, int>)
                ::SetShaderValue(m_data.shader, uniformLoc, &val, SHADER_UNIFORM_INT);
            else if constexpr (std::is_same_v<T, std::array<int, 2>>)
                ::SetShaderValue(m_data.shader, uniformLoc, val.data(), SHADER_UNIFORM_IVEC2);
            else if constexpr (std::is_same_v<T, std::array<int, 3>>)
                ::SetShaderValue(m_data.shader, uniformLoc, val.data(), SHADER_UNIFORM_IVEC3);
            else if constexpr (std::is_same_v<T, std::array<int, 4>>)
                ::SetShaderValue(m_data.shader, uniformLoc, val.data(), SHADER_UNIFORM_IVEC4);
            else if constexpr (std::is_same_v<T, ::Texture2D>)
                ::SetShaderValue(m_data.shader, uniformLoc, &val.id, SHADER_UNIFORM_SAMPLER2D);
        }, value);
        return *this;
    }
    Material& SetShaderValue(const char* uniformName, std::variant<float,
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
        return SetShaderValue(::GetShaderLocation(m_data.shader, uniformName), value);
    }
    Material& SetShaderValueFromLoc(size_t loc_index, std::variant<float,
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
        const auto GetLoc = [](::Shader shader, size_t index) {
            if(shader.locs != nullptr) {
                return shader.locs[index];
            }
            return -1;
        };
        return SetShaderValue(GetLoc(m_data.shader, loc_index), value);
    }


    //CONST_GETTER(::MaterialMap*, MapsC, m_data.maps)
    SPAN_GETTER(::MaterialMap, Maps, m_data.maps, MAX_MATERIAL_MAPS)

    [[nodiscard]] const ::MaterialMap& GetMap(size_t index) const {
        return m_data.maps[index];
    }
    [[nodiscard]] ::MaterialMap& GetMap(size_t index) {
        return m_data.maps[index];
    }

    [[nodiscard]] const ::MaterialMap& GetMap(MaterialMapIndexT index) const {
        return m_data.maps[static_cast<size_t>(index)];
    }
    [[nodiscard]] ::MaterialMap& GetMap(MaterialMapIndexT index) {
        return m_data.maps[static_cast<size_t>(index)];
    }

    /** Retrieves the params value for the object. @return The params value of the object. */
    [[nodiscard]] constexpr std::array<float, 4> GetParams() const {
        return std::array<float, 4>{ m_data.params[0], m_data.params[1], m_data.params[2], m_data.params[3] };
    }
    [[nodiscard]] constexpr std::span<float> GetParams() {
        return std::span<float>{ m_data.params, 4 };
    }
    /** Sets the params value for the object. @param value The value of which to set params to. */
    constexpr void SetParams(std::array<float, 4> value) {
        m_data.params[0] = value[0];
        m_data.params[1] = value[1];
        m_data.params[2] = value[2];
        m_data.params[3] = value[3];
    }


    /**
     * Unload material from memory
     */
    void Unload() noexcept {
        switch (m_trackShaderManagement) {
            case MaterialShaderOption::UnloadShaderWhenUnloadingMaterial:
                break;
            case MaterialShaderOption::UnbindShaderWhenUnload:
                m_data.shader.id = rlGetShaderIdDefault();
                m_data.shader.locs = nullptr;
                break;
        }
        if (m_data.maps != nullptr) {
            ::UnloadMaterial(m_data);
            m_data.maps = nullptr;
            m_data.shader = NullShader;
        }
    }

    /**
     * Set texture for a material map type (MAP_DIFFUSE, MAP_SPECULAR...)
     */
    Material& SetMaterialTexture(int mapType, const ::Texture2D& texture) noexcept {
        ::SetMaterialTexture(&m_data, mapType, texture);
        return *this;
    }
    [[deprecated("Use SetMaterialTexture(MaterialMapIndex, ...)")]]
    Material& SetMaterialTexture(int mapType, const raylib::Texture2D& texture) {
        ::SetMaterialTexture(&m_data, mapType, texture.c_raylib());
        return *this;
    }
    Material& SetMaterialTexture(MaterialMapIndexT mapType, const ::Texture2D& texture) noexcept {
        // underlying type is size_t, but SetMaterialTexture needs int
        ::SetMaterialTexture(&m_data, static_cast<int>(static_cast<size_t>(mapType)), texture);
        return *this;
    }
    Material& SetMaterialTexture(MaterialMapIndexT mapType, const raylib::Texture2D& texture) {
        // underlying type is size_t, but SetMaterialTexture needs int
        ::SetMaterialTexture(&m_data, static_cast<int>(static_cast<size_t>(mapType)), texture.c_raylib());
        return *this;
    }

    /**
     * Draw a 3d mesh with material and transform
     */
    void DrawMesh(const ::Mesh& mesh, ::Matrix transform) const noexcept {
        ::DrawMesh(mesh, m_data, transform);
    }
    void DrawMesh(const raylib::Mesh& mesh, ::Matrix transform) const {
        ::DrawMesh(mesh.c_raylib(), m_data, transform);
    }

    /**
     * Draw multiple mesh instances with material and different transforms
     */
    void DrawMesh(const ::Mesh& mesh, ::Matrix* transforms, int instances) const noexcept {
        ::DrawMeshInstanced(mesh, m_data, transforms, instances);
    }
    void DrawMesh(const raylib::Mesh& mesh, ::Matrix* transforms, int instances) const {
        ::DrawMeshInstanced(mesh.c_raylib(), m_data, transforms, instances);
    }

    /**
     * Check if material is ready
     */
    [[nodiscard]] bool IsReady() const noexcept {
        return ::IsMaterialReady(m_data);
    }

 protected:
    constexpr void set(const ::Material& material) noexcept {
        m_data.shader = material.shader;
        m_data.maps = material.maps;
        m_data.params[0] = material.params[0];
        m_data.params[1] = material.params[1];
        m_data.params[2] = material.params[2];
        m_data.params[3] = material.params[3];
    }

    ::Material m_data;
    MaterialShaderOption m_trackShaderManagement{MaterialShaderOption::UnloadShaderWhenUnloadingMaterial};
};
}  // namespace raylib

using RMaterial = raylib::Material;

#endif  // RAYLIB_CPP_INCLUDE_MATERIAL_HPP_
