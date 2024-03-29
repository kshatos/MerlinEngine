#include "Merlin/Render/skybox.hpp"

#include "Merlin/Render/mesh.hpp"
#include "Merlin/Render/mesh_utils.hpp"
#include "Merlin/Render/mesh_vertex.hpp"

namespace Merlin
{
    static const Vertex_XNUV cubemap_verts[]{
        {-1.0, -1.0, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0},
        {1.0, -1.0, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0},
        {1.0, 1.0, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0},
        {-1.0, 1.0, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0},

        {1.0, -1.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0},
        {1.0, -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0},
        {1.0, 1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0},
        {1.0, 1.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0},

        {1.0, -1.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0},
        {-1.0, -1.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0},
        {-1.0, 1.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0},
        {1.0, 1.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0},

        {-1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 0.0},
        {-1.0, -1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0},
        {-1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0},
        {-1.0, 1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 0.0},

        {-1.0, 1.0, 1.0, 0.0, -1.0, 0.0, 0.0, 0.0},
        {1.0, 1.0, 1.0, 0.0, -1.0, 0.0, 0.0, 0.0},
        {1.0, 1.0, -1.0, 0.0, -1.0, 0.0, 0.0, 0.0},
        {-1.0, 1.0, -1.0, 0.0, -1.0, 0.0, 0.0, 0.0},

        {1.0, -1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0},
        {-1.0, -1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0},
        {-1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0},
        {1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0},
    };

    static const uint32_t cubemap_triangles[]{
        0,  3,  2,  0,  2,  1,

        4,  7,  6,  4,  6,  5,

        8,  11, 10, 8,  10, 9,

        12, 15, 14, 12, 14, 13,

        16, 19, 18, 16, 18, 17,

        20, 23, 22, 20, 22, 21,
    };

    Skybox::Skybox(const std::shared_ptr<Cubemap>& cubemap, float scale)
        : m_cubemap(cubemap)
    {
        auto mesh = std::make_shared<Mesh<Vertex_XNUV>>();
        mesh->SetVertexData(cubemap_verts,
                            sizeof(cubemap_verts) / sizeof(Vertex_XNUV));
        mesh->SetIndexData(cubemap_triangles,
                           sizeof(cubemap_triangles) / sizeof(uint32_t));
        for (int i = 0; i < mesh->GetVertexCount(); i++)
        {
            mesh->GetVertex(i).position *= scale;
        }
        m_mbuffer = Renderer::CreateMeshBuffer(
            mesh->GetVertexDataPointer(),
            mesh->GetVertexCount() * sizeof(Vertex_XNUV),
            mesh->GetIndexDataPointer(),
            mesh->GetTriangleCount() * 3,
            Vertex_XNUV::GetLayout());
    }

}  // namespace Merlin