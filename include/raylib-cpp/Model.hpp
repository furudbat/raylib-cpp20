#ifndef RAYLIB_CPP_INCLUDE_MODEL_HPP_
#define RAYLIB_CPP_INCLUDE_MODEL_HPP_

#include <string>
#include <filesystem>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"

#include "./Mesh.hpp"
#ifdef __cpp_exceptions
#include "./RaylibException.hpp"
#endif
#include "./RaylibError.hpp"

namespace raylib {

class Mesh;

/**
 * Model type
 */
class Model {
 public:
    constexpr Model() = default;

    explicit constexpr Model(owner<const ::Model&> model) = delete;
    explicit constexpr Model(owner<::Model&&> model) {
        set(model);

        model.meshCount = 0;
        model.materialCount = 0;
        model.meshes = nullptr;
        model.materials = nullptr;
        model.meshMaterial = nullptr;
        model.boneCount = 0;
        model.bones = nullptr;
        model.bindPose = nullptr;
    }

    /*
     * Load a model from a file.
     *
     * @throws raylib::RaylibException Throws if failed to load the Modal.
     */
    Model(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        Load(fileName);
    }

    /*
     * Load a model from a mesh.
     *
     * @throws raylib::RaylibException Throws if failed to load the Modal.
     */
    Model(const ::Mesh& mesh) RAYLIB_CPP_THROWS {
        Load(mesh);
    }
    Model(const raylib::Mesh& mesh) RAYLIB_CPP_THROWS {
        Load(mesh);
    }

    ~Model() {
        Unload();
    }

    constexpr Model(const Model&) = delete;
    Model(Model&& other) {
        set(other.m_data);

        other.m_data.meshCount = 0;
        other.m_data.materialCount = 0;
        other.m_data.meshes = nullptr;
        other.m_data.materials = nullptr;
        other.m_data.meshMaterial = nullptr;
        other.m_data.boneCount = 0;
        other.m_data.bones = nullptr;
        other.m_data.bindPose = nullptr;
    }

    constexpr Model& operator=(owner<const ::Model&> model) = delete;
    constexpr Model& operator=(owner<::Model&&> model) {
        set(model);

        model.meshCount = 0;
        model.materialCount = 0;
        model.meshes = nullptr;
        model.materials = nullptr;
        model.meshMaterial = nullptr;
        model.boneCount = 0;
        model.bones = nullptr;
        model.bindPose = nullptr;

        return *this;
    }

    constexpr Model& operator=(const Model&) = delete;
    Model& operator=(Model&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other.m_data);

        other.m_data.meshCount = 0;
        other.m_data.materialCount = 0;
        other.m_data.meshes = nullptr;
        other.m_data.materials = nullptr;
        other.m_data.meshMaterial = nullptr;
        other.m_data.boneCount = 0;
        other.m_data.bones = nullptr;
        other.m_data.bindPose = nullptr;

        return *this;
    }

    //explicit operator ::Model() const {
    //    return m_data;
    //}
    [[nodiscard]] ::Model c_raylib() const & {
        return m_data;
    }

    GETTER(::Matrix, Transform, m_data.transform)
    GETTER(int, MeshCount, m_data.meshCount)
    GETTER(int, MaterialCount, m_data.materialCount)
    SPAN_GETTER(::Mesh, Meshes, m_data.meshes, m_data.meshCount)
    SPAN_GETTER(::Material, Materials, m_data.materials, m_data.materialCount)
    CONST_GETTER(int*, MeshMaterial, m_data.meshMaterial)
    GETTER(int, BoneCount, m_data.boneCount)
    SPAN_GETTER(::BoneInfo, Bones, m_data.bones, m_data.boneCount)
    CONST_GETTER(::Transform*, BindPose, m_data.bindPose)

    /**
     * Unload model (including meshes) from memory (RAM and/or VRAM)
     */
    void Unload() {
        if (m_data.meshes != nullptr || m_data.materials != nullptr) {
            ::UnloadModel(m_data);
            m_data.meshes = nullptr;
            m_data.materials = nullptr;
        }
    }

    /**
     * Set material for a mesh
     */
    Model& SetMeshMaterial(int meshId, int materialId) {
        ::SetModelMeshMaterial(&m_data, meshId, materialId);
        return *this;
    }

    /**
     * Update model animation pose
     */
    Model& UpdateAnimation(const ::ModelAnimation& anim, int frame) {
        ::UpdateModelAnimation(m_data, anim, frame);
        return *this;
    }

    /**
     * Check model animation skeleton match
     */
    [[nodiscard]] bool IsModelAnimationValid(const ::ModelAnimation& anim) const {
        return ::IsModelAnimationValid(m_data, anim);
    }

    /**
     * Draw a model (with texture if set)
     */
    void Draw(::Vector3 position,
            float scale = 1.0F,
            ::Color tint = WHITE) const {
        ::DrawModel(m_data, position, scale, tint);
    }

    /**
     * Draw a model with extended parameters
     */
    void Draw(
            ::Vector3 position,
            ::Vector3 rotationAxis,
            float rotationAngle = 0.0F,
            ::Vector3 scale = {1.0F, 1.0F, 1.0F},
            ::Color tint = {255, 255, 255, 255}) const {
        ::DrawModelEx(m_data, position, rotationAxis, rotationAngle, scale, tint);
    }

    /**
     * Draw a model wires (with texture if set)
     */
    void DrawWires(::Vector3 position,
            float scale = 1.0F,
            ::Color tint = WHITE) const {
        ::DrawModelWires(m_data, position, scale, tint);
    }

    /**
     * Draw a model wires (with texture if set) with extended parameters
     */
    void DrawWires(
            ::Vector3 position,
            ::Vector3 rotationAxis,
            float rotationAngle = 0.0F,
            ::Vector3 scale = {1.0F, 1.0F, 1.0F},
            ::Color tint = WHITE) const {
        ::DrawModelWiresEx(m_data, position, rotationAxis, rotationAngle, scale, tint);
    }

    /**
     * Compute model bounding box limits (considers all meshes)
     */
    [[nodiscard]] raylib::BoundingBox GetBoundingBox() const {
        return raylib::BoundingBox{::GetModelBoundingBox(m_data)};
    }

    /**
     * Compute model bounding box limits (considers all meshes)
     */
    explicit operator raylib::BoundingBox() const {
        return raylib::BoundingBox{::GetModelBoundingBox(m_data)};
    }

    /**
     * Determines whether or not the Model has data in it.
     */
    [[nodiscard]] bool IsReady() const {
        return ::IsModelReady(m_data);
    }

    /**
     * Loads a Model from the given file.
     *
     * @throws raylib::RaylibException Throws if failed to load the Modal.
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        set(::LoadModel(fileName.c_str()));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Model from " + fileName.string()));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Loads a Model from the given Mesh.
     *
     * @throws raylib::RaylibException Throws if failed to load the Modal.
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const ::Mesh& mesh) RAYLIB_CPP_THROWS {
        set(::LoadModelFromMesh(mesh));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Model from Mesh"));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }
    inline RAYLIB_CPP_EXPECTED_RESULT(void) Load(const raylib::Mesh& mesh) RAYLIB_CPP_THROWS {
        return Load(mesh.c_raylib());
    }

 protected:
    constexpr void set(const ::Model& model) noexcept {
        m_data.transform = model.transform;

        m_data.meshCount = model.meshCount;
        m_data.materialCount = model.materialCount;
        m_data.meshes = model.meshes;
        m_data.materials = model.materials;
        m_data.meshMaterial = model.meshMaterial;

        m_data.boneCount = model.boneCount;
        m_data.bones = model.bones;
        m_data.bindPose = model.bindPose;
    }

    ::Model m_data;
};

}  // namespace raylib

using RModel = raylib::Model;

#endif  // RAYLIB_CPP_INCLUDE_MODEL_HPP_
