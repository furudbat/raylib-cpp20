#ifndef RAYLIB_CPP_INCLUDE_MESH_HPP_
#define RAYLIB_CPP_INCLUDE_MESH_HPP_

#include <string>
#include <vector>
#include <filesystem>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "./BoundingBox.hpp"
#include "./Image.hpp"
#include "./MeshUnmanaged.hpp"

namespace raylib {

/**
 * Vertex data definning a mesh
 */
class Mesh {
 public:
    constexpr Mesh() = default;

    constexpr explicit Mesh(owner<const ::Mesh&> mesh) = delete;
    constexpr explicit Mesh(owner<::Mesh&&> mesh) {
        m_mesh.set(mesh);

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
        m_mesh.set(other.m_mesh.m_data);

        other.m_mesh.m_data.vertexCount = 0;
        other.m_mesh.m_data.triangleCount = 0;
        other.m_mesh.m_data.vertices = nullptr;
        other.m_mesh.m_data.texcoords = nullptr;
        other.m_mesh.m_data.texcoords2 = nullptr;
        other.m_mesh.m_data.normals = nullptr;
        other.m_mesh.m_data.tangents = nullptr;
        other.m_mesh.m_data.colors = nullptr;
        other.m_mesh.m_data.indices = nullptr;
        other.m_mesh.m_data.animVertices = nullptr;
        other.m_mesh.m_data.animNormals = nullptr;
        other.m_mesh.m_data.boneIds = nullptr;
        other.m_mesh.m_data.boneWeights = nullptr;
        other.m_mesh.m_data.vaoId = 0;
        other.m_mesh.m_data.vboId = nullptr;
    }

    explicit operator ::Mesh() const {
        return m_mesh.m_data;
    }
    [[nodiscard]] ::Mesh c_raylib() const & {
        return m_mesh.c_raylib();
    }

    GETTER(int, VertexCount, m_mesh.m_data.vertexCount)
    GETTER(int, TriangleCount, m_mesh.m_data.triangleCount)
    CONST_GETTER(float*, Vertices, m_mesh.m_data.vertices)
    CONST_GETTER(float *, TexCoords, m_mesh.m_data.texcoords)
    CONST_GETTER(float *, TexCoords2, m_mesh.m_data.texcoords2)
    CONST_GETTER(float *, Normals, m_mesh.m_data.normals)
    CONST_GETTER(float *, Tangents, m_mesh.m_data.tangents)
    CONST_GETTER(unsigned char *, Colors, m_mesh.m_data.colors)
    CONST_GETTER(unsigned short *, Indices, m_mesh.m_data.indices) // NOLINT
    CONST_GETTER(float *, AnimVertices, m_mesh.m_data.animVertices)
    CONST_GETTER(float *, AnimNormals, m_mesh.m_data.animNormals)
    CONST_GETTER(unsigned char *, BoneIds, m_mesh.m_data.boneIds)
    CONST_GETTER(float *, BoneWeights, m_mesh.m_data.boneWeights)
    GETTER(unsigned int, VaoId, m_mesh.m_data.vaoId)
    CONST_GETTER(unsigned int *, VboId, m_mesh.m_data.vboId)

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

    constexpr Mesh& operator=(const ::Mesh& mesh) = delete;
    constexpr Mesh& operator=(::Mesh&& mesh) {
        m_mesh.set(mesh);

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

        return *this;
    }

    constexpr Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        m_mesh.set(other.m_mesh.m_data);

        other.m_mesh.m_data.vertexCount = 0;
        other.m_mesh.m_data.triangleCount = 0;
        other.m_mesh.m_data.vertices = nullptr;
        other.m_mesh.m_data.texcoords = nullptr;
        other.m_mesh.m_data.texcoords2 = nullptr;
        other.m_mesh.m_data.normals = nullptr;
        other.m_mesh.m_data.tangents = nullptr;
        other.m_mesh.m_data.colors = nullptr;
        other.m_mesh.m_data.indices = nullptr;
        other.m_mesh.m_data.animVertices = nullptr;
        other.m_mesh.m_data.animNormals = nullptr;
        other.m_mesh.m_data.boneIds = nullptr;
        other.m_mesh.m_data.boneWeights = nullptr;
        other.m_mesh.m_data.vaoId = 0;
        other.m_mesh.m_data.vboId = nullptr;

        return *this;
    }

    ~Mesh() {
        Unload();
    }

    /**
     * Unload mesh from memory (RAM and/or VRAM)
     */
    void Unload() {
        if (m_mesh.m_data.vboId != nullptr) {
            ::UnloadMesh(m_mesh.m_data);
            m_mesh.m_data.vboId = nullptr;
        }
    }

    /**
     * Compute mesh bounding box limits
     */
    explicit operator raylib::BoundingBox() const {
        return m_mesh.BoundingBox();
    }


    /**
     * Load model from generated mesh
     */
    /*
    [[nodiscard]] raylib::Model LoadFromMesh() const {
        return raylib::Model{::LoadModelFromMesh(m_mesh.m_data)};
    }
    */

    /**
     * Load model from generated mesh
     */
    /*
    explicit operator raylib::Model() const {
        return raylib::Model{::LoadModelFromMesh(m_mesh.m_data)};
    }
    */

    COMPOSITION_METHODE_CALL_RETURN_THIS(Upload, m_mesh)
    COMPOSITION_METHODE_CALL_RETURN_THIS(UpdateBuffer, m_mesh)
    COMPOSITION_METHODE_CALL_RETURN_THIS(Draw, m_mesh)

    CONST_COMPOSITION_METHODE_CALL(Export, m_mesh)
    CONST_COMPOSITION_METHODE_CALL(BoundingBox, m_mesh)

    COMPOSITION_METHODE_CALL_RETURN_THIS(GenTangents, m_mesh)

 protected:
    MeshUnmanaged m_mesh;

    friend class Model;
};
}  // namespace raylib

using RMesh = raylib::Mesh;

#endif  // RAYLIB_CPP_INCLUDE_MESH_HPP_
