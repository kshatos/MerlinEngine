#ifndef MESH_VERTEX_HPP
#define MESH_VERTEX_HPP
#include <glm/glm.hpp>
#include "Merlin/Render/buffer_layout.hpp"
#include "Merlin/Render/vertex_array.hpp"
#include "Merlin/Render/mesh.hpp"


namespace Merlin
{
    struct Vertex_XNUV
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;

        Vertex_XNUV() {}
        Vertex_XNUV(
            float px, float py, float pz,
            float nx, float ny, float nz,
            float u, float v) :
            position(glm::vec3(px, py, pz)),
            normal(glm::vec3(nx, ny, nz)),
            uv(glm::vec2(u, v)) {}

        static BufferLayout GetLayout()
        {
            return BufferLayout{
                {ShaderDataType::Float3, "a_Position"},
                {ShaderDataType::Float3, "a_Normal"},
                {ShaderDataType::Float2, "a_TexCoord"}
            };
        }
    };

    struct Vertex_XNTBUV
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 bitangent;
        glm::vec2 uv;

        Vertex_XNTBUV() :
            position(glm::vec3(0.0f, 0.0f, 0.0f)),
            normal(glm::vec3(0.0f, 0.0f, 0.0f)),
            tangent(glm::vec3(0.0f, 0.0f, 0.0f)),
            bitangent(glm::vec3(0.0f, 0.0f, 0.0f)),
            uv(glm::vec2(0.0f, 0.0f)) {}

        Vertex_XNTBUV(
            float px, float py, float pz,
            float nx, float ny, float nz,
            float tx, float ty, float tz,
            float bx, float by, float bz,
            float u, float v) :
            position(glm::vec3(px, py, pz)),
            normal(glm::vec3(nx, ny, nz)),
            tangent(glm::vec3(tx, ty, tz)),
            bitangent(glm::vec3(bx, by, bz)),
            uv(glm::vec2(u, v)) {}

        static BufferLayout GetLayout()
        {
            return BufferLayout{
                {ShaderDataType::Float3, "a_Position"},
                {ShaderDataType::Float3, "a_Normal"},
                {ShaderDataType::Float3, "a_Tangent"},
                {ShaderDataType::Float3, "a_Bitangent"},
                {ShaderDataType::Float2, "a_TexCoord"}
            };
        }
    };

    template<typename VertexType>
    void CalculateUVFrame(const std::shared_ptr<Mesh<VertexType>>& mesh) {}

    template<>
    void CalculateUVFrame<Vertex_XNTBUV>(
        const std::shared_ptr<Mesh<Vertex_XNTBUV>>& mesh)
    {
        for (int tri_index = 0; tri_index < mesh->GetTriangleCount(); tri_index++)
        {
            auto& p1 = mesh->GetVertex(mesh->GetIndex(tri_index, 0));
            auto& p2 = mesh->GetVertex(mesh->GetIndex(tri_index, 1));
            auto& p3 = mesh->GetVertex(mesh->GetIndex(tri_index, 2));

            auto E1 = p2.position - p1.position;
            auto E2 = p3.position - p1.position;

            auto DUV1 = p2.uv - p1.uv;
            auto DUV2 = p3.uv - p1.uv;

            float detUV = 1.0f / (DUV1.x * DUV2.y - DUV2.x * DUV1.y);

            glm::vec3 tangent{
                detUV * (DUV2.y * E1.x - DUV1.y * E2.x),
                detUV * (DUV2.y * E1.y - DUV1.y * E2.y),
                detUV * (DUV2.y * E1.z - DUV1.y * E2.z)
            };
            glm::vec3 bitangent{
                detUV * (-DUV2.x * E1.x + DUV1.x * E2.x),
                detUV * (-DUV2.x * E1.y + DUV1.x * E2.y),
                detUV * (-DUV2.x * E1.z + DUV1.x * E2.z)
            };

            p1.tangent = tangent;
            p2.tangent = tangent;
            p3.tangent = tangent;

            p1.bitangent = bitangent;
            p2.bitangent = bitangent;
            p3.bitangent = bitangent;
        }
    }

    template<typename VertexType>
    std::shared_ptr<VertexArray> UploadMesh(std::shared_ptr<Mesh<VertexType>> mesh)
    {
        BufferLayout layout = VertexType::GetLayout();

        auto vbuffer = VertexBuffer::Create(
            mesh->GetVertexDataPointer(),
            mesh->GetVertexCount() * sizeof(VertexType));
        vbuffer->SetLayout(layout);

        auto ibuffer = IndexBuffer::Create(
            mesh->GetIndexDataPointer(),
            mesh->GetTriangleCount() * 3);

        auto varray = VertexArray::Create();
        varray->AddVertexBuffer(vbuffer);
        varray->SetIndexBuffer(ibuffer);

        return varray;
    }

}
#endif