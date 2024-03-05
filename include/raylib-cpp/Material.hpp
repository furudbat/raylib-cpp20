#ifndef RAYLIB_CPP_INCLUDE_MATERIAL_HPP_
#define RAYLIB_CPP_INCLUDE_MATERIAL_HPP_

#include <string>
#include <vector>
#include <array>

#include "Mesh.hpp"
#include "Texture.hpp"
#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"

namespace raylib {

struct RayMaterials {
    ::Material* data{nullptr};
    size_t size{0};

    RayMaterials(owner<::Material*> _data, size_t _size) : data(_data), size(_size) {}
    ~RayMaterials() {
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

    explicit constexpr Material(owner<const ::Material&> material) = delete;
    explicit constexpr Material(owner<::Material&&> material) {
        set(material);

        material.maps = nullptr;
        material.shader = NullShader;
        material.params[0] = 0.0F;
        material.params[1] = 0.0F;
        material.params[2] = 0.0F;
        material.params[3] = 0.0F;
    }

    Material(const Material&) = delete;
    Material(Material&& other) {
        set(other.m_data);

        other.m_data.maps = nullptr;
        other.m_data.shader = NullShader;
        other.m_data.params[0] = 0.0F;
        other.m_data.params[1] = 0.0F;
        other.m_data.params[2] = 0.0F;
        other.m_data.params[3] = 0.0F;
    }

    ~Material() {
        Unload();
    }

    //explicit operator ::Material() const {
    //    return m_data;
    //}
    [[nodiscard]] ::Material c_raylib() const & {
        return m_data;
    }

    constexpr Material& operator=(const ::Material& material) = delete;
    constexpr Material& operator=(::Material&& material) {
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
    static std::vector<Material> Load(const std::string& fileName) {
        int count = 0;
        ::Material* materials_data = ::LoadMaterials(fileName.c_str(), &count);
        RayMaterials materials (materials_data, static_cast<size_t>(count));

        std::vector<Material> ret;
        ret.reserve(static_cast<size_t>(count));
        for (auto& mat : materials.AsSpan()) {
            ret.emplace_back(std::move(mat));
            mat.maps = nullptr;
        }
        materials.data = nullptr; ///< data has been moved

        return ret;
    }

    GETTERSETTER(::Shader, Shader, m_data.shader)
    CONST_GETTER(::MaterialMap*, Maps, m_data.maps)
    /** Retrieves the params value for the object. @return The params value of the object. */
    constexpr std::array<float, 4> GetParams() const {
        return std::array<float, 4>{ m_data.params[0], m_data.params[1], m_data.params[2], m_data.params[3] };
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
    void Unload() {
        if (m_data.maps != nullptr) {
            ::UnloadMaterial(m_data);
            m_data.maps = nullptr;
            m_data.shader = NullShader;
        }
    }

    /**
     * Set texture for a material map type (MAP_DIFFUSE, MAP_SPECULAR...)
     */
    Material& SetTexture(int mapType, const ::Texture2D& texture) {
        ::SetMaterialTexture(&m_data, mapType, texture);
        return *this;
    }
    Material& SetTexture(int mapType, const raylib::Texture2D& texture) {
        ::SetMaterialTexture(&m_data, mapType, texture.c_raylib());
        return *this;
    }

    /**
     * Draw a 3d mesh with material and transform
     */
    void DrawMesh(const ::Mesh& mesh, ::Matrix transform) const {
        ::DrawMesh(mesh, m_data, transform);
    }
    void DrawMesh(const raylib::Mesh& mesh, ::Matrix transform) const {
        ::DrawMesh(mesh.c_raylib(), m_data, transform);
    }

    /**
     * Draw multiple mesh instances with material and different transforms
     */
    void DrawMesh(const ::Mesh& mesh, ::Matrix* transforms, int instances) const {
        ::DrawMeshInstanced(mesh, m_data, transforms, instances);
    }
    void DrawMesh(const raylib::Mesh& mesh, ::Matrix* transforms, int instances) const {
        ::DrawMeshInstanced(mesh.c_raylib(), m_data, transforms, instances);
    }

    /**
     * Check if material is ready
     */
    [[nodiscard]] bool IsReady() const {
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
};
}  // namespace raylib

using RMaterial = raylib::Material;

#endif  // RAYLIB_CPP_INCLUDE_MATERIAL_HPP_
