#ifndef MESH_UTILS_HPP
#define MESH_UTILS_HPP
#include "Merlin/Render/mesh.hpp"
#include "Merlin/Render/mesh_vertex.hpp"
#include "Merlin/Render/renderer.hpp"

namespace Merlin
{
    struct TangentFrame
    {
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 bitangent;

        TangentFrame()
            : normal(glm::vec3(0.0f))
            , tangent(glm::vec3(0.0f))
            , bitangent(glm::vec3(0.0f))
        {
        }
    };

    TangentFrame TriangleFrame(const glm::vec3& p1,
                               const glm::vec3& p2,
                               const glm::vec3& p3,
                               const glm::vec2& uv1,
                               const glm::vec2& uv2,
                               const glm::vec2& uv3);

    void CalculateTangentFrame(
        const std::shared_ptr<Mesh<Vertex_XNTBUV>>& mesh);

}  // namespace Merlin
#endif