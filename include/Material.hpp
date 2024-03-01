#ifndef RAYLIB_CPP_INCLUDE_MATERIAL_HPP_
#define RAYLIB_CPP_INCLUDE_MATERIAL_HPP_

#include <string>
#include <vector>
#include <array>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"

namespace raylib {

struct RayMaterials {
    ::Material* data{nullptr};
    size_t size{0};

    RayMaterials(owner<::Material*> _data, size_t _size) : data(_data), size(_size) {}
    ~RayMaterials() {
        if (data != nullptr) {
            for (auto &mat: as_span()) {
                ::UnloadMaterial(mat);
            }
        }
    }

    operator std::span<::Material>() {
        return {data, size};
    }
    operator std::span<const ::Material>() const {
        return {data, size};
    }

    std::span<::Material> as_span() { return {data, size}; }
    std::span<const ::Material> as_span() const { return {data, size}; }
};

/**
 * Material type (generic)
 */
class Material : public ::Material {
 public:
    /**
     * Load default material (Supports: DIFFUSE, SPECULAR, NORMAL maps)
     */
    Material() {
        set(LoadMaterialDefault());
    }

    explicit constexpr Material(const ::Material& material) = delete;
    explicit constexpr Material(::Material&& material) {
        set(material);

        material.maps = nullptr;
        material.shader = { .id = 0, };
        material.params[0] = 0.0F;
        material.params[1] = 0.0F;
        material.params[2] = 0.0F;
        material.params[3] = 0.0F;
    }

    Material(const Material&) = delete;
    Material(Material&& other) {
        set(other);

        other.maps = nullptr;
        other.shader = { .id = 0, };
        other.params[0] = 0.0F;
        other.params[1] = 0.0F;
        other.params[2] = 0.0F;
        other.params[3] = 0.0F;
    }

    ~Material() {
        Unload();
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
        for (auto& mat : materials.as_span()) {
            ret.emplace_back(std::move(mat));
        }
        materials.data = nullptr; ////< data has been moved

        return ret;
    }

    GETTERSETTER(::Shader, Shader, shader)
    GETTERSETTER(::MaterialMap*, Maps, maps)
    /** Retrieves the params value for the object. @return The params value of the object. */
    constexpr std::array<float, 4> GetParams() const {
        return params != nullptr ? std::array<float, 4>{ params[0], params[1], params[2], params[3] } : std::array<float, 4>{ 0.0F, 0.0F, 0.0F, 0.0F };
    }
    /** Sets the params value for the object. @param value The value of which to set params to. */
    constexpr bool SetParams(std::array<float, 4> value) {
        /// @TODO: better setter for params
        if (params != nullptr) {
            params[0] = value[0];
            params[1] = value[1];
            params[2] = value[2];
            params[3] = value[3];
            return true;
        }
        return false;
    }

    constexpr Material& operator=(const ::Material& material) = delete;
    constexpr Material& operator=(::Material&& material) {
        set(material);

        material.maps = nullptr;
        material.shader = { .id = 0, };

        return *this;
    }

    constexpr Material& operator=(const Material&) = delete;
    Material& operator=(Material&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other);

        other.maps = nullptr;
        other.shader = { .id = 0, };

        return *this;
    }

    /**
     * Unload material from memory
     */
    void Unload() {
        if (maps != nullptr) {
            ::UnloadMaterial(*this);
            maps = nullptr;
        }
    }

    /**
     * Set texture for a material map type (MAP_DIFFUSE, MAP_SPECULAR...)
     */
    Material& SetTexture(int mapType, const ::Texture2D& texture) {
        ::SetMaterialTexture(this, mapType, texture);
        return *this;
    }

    /**
     * Draw a 3d mesh with material and transform
     */
    void DrawMesh(const ::Mesh& mesh, ::Matrix transform) const {
        ::DrawMesh(mesh, *this, transform);
    }

    /**
     * Draw multiple mesh instances with material and different transforms
     */
    void DrawMesh(const ::Mesh& mesh, ::Matrix* transforms, int instances) const {
        ::DrawMeshInstanced(mesh, *this, transforms, instances);
    }

    /**
     * Check if material is ready
     */
    [[nodiscard]] bool IsReady() const {
        return ::IsMaterialReady(*this);
    }

 protected:
    constexpr void set(const ::Material& material) noexcept {
        shader = material.shader;
        maps = material.maps;
        params[0] = material.params[0];
        params[1] = material.params[1];
        params[2] = material.params[2];
        params[3] = material.params[3];
    }
};
}  // namespace raylib

using RMaterial = raylib::Material;

#endif  // RAYLIB_CPP_INCLUDE_MATERIAL_HPP_
