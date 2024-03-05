#ifndef RAYLIB_CPP_INCLUDE_MESH_HPP_
#define RAYLIB_CPP_INCLUDE_MESH_HPP_

#include <string>
#include <vector>
#include <filesystem>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "./BoundingBox.hpp"
#include "./Image.hpp"

namespace raylib {

/**
 * Vertex data definning a mesh
 */
class Mesh {
 public:
    constexpr Mesh() {
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

    constexpr explicit Mesh(owner<const ::Mesh&> mesh) = delete;
    constexpr explicit Mesh(owner<::Mesh&&> mesh) {
        set(mesh);

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
    }

    constexpr Mesh(const Mesh&) = delete;
    constexpr Mesh(Mesh&& other) {
        set(other.m_data);

        other.m_data.vertexCount = 0;
        other.m_data.triangleCount = 0;
        other.m_data.vertices = nullptr;
        other.m_data.texcoords = nullptr;
        other.m_data.texcoords2 = nullptr;
        other.m_data.normals = nullptr;
        other.m_data.tangents = nullptr;
        other.m_data.colors = nullptr;
        other.m_data.indices = nullptr;
        other.m_data.animVertices = nullptr;
        other.m_data.animNormals = nullptr;
        other.m_data.boneIds = nullptr;
        other.m_data.boneWeights = nullptr;
        other.m_data.vaoId = 0;
        other.m_data.vboId = nullptr;
    }

    //explicit operator ::Mesh() const {
    //    return m_data;
    //}
    [[nodiscard]] ::Mesh c_raylib() const & {
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
     * Generate polygonal mesh
     */
    static raylib::Mesh Poly(int sides, float radius) {
        return raylib::Mesh{::GenMeshPoly(sides, radius)};
    }

    /**
     * Generate plane mesh (with subdivisions)
     */
    static raylib::Mesh Plane(float width, float length, int resX, int resZ) {
        return raylib::Mesh{::GenMeshPlane(width, length, resX, resZ)};
    }

    /**
     * Generate cuboid mesh
     */
    static raylib::Mesh Cube(float width, float height, float length) {
        return raylib::Mesh{::GenMeshCube(width, height, length)};
    }

    /**
     * Generate sphere mesh (standard sphere)
     */
    static raylib::Mesh Sphere(float radius, int rings, int slices) {
        return raylib::Mesh{::GenMeshSphere(radius, rings, slices)};
    }

    /**
     * Generate half-sphere mesh (no bottom cap)
     */
    static raylib::Mesh HemiSphere(float radius, int rings, int slices) {
        return raylib::Mesh{::GenMeshHemiSphere(radius, rings, slices)};
    }

    /**
     * Generate cylinder mesh
     */
    static raylib::Mesh Cylinder(float radius, float height, int slices) {
        return raylib::Mesh{::GenMeshCylinder(radius, height, slices)};
    }

    /**
     * Generate cone/pyramid mesh
     */
    static raylib::Mesh Cone(float radius, float height, int slices) {
        return raylib::Mesh{::GenMeshCone(radius, height, slices)};
    }

    /**
     * Generate torus mesh
     */
    static raylib::Mesh Torus(float radius, float size, int radSeg, int sides) {
        return raylib::Mesh{::GenMeshTorus(radius, size, radSeg, sides)};
    }

    /**
     * Generate trefoil knot mesh
     */
    static raylib::Mesh Knot(float radius, float size, int radSeg, int sides) {
        return raylib::Mesh{::GenMeshKnot(radius, size, radSeg, sides)};
    }

    /**
     * Generate heightmap mesh from image data
     */
    static raylib::Mesh Heightmap(const ::Image& heightmap, ::Vector3 size) {
        return raylib::Mesh{::GenMeshHeightmap(heightmap, size)};
    }
    static raylib::Mesh Heightmap(const raylib::Image& heightmap, ::Vector3 size) {
        return raylib::Mesh{::GenMeshHeightmap(heightmap.c_raylib(), size)};
    }

    /**
     * Generate cubes-based map mesh from image data
     */
    static raylib::Mesh Cubicmap(const ::Image& cubicmap, ::Vector3 cubeSize) {
        return raylib::Mesh{::GenMeshCubicmap(cubicmap, cubeSize)};
    }
    static raylib::Mesh Cubicmap(const raylib::Image& cubicmap, ::Vector3 cubeSize) {
        return raylib::Mesh{::GenMeshCubicmap(cubicmap.c_raylib(), cubeSize)};
    }

    constexpr Mesh& operator=(const ::Mesh& mesh) {
        set(mesh);
        return *this;
    }

    constexpr Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other.m_data);

        other.m_data.vertexCount = 0;
        other.m_data.triangleCount = 0;
        other.m_data.vertices = nullptr;
        other.m_data.texcoords = nullptr;
        other.m_data.texcoords2 = nullptr;
        other.m_data.normals = nullptr;
        other.m_data.tangents = nullptr;
        other.m_data.colors = nullptr;
        other.m_data.indices = nullptr;
        other.m_data.animVertices = nullptr;
        other.m_data.animNormals = nullptr;
        other.m_data.boneIds = nullptr;
        other.m_data.boneWeights = nullptr;
        other.m_data.vaoId = 0;
        other.m_data.vboId = nullptr;

        return *this;
    }

    ~Mesh() {
        Unload();
    }

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
     * Unload mesh from memory (RAM and/or VRAM)
     */
    void Unload() {
        if (m_data.vboId != nullptr) {
            ::UnloadMesh(m_data);
            m_data.vboId = nullptr;
        }
    }

    /**
     * Compute mesh bounding box limits
     */
    [[nodiscard]] raylib::BoundingBox BoundingBox() const {
        return raylib::BoundingBox{::GetMeshBoundingBox(m_data)};
    }

    /**
     * Compute mesh bounding box limits
     */
    explicit operator raylib::BoundingBox() const {
        return BoundingBox();
    }

    /**
     * Compute mesh tangents
     */
    Mesh& GenTangents() {
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
    constexpr void set(const ::Mesh& mesh) {
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
};
}  // namespace raylib

using RMesh = raylib::Mesh;

#endif  // RAYLIB_CPP_INCLUDE_MESH_HPP_
