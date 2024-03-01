#ifndef RAYLIB_CPP_INCLUDE_MESH_HPP_
#define RAYLIB_CPP_INCLUDE_MESH_HPP_

#include <string>
#include <vector>
#include <filesystem>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "./BoundingBox.hpp"
#include "./Model.hpp"

namespace raylib {

/**
 * Vertex data definning a mesh
 */
class Mesh : public ::Mesh {
 public:
    constexpr explicit Mesh(const ::Mesh& mesh) = delete;
    constexpr explicit Mesh(::Mesh&& mesh) {
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
        set(other);

        other.vertexCount = 0;
        other.triangleCount = 0;
        other.vertices = nullptr;
        other.texcoords = nullptr;
        other.texcoords2 = nullptr;
        other.normals = nullptr;
        other.tangents = nullptr;
        other.colors = nullptr;
        other.indices = nullptr;
        other.animVertices = nullptr;
        other.animNormals = nullptr;
        other.boneIds = nullptr;
        other.boneWeights = nullptr;
        other.vaoId = 0;
        other.vboId = nullptr;
    }

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

    /**
     * Generate cubes-based map mesh from image data
     */
    static raylib::Mesh Cubicmap(const ::Image& cubicmap, ::Vector3 cubeSize) {
        return raylib::Mesh{::GenMeshCubicmap(cubicmap, cubeSize)};
    }

    GETTERSETTER(int, VertexCount, vertexCount)
    GETTERSETTER(int, TriangleCount, triangleCount)
    GETTERSETTER(float*, Vertices, vertices)
    GETTERSETTER(float *, TexCoords, texcoords)
    GETTERSETTER(float *, TexCoords2, texcoords2)
    GETTERSETTER(float *, Normals, normals)
    GETTERSETTER(float *, Tangents, tangents)
    GETTERSETTER(unsigned char *, Colors, colors)
    GETTERSETTER(unsigned short *, Indices, indices) // NOLINT
    GETTERSETTER(float *, AnimVertices, animVertices)
    GETTERSETTER(float *, AnimNormals, animNormals)
    GETTERSETTER(unsigned char *, BoneIds, boneIds)
    GETTERSETTER(float *, BoneWeights, boneWeights)
    GETTERSETTER(unsigned int, VaoId, vaoId)
    GETTERSETTER(unsigned int *, VboId, vboId)

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
        set(other);

        other.vertexCount = 0;
        other.triangleCount = 0;
        other.vertices = nullptr;
        other.texcoords = nullptr;
        other.texcoords2 = nullptr;
        other.normals = nullptr;
        other.tangents = nullptr;
        other.colors = nullptr;
        other.indices = nullptr;
        other.animVertices = nullptr;
        other.animNormals = nullptr;
        other.boneIds = nullptr;
        other.boneWeights = nullptr;
        other.vaoId = 0;
        other.vboId = nullptr;

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
        ::UploadMesh(this, dynamic);
    }

    enum class UploadOption : bool { Static = false, Dynamic = true};
    void Upload(UploadOption dynamic = UploadOption::Static) {
        ::UploadMesh(this, dynamic == UploadOption::Dynamic);
    }

    /**
     * Upload mesh vertex data to GPU (VRAM)
     */
    void UpdateBuffer(int index, void *data, int dataSize, int offset = 0) {
        ::UpdateMeshBuffer(*this, index, data, dataSize, offset);
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
        ::DrawMesh(*this, material, transform);
    }

    /**
     * Draw multiple mesh instances with material and different transforms
     */
    void Draw(const ::Material& material, ::Matrix* transforms, int instances) const {
        ::DrawMeshInstanced(*this, material, transforms, instances);
    }

    /**
     * Export mesh data to file
     *
     * @throws raylib::RaylibException Throws if failed to export the Mesh.
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Export(const std::string& fileName) RAYLIB_CPP_THROWS {
        if (!::ExportMesh(*this, fileName.c_str())) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to export the Mesh"));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Unload mesh from memory (RAM and/or VRAM)
     */
    void Unload() {
        if (vboId != nullptr) {
            ::UnloadMesh(*this);
            vboId = nullptr;
        }
    }

    /**
     * Compute mesh bounding box limits
     */
    [[nodiscard]] raylib::BoundingBox BoundingBox() const {
        return raylib::BoundingBox{::GetMeshBoundingBox(*this)};
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
        ::GenMeshTangents(this);
        return *this;
    }

    /**
     * Load model from generated mesh
     */
    [[nodiscard]] raylib::Model LoadModelFrom() const {
        return raylib::Model{::LoadModelFromMesh(*this)};
    }

    /**
     * Load model from generated mesh
     */
    explicit operator raylib::Model() const {
        return raylib::Model{::LoadModelFromMesh(*this)};
    }

 protected:
    constexpr void set(const ::Mesh& mesh) {
        vertexCount = mesh.vertexCount;
        triangleCount = mesh.triangleCount;
        vertices = mesh.vertices;
        texcoords = mesh.texcoords;
        texcoords2 = mesh.texcoords2;
        normals = mesh.normals;
        tangents = mesh.tangents;
        colors = mesh.colors;
        indices = mesh.indices;
        animVertices = mesh.animVertices;
        animNormals = mesh.animNormals;
        boneIds = mesh.boneIds;
        boneWeights = mesh.boneWeights;
        vaoId = mesh.vaoId;
        vboId = mesh.vboId;
    }
};
}  // namespace raylib

using RMesh = raylib::Mesh;

#endif  // RAYLIB_CPP_INCLUDE_MESH_HPP_
