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

    template<typename VertexType>
    std::shared_ptr<VertexArray> UploadMesh(Mesh<VertexType> mesh)
    {
        BufferLayout layout = VertexType::GetLayout();

        auto vbuffer = VertexBuffer::Create(
            mesh.GetVertexDataPointer(),
            mesh.GetVertexCount() * sizeof(VertexType));
        vbuffer->SetLayout(layout);

        auto ibuffer = IndexBuffer::Create(
            mesh.GetIndexDataPointer(),
            mesh.GetTriangleCount() * 3);

        auto varray = VertexArray::Create();
        varray->AddVertexBuffer(vbuffer);
        varray->SetIndexBuffer(ibuffer);

        return varray;
    }

}
#endif