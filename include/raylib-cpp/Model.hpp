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

#include <unordered_map>

namespace raylib {

class Mesh;

enum class ModelMaterialOptions : uint8_t {
    None = 0,                                          ///< Do Nothing when UnloadingModel, manage materials and shaders on your own
    UnloadMaterial = 1,                                ///< UnloadMaterial: assume shader ownership is in Model (Material), shader gets unloaded by UnloadMaterial
    UnbindShader = 2,                                  ///< Unbind (disconnect) shader from Model (Material), to avoid UnloadMaterial() trying to unload it automatically (NO UnloadMaterial)
    UnbindShaderBeforeUnloadAndUnloadMaterial  = 3,    ///< UnloadMaterial AND Unbind (disconnect) shader from Model (Material), to avoid UnloadMaterial() trying to unload it automatically (manage shader ownership on your own)
};

enum class ModelMaterialShaderOption : uint8_t {
    NoUnload = 0,                                      ///< Do Nothing when UnloadingModel, manage materials and shaders on your own
    UnloadMaterial = 1,                                ///< UnloadMaterial: assume shader ownership is in Model (Material), shader gets unloaded by UnloadMaterial
    UnbindShader = 2,                                  ///< Unbind (disconnect) shader from Model (Material), to avoid UnloadMaterial() trying to unload it automatically (NO UnloadMaterial)
    UnbindShaderBeforeUnloadAndUnloadMaterial  = 3,    ///< UnloadMaterial AND Unbind (disconnect) shader from Model (Material), to avoid UnloadMaterial() trying to unload it automatically (manage shader ownership on your own)
};

enum class ModelMaterialTextureOption : uint8_t {
    NoUnload = 0,                       ///< Do Nothing when UnloadingModel, manage materials and shaders on your own
    UnloadMaterial = 1,             ///< UnloadMaterial: assume texture ownership is in Model (Material), texture gets unloaded by UnloadMaterial
};


/**
 * Model type
 */
class Model {
 public:
    inline static constexpr float DefaultDrawScale = 1.0f;
    inline static constexpr ::Color DefaultDrawTintColor = WHITE;
    inline static constexpr float DefaultDrawRotationAngle = 0.0f;
    inline static constexpr ::Vector3 DefaultDrawScaleVector = {1.0f, 1.0f, 1.0f};

    Model() = default;

    explicit Model(owner<const ::Model&> model) = delete;
    explicit Model(owner<::Model&&> model) noexcept {
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

    Model(const Model&) = delete;
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

    Model& operator=(owner<const ::Model&> model) = delete;
    Model& operator=(owner<::Model&&> model) noexcept {
        set(model);
        m_trackMaterialOwnership.clear();

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
        std::exchange(m_trackMaterialOwnership, other.m_trackMaterialOwnership);

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

    GETTERSETTER(::Matrix, Transform, m_data.transform)
    GETTER(int, MeshCount, m_data.meshCount)
    GETTER(int, MaterialCount, m_data.materialCount)
    SPAN_GETTER(::Mesh, Meshes, m_data.meshes, m_data.meshCount)
    SPAN_GETTER(::Material, Materials, m_data.materials, m_data.materialCount)
    //CONST_GETTER(const int*, MeshMaterial, m_data.meshMaterial)
    //GETTER(int*, MeshMaterial, m_data.meshMaterial)
    SPAN_GETTER(int, MeshMaterial, m_data.meshMaterial, m_data.meshCount)
    GETTER(int, BoneCount, m_data.boneCount)
    SPAN_GETTER(::BoneInfo, Bones, m_data.bones, m_data.boneCount)
    CONST_GETTER(::Transform*, BindPose, m_data.bindPose)

    constexpr ::Mesh& GetMesh(size_t index) {
        assert(index < m_data.meshCount);
        return m_data.meshes[index];
    }
    constexpr const ::Mesh& GetMesh(size_t index) const {
        assert(index < m_data.meshCount);
        return m_data.meshes[index];
    }

    constexpr ::Material& GetMaterial(size_t index) {
        assert(index < m_data.materialCount);
        return m_data.materials[index];
    }
    constexpr const ::Material& GetMaterial(size_t index) const {
        assert(index < m_data.materialCount);
        return m_data.materials[index];
    }

    constexpr int& GetMeshMaterial(size_t index) {
        return m_data.meshMaterial[index];
    }
    constexpr int GetMeshMaterial(size_t index) const {
        return m_data.meshMaterial[index];
    }

    /// Set Shader and Material Management
    void SetMaterialShader(size_t material_index, ::Shader shader, ModelMaterialShaderOption option = ModelMaterialShaderOption::NoUnload) {
        assert(material_index < m_data.materialCount);
        m_data.materials[material_index].shader = shader;
        trackMaterialOwnership(material_index, option);
    }
    void SetMaterialShader(size_t material_index, ::Shader&& shader, ModelMaterialShaderOption option) {
        assert(material_index < m_data.materialCount);
        m_data.materials[material_index].shader = shader;
        trackMaterialOwnership(material_index, option);
        switch(m_trackMaterialOwnership[material_index]) {
            case ModelMaterialOptions::None:
                break;
            case ModelMaterialOptions::UnloadMaterial:
                // ownership of shader moved into material
                shader.id = rlGetShaderIdDefault();
                shader.locs = nullptr;
                break;
            case ModelMaterialOptions::UnbindShader:
            case ModelMaterialOptions::UnbindShaderBeforeUnloadAndUnloadMaterial:
                // nothing to move, user should manage shader on its own
                break;
        }
    }
    void SetMaterialShader(size_t material_index, const raylib::Shader& shader, ModelMaterialShaderOption option = ModelMaterialShaderOption::NoUnload) {
        assert(material_index < m_data.materialCount);
        m_data.materials[material_index].shader = shader.c_raylib();
        trackMaterialOwnership(material_index, option);
    }
    // move shader ownership into model/materials (Material also gets automatically unloaded when Unload)
    void MoveMaterialShader(size_t material_index, raylib::Shader&& shader) {
        assert(material_index < m_data.materialCount);
        m_data.materials[material_index].shader = shader.c_raylib();
        trackMaterialOwnership(material_index, ModelMaterialShaderOption::UnloadMaterial);
        shader.m_shader.m_data.id = rlGetShaderIdDefault();
        shader.m_shader.m_data.locs = nullptr;
    }
    void SetMaterialShader(size_t material_index, ::Shader&& shader) {
        assert(material_index < m_data.materialCount);
        m_data.materials[material_index].shader = shader;
        trackMaterialOwnership(material_index, ModelMaterialShaderOption::UnloadMaterial);
        shader.id = rlGetShaderIdDefault();
        shader.locs = nullptr;
    }

    template<class... Args>
    void EmplaceMaterialShader(size_t material_index, Args&&... shader_args) {
        MoveMaterialShader(material_index, Shader(std::forward<Args>(shader_args)...));
    }

    /// Set Texture and Material Management
    void SetMaterialMapTexture(size_t material_index, size_t map_index, ::Texture texture, ModelMaterialTextureOption option = ModelMaterialTextureOption::NoUnload) {
        assert(material_index < m_data.materialCount);
        m_data.materials[material_index].maps[map_index].texture = texture;
        trackMaterialOwnership(material_index, option);
    }
    void SetMaterialMapTexture(size_t material_index, size_t map_index, const raylib::Texture& texture, ModelMaterialTextureOption option) {
        assert(material_index < m_data.materialCount);
        m_data.materials[material_index].maps[map_index].texture = texture.c_raylib();
        trackMaterialOwnership(material_index, option);
    }
    void SetMaterialMapTexture(size_t material_index, size_t map_index, ::Texture&& texture, ModelMaterialTextureOption option) {
        assert(material_index < m_data.materialCount);
        m_data.materials[material_index].maps[map_index].texture = texture;
        trackMaterialOwnership(material_index, option);
        switch(m_trackMaterialOwnership[material_index]) {
            case ModelMaterialOptions::None:
                break;
            case ModelMaterialOptions::UnloadMaterial:
            case ModelMaterialOptions::UnbindShaderBeforeUnloadAndUnloadMaterial:
                // ownership of texture moved into maps
                texture.id = 0;
                texture.format = 0;
                texture.width = 0;
                texture.height = 0;
                texture.mipmaps = 0;
                break;
            case ModelMaterialOptions::UnbindShader:
                // only impact shader, manage texture by user
                break;
        }
    }


    void SetMaterialManagement(size_t material_index, ModelMaterialOptions options) {
        m_trackMaterialOwnership[material_index] = options;
    }

    /**
     * Unload model (including meshes) from memory (RAM and/or VRAM)
     */
    void Unload() noexcept {
        if (m_data.meshes != nullptr || m_data.materials != nullptr) {
            for(const auto& [material_index , option] : m_trackMaterialOwnership) {
                switch(option) {
                    case ModelMaterialOptions::None:
                        // do nothing
                        break;
                    case ModelMaterialOptions::UnloadMaterial:
                        // UnloadMaterial, should also unload shader
                        UnloadMaterial(m_data.materials[material_index]);
                        // maps also geta unloaded in UnloadMaterial
                        m_data.materials[material_index].maps = nullptr;
                        break;
                    case ModelMaterialOptions::UnbindShader:
                        m_data.materials[material_index].shader.id = rlGetShaderIdDefault();
                        m_data.materials[material_index].shader.locs = nullptr;
                        break;
                    case ModelMaterialOptions::UnbindShaderBeforeUnloadAndUnloadMaterial:
                        m_data.materials[material_index].shader.id = rlGetShaderIdDefault();
                        m_data.materials[material_index].shader.locs = nullptr;
                        // UnloadMaterial, should NOT unload the shader
                        UnloadMaterial(m_data.materials[material_index]);
                        // maps also geta unloaded in UnloadMaterial
                        m_data.materials[material_index].maps = nullptr;
                        break;
                }
            }
            ::UnloadModel(m_data);
            m_data.meshes = nullptr;
            m_data.materials = nullptr;
            m_trackMaterialOwnership.clear();
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

    static RAYLIB_CPP_EXPECTED_RESULT(Model) LoadFromFile(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        Model model (::LoadModel(fileName.c_str()));
        if (!model.IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Model from " + fileName.string()));
        }
        RAYLIB_CPP_RETURN_EXPECTED_VALUE(model);
    }
    static RAYLIB_CPP_EXPECTED_RESULT(Model) LoadFromMesh(::Mesh&& mesh) RAYLIB_CPP_THROWS {
        Model model (::LoadModelFromMesh(mesh));
        if (!model.IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load Model from Mesh"));
        }
        RAYLIB_CPP_RETURN_EXPECTED_VALUE(model);
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

    void trackMaterialOwnership(size_t material_index, ModelMaterialShaderOption option) {
        if (m_trackMaterialOwnership.contains(material_index)) {
            m_trackMaterialOwnership[material_index] = static_cast<ModelMaterialOptions>(static_cast<uint8_t>(m_trackMaterialOwnership[material_index]) | static_cast<uint8_t>(option));
        }

        m_trackMaterialOwnership[material_index] = static_cast<ModelMaterialOptions>(option);
    }
    void trackMaterialOwnership(size_t material_index, ModelMaterialTextureOption option) {
        if (m_trackMaterialOwnership.contains(material_index)) {
            m_trackMaterialOwnership[material_index] = static_cast<ModelMaterialOptions>(static_cast<uint8_t>(m_trackMaterialOwnership[material_index]) | static_cast<uint8_t>(option));
        }

        m_trackMaterialOwnership[material_index] = static_cast<ModelMaterialOptions>(option);
    }

    ::Model m_data;
    std::unordered_map<size_t, ModelMaterialOptions> m_trackMaterialOwnership;
};

}  // namespace raylib

using RModel = raylib::Model;

#endif  // RAYLIB_CPP_INCLUDE_MODEL_HPP_
