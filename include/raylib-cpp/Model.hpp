#ifndef RAYLIB_CPP_INCLUDE_MODEL_HPP_
#define RAYLIB_CPP_INCLUDE_MODEL_HPP_

#include <string>
#include <filesystem>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"

#include "./Mesh.hpp"
#include "./ModelAnimation.hpp"
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
    inline static constexpr float DefaultDrawScale = 1.0f;
    inline static constexpr ::Color DefaultDrawTintColor = WHITE;
    inline static constexpr float DefaultDrawRotationAngle = 0.0f;
    inline static constexpr ::Vector3 DefaultDrawScaleVector = {1.0f, 1.0f, 1.0f};

    constexpr Model() = default;

    explicit constexpr Model(owner<const ::Model&> model) = delete;
    explicit constexpr Model(owner<::Model&&> model) noexcept {
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
    Model(const ::Mesh& mesh) = delete;
    Model(::Mesh&& mesh) RAYLIB_CPP_THROWS {
        Load(std::move(mesh));
    }
    Model(const raylib::Mesh& mesh) = delete;
    Model(raylib::Mesh&& mesh) RAYLIB_CPP_THROWS {
        Load(std::move(mesh));
    }

    ~Model() noexcept {
        Unload();
    }

    constexpr Model(const Model&) = delete;
    Model(Model&& other) noexcept {
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
    constexpr Model& operator=(owner<::Model&&> model) noexcept {
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

    explicit operator ::Model() const noexcept {
        return m_data;
    }
    [[nodiscard]] ::Model c_raylib() const & noexcept {
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

    constexpr MeshUnmanaged GetMesh() const { return m_data.meshes != nullptr ? MeshUnmanaged{*m_data.meshes} : MeshUnmanaged{}; }

    /**
     * Unload model (including meshes) from memory (RAM and/or VRAM)
     */
    void Unload() noexcept {
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
     * Check model animation skeleton match
     */
    [[nodiscard]] bool IsModelAnimationValid(const ::ModelAnimation& anim) const noexcept {
        return ::IsModelAnimationValid(m_data, anim);
    }
    [[nodiscard]] bool IsModelAnimationValid(const raylib::ModelAnimation& anim) const {
        return ::IsModelAnimationValid(m_data, anim.c_raylib());
    }

    /**
     * Draw a model (with texture if set)
     */
    void Draw(::Vector3 position,
            float scale = DefaultDrawScale,
            ::Color tint = DefaultDrawTintColor) const noexcept {
        ::DrawModel(m_data, position, scale, tint);
    }

    /**
     * Draw a model with extended parameters
     */
    void Draw(
            ::Vector3 position,
            ::Vector3 rotationAxis,
            float rotationAngle = DefaultDrawRotationAngle,
            ::Vector3 scale = DefaultDrawScaleVector,
            ::Color tint = DefaultDrawTintColor) const noexcept {
        ::DrawModelEx(m_data, position, rotationAxis, rotationAngle, scale, tint);
    }

    /**
     * Draw a model wires (with texture if set)
     */
    void DrawWires(::Vector3 position,
            float scale = DefaultDrawScale,
            ::Color tint = DefaultDrawTintColor) const noexcept {
        ::DrawModelWires(m_data, position, scale, tint);
    }

    /**
     * Draw a model wires (with texture if set) with extended parameters
     */
    void DrawWires(
            ::Vector3 position,
            ::Vector3 rotationAxis,
            float rotationAngle = DefaultDrawRotationAngle,
            ::Vector3 scale = DefaultDrawScaleVector,
            ::Color tint = DefaultDrawTintColor) const noexcept {
        ::DrawModelWiresEx(m_data, position, rotationAxis, rotationAngle, scale, tint);
    }

    /**
     * Compute model bounding box limits (considers all meshes)
     */
    [[nodiscard]] raylib::BoundingBox GetBoundingBox() const noexcept {
        return raylib::BoundingBox{::GetModelBoundingBox(m_data)};
    }

    /**
     * Compute model bounding box limits (considers all meshes)
     */
    explicit operator raylib::BoundingBox() const noexcept {
        return raylib::BoundingBox{::GetModelBoundingBox(m_data)};
    }

    /**
     * Determines whether or not the Model has data in it.
     */
    [[nodiscard]] bool IsReady() const noexcept {
        return ::IsModelReady(m_data);
    }

    [[nodiscard]] bool IsValidAnimation(const ::ModelAnimation& modelAnimation) const noexcept {
        return ::IsModelAnimationValid(m_data, modelAnimation);
    }
    [[nodiscard]] bool IsValidAnimation(const raylib::ModelAnimation& modelAnimation) const noexcept {
        return IsValidAnimation(modelAnimation.c_raylib());
    }

    /**
     * Update model animation pose
     */
    Model& UpdateAnimation(const ::ModelAnimation& modelAnimation, int frame) noexcept {
        ::UpdateModelAnimation(m_data, modelAnimation, frame);
        return *this;
    }
    Model& UpdateAnimation(const raylib::ModelAnimation& modelAnimation, int frame) {
        return UpdateAnimation(modelAnimation.c_raylib(), frame);
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
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const ::Mesh& mesh) = delete;
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(::Mesh&& mesh) RAYLIB_CPP_THROWS {
        set(::LoadModelFromMesh(mesh));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Model from Mesh"));
        }

        mesh.vertexCount = 0;
        mesh.triangleCount = 0;
        mesh.vertices = nullptr;
        mesh.texcoords = nullptr;
        mesh.texcoords2 = nullptr;
        mesh.normals = nullptr;
        mesh.tangents = nullptr;
        mesh.colors = nullptr;
        mesh.indices = nullptr;
        mesh.animVertices = nullptr;
        mesh.animNormals = nullptr;
        mesh.boneIds = nullptr;
        mesh.boneWeights = nullptr;
        mesh.vaoId = 0;
        mesh.vboId = nullptr;

        RAYLIB_CPP_RETURN_EXPECTED();
    }

    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const raylib::Mesh& mesh) = delete;
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(raylib::Mesh&& mesh) RAYLIB_CPP_THROWS {
        set(::LoadModelFromMesh(mesh.m_mesh.m_data));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Model from Mesh"));
        }

        mesh.m_mesh.m_data.vertexCount = 0;
        mesh.m_mesh.m_data.triangleCount = 0;
        mesh.m_mesh.m_data.vertices = nullptr;
        mesh.m_mesh.m_data.texcoords = nullptr;
        mesh.m_mesh.m_data.texcoords2 = nullptr;
        mesh.m_mesh.m_data.normals = nullptr;
        mesh.m_mesh.m_data.tangents = nullptr;
        mesh.m_mesh.m_data.colors = nullptr;
        mesh.m_mesh.m_data.indices = nullptr;
        mesh.m_mesh.m_data.animVertices = nullptr;
        mesh.m_mesh.m_data.animNormals = nullptr;
        mesh.m_mesh.m_data.boneIds = nullptr;
        mesh.m_mesh.m_data.boneWeights = nullptr;
        mesh.m_mesh.m_data.vaoId = 0;
        mesh.m_mesh.m_data.vboId = nullptr;

        RAYLIB_CPP_RETURN_EXPECTED();
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
