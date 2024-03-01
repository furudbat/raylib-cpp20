#ifndef RAYLIB_CPP_INCLUDE_MODEL_HPP_
#define RAYLIB_CPP_INCLUDE_MODEL_HPP_

#include <string>
#include <filesystem>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"

#ifdef __cpp_exceptions
#include "./RaylibException.hpp"
#endif
#include "./RaylibError.hpp"

namespace raylib {

class Mesh;

/**
 * Model type
 */
class Model : public ::Model {
 public:
    constexpr Model() = default;

    explicit constexpr Model(const ::Model& model) = delete;
    explicit constexpr Model(::Model&& model) {
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

    ~Model() {
        Unload();
    }

    constexpr Model(const Model&) = delete;
    Model(Model&& other) {
        set(other);

        other.meshCount = 0;
        other.materialCount = 0;
        other.meshes = nullptr;
        other.materials = nullptr;
        other.meshMaterial = nullptr;
        other.boneCount = 0;
        other.bones = nullptr;
        other.bindPose = nullptr;
    }

    GETTERSETTER(::Matrix, Transform, transform)
    GETTERSETTER(int, MeshCount, meshCount)
    GETTERSETTER(int, MaterialCount, materialCount)
    GETTERSETTER(::Mesh*, Meshes, meshes)
    GETTERSETTER(::Material*, Materials, materials)
    GETTERSETTER(int*, MeshMaterial, meshMaterial)
    GETTERSETTER(int, BoneCount, boneCount)
    GETTERSETTER(::BoneInfo*, Bones, bones)
    GETTERSETTER(::Transform*, BindPose, bindPose)

    constexpr Model& operator=(const ::Model& model) = delete;
    constexpr Model& operator=(::Model&& model) {
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
        set(other);

        other.meshCount = 0;
        other.materialCount = 0;
        other.meshes = nullptr;
        other.materials = nullptr;
        other.meshMaterial = nullptr;
        other.boneCount = 0;
        other.bones = nullptr;
        other.bindPose = nullptr;

        return *this;
    }

    /**
     * Unload model (including meshes) from memory (RAM and/or VRAM)
     */
    void Unload() {
        if (meshes != nullptr || materials != nullptr) {
            ::UnloadModel(*this);
            meshes = nullptr;
            materials = nullptr;
        }
    }

    /**
     * Set material for a mesh
     */
    Model& SetMeshMaterial(int meshId, int materialId) {
        ::SetModelMeshMaterial(this, meshId, materialId);
        return *this;
    }

    /**
     * Update model animation pose
     */
    Model& UpdateAnimation(const ::ModelAnimation& anim, int frame) {
        ::UpdateModelAnimation(*this, anim, frame);
        return *this;
    }

    /**
     * Check model animation skeleton match
     */
    [[nodiscard]] bool IsModelAnimationValid(const ::ModelAnimation& anim) const {
        return ::IsModelAnimationValid(*this, anim);
    }

    /**
     * Draw a model (with texture if set)
     */
    void Draw(::Vector3 position,
            float scale = 1.0F,
            ::Color tint = WHITE) const {
        ::DrawModel(*this, position, scale, tint);
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
        ::DrawModelEx(*this, position, rotationAxis, rotationAngle, scale, tint);
    }

    /**
     * Draw a model wires (with texture if set)
     */
    void DrawWires(::Vector3 position,
            float scale = 1.0F,
            ::Color tint = WHITE) const {
        ::DrawModelWires(*this, position, scale, tint);
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
        ::DrawModelWiresEx(*this, position, rotationAxis, rotationAngle, scale, tint);
    }

    /**
     * Compute model bounding box limits (considers all meshes)
     */
    [[nodiscard]] raylib::BoundingBox GetBoundingBox() const {
        return raylib::BoundingBox{::GetModelBoundingBox(*this)};
    }

    /**
     * Compute model bounding box limits (considers all meshes)
     */
    explicit operator raylib::BoundingBox() const {
        return raylib::BoundingBox{::GetModelBoundingBox(*this)};
    }

    /**
     * Determines whether or not the Model has data in it.
     */
    [[nodiscard]] bool IsReady() const {
        return ::IsModelReady(*this);
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

 protected:
    constexpr void set(const ::Model& model) noexcept {
        transform = model.transform;

        meshCount = model.meshCount;
        materialCount = model.materialCount;
        meshes = model.meshes;
        materials = model.materials;
        meshMaterial = model.meshMaterial;

        boneCount = model.boneCount;
        bones = model.bones;
        bindPose = model.bindPose;
    }
};

}  // namespace raylib

using RModel = raylib::Model;

#endif  // RAYLIB_CPP_INCLUDE_MODEL_HPP_
