#ifndef RAYLIB_CPP_INCLUDE_MESHUNMANAGED_HPP_
#define RAYLIB_CPP_INCLUDE_MESHUNMANAGED_HPP_

#include <string>
#include <vector>
#include <filesystem>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "./BoundingBox.hpp"
#include "./Image.hpp"

namespace raylib {

class Mesh;
class Model;

/**
 * Vertex data defining a mesh, that is not managed by the C++ RAII.
 */
class MeshUnmanaged {
 public:
    constexpr MeshUnmanaged() {
        m_data.vertexCount = 0;
        m_data.triangleCount = 0;
        m_data.vertices = nullptr;
        m_data.texcoords = nullptr;
        m_data.texcoords2 = nullptr;
        m_data.normals = nullptr;
        m_data.tangents = nullptr;
        m_data.colors = nullptr;
        m_data.indices = nullptr;
        m_data.animVertices = nullptr;
        m_data.animNormals = nullptr;
        m_data.boneIds = nullptr;
        m_data.boneWeights = nullptr;
        m_data.vaoId = 0;
        m_data.vboId = nullptr;
    }

    constexpr explicit MeshUnmanaged(const ::Mesh& mesh) {
        set(mesh);
    }

    constexpr MeshUnmanaged(const MeshUnmanaged& other) {
        set(other.m_data);
    }

    constexpr MeshUnmanaged& operator=(const ::Mesh& mesh) {
        set(mesh);
        return *this;
    }

    MeshUnmanaged& operator=(const MeshUnmanaged& other) noexcept {
        set(other.m_data);
        return *this;
    }

    explicit operator ::Mesh() const noexcept {
        return m_data;
    }
    [[nodiscard]] ::Mesh c_raylib() const & noexcept {
        return m_data;
    }

    GETTER(int, VertexCount, m_data.vertexCount)
    GETTER(int, TriangleCount, m_data.triangleCount)
    CONST_GETTER(float*, Vertices, m_data.vertices)
    CONST_GETTER(float *, TexCoords, m_data.texcoords)
    CONST_GETTER(float *, TexCoords2, m_data.texcoords2)
    CONST_GETTER(float *, Normals, m_data.normals)
    CONST_GETTER(float *, Tangents, m_data.tangents)
    CONST_GETTER(unsigned char *, Colors, m_data.colors)
    CONST_GETTER(unsigned short *, Indices, m_data.indices) // NOLINT
    CONST_GETTER(float *, AnimVertices, m_data.animVertices)
    CONST_GETTER(float *, AnimNormals, m_data.animNormals)
    CONST_GETTER(unsigned char *, BoneIds, m_data.boneIds)
    CONST_GETTER(float *, BoneWeights, m_data.boneWeights)
    GETTER(unsigned int, VaoId, m_data.vaoId)
    CONST_GETTER(unsigned int *, VboId, m_data.vboId)

    /**
     * Upload mesh vertex data to GPU (VRAM)
     */
    [[deprecated("Use Upload(UploadOption)")]]
    void Upload(bool dynamic = false) {
        ::UploadMesh(&m_data, dynamic);
    }

    enum class UploadOption : bool { Static = false, Dynamic = true};
    void Upload(UploadOption dynamic = UploadOption::Static) {
        ::UploadMesh(&m_data, dynamic == UploadOption::Dynamic);
    }

    /**
     * Upload mesh vertex data to GPU (VRAM)
     */
    void UpdateBuffer(int index, void *data, int dataSize, int offset = 0) {
        ::UpdateMeshBuffer(m_data, index, data, dataSize, offset);
    }
    /// @FIXME: can't use std::span<void>
    /*
    void UpdateBuffer(int index, std::span<void> data, int offset = 0) {
        ::UpdateMeshBuffer(*this, index, data.data(), data.size(), offset);
    }
    */

    /**
     * Draw a 3d mesh with material and transform
     */
    void Draw(const ::Material& material, const ::Matrix& transform) const {
        ::DrawMesh(m_data, material, transform);
    }

    /**
     * Draw multiple mesh instances with material and different transforms
     */
    void Draw(const ::Material& material, ::Matrix* transforms, int instances) const {
        ::DrawMeshInstanced(m_data, material, transforms, instances);
    }

    /**
     * Export mesh data to file
     *
     * @throws raylib::RaylibException Throws if failed to export the Mesh.
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Export(const std::string& fileName) RAYLIB_CPP_THROWS {
        if (!::ExportMesh(m_data, fileName.c_str())) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to export the Mesh"));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Compute mesh bounding box limits
     */
    [[nodiscard]] raylib::BoundingBox BoundingBox() const noexcept {
        return raylib::BoundingBox{::GetMeshBoundingBox(m_data)};
    }

    /**
     * Compute mesh bounding box limits
     */
    explicit operator raylib::BoundingBox() const noexcept {
        return BoundingBox();
    }

    /**
     * Compute mesh tangents
     */
    MeshUnmanaged& GenTangents() {
        ::GenMeshTangents(&m_data);
        return *this;
    }


    /**
     * Load model from generated mesh
     */
    /*
    [[nodiscard]] raylib::Model LoadFromMesh() const {
        return raylib::Model{::LoadModelFromMesh(m_data)};
    }
    */

    /**
     * Load model from generated mesh
     */
    /*
    explicit operator raylib::Model() const {
        return raylib::Model{::LoadModelFromMesh(m_data)};
    }
    */

 protected:
    constexpr void set(const ::Mesh& mesh) noexcept {
        m_data.vertexCount = mesh.vertexCount;
        m_data.triangleCount = mesh.triangleCount;
        m_data.vertices = mesh.vertices;
        m_data.texcoords = mesh.texcoords;
        m_data.texcoords2 = mesh.texcoords2;
        m_data.normals = mesh.normals;
        m_data.tangents = mesh.tangents;
        m_data.colors = mesh.colors;
        m_data.indices = mesh.indices;
        m_data.animVertices = mesh.animVertices;
        m_data.animNormals = mesh.animNormals;
        m_data.boneIds = mesh.boneIds;
        m_data.boneWeights = mesh.boneWeights;
        m_data.vaoId = mesh.vaoId;
        m_data.vboId = mesh.vboId;
    }

    ::Mesh m_data;

    friend class Mesh;
    friend class Model;
};
}  // namespace raylib

using RMeshUnmanaged = raylib::MeshUnmanaged;

#endif  // RAYLIB_CPP_INCLUDE_MESHUNMANAGED_HPP_
