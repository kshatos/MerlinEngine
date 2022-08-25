#include "Merlin/Render/mesh_utils.hpp"

namespace Merlin
{
    TangentFrame TriangleFrame(const glm::vec3& p1,
                               const glm::vec3& p2,
                               const glm::vec3& p3,
                               const glm::vec2& uv1,
                               const glm::vec2& uv2,
                               const glm::vec2& uv3)
    {
        TangentFrame frame;

        auto E1 = p2 - p1;
        auto E2 = p3 - p1;

        auto DUV1 = uv2 - uv1;
        auto DUV2 = uv3 - uv1;

        float detUV = 1.0f / (DUV1.x * DUV2.y - DUV2.x * DUV1.y);

        glm::vec3 normal = glm::cross(E1, E2);

        glm::vec3 tangent{detUV * (DUV2.y * E1.x - DUV1.y * E2.x),
                          detUV * (DUV2.y * E1.y - DUV1.y * E2.y),
                          detUV * (DUV2.y * E1.z - DUV1.y * E2.z)};
        glm::vec3 bitangent{detUV * (-DUV2.x * E1.x + DUV1.x * E2.x),
                            detUV * (-DUV2.x * E1.y + DUV1.x * E2.y),
                            detUV * (-DUV2.x * E1.z + DUV1.x * E2.z)};

        frame.normal = glm::normalize(normal);
        frame.tangent = glm::normalize(tangent);
        frame.bitangent = glm::normalize(bitangent);

        return frame;
    }

    void CalculateTangentFrame(const std::shared_ptr<Mesh<Vertex_XNTBUV>>& mesh)
    {
        // Clear old tangent frame data
        for (int v_index = 0; v_index < mesh->GetVertexCount(); v_index++)
        {
            auto& v = mesh->GetVertex(v_index);
            v.normal *= 0.0f;
            v.tangent *= 0.0f;
            v.bitangent *= 0.0f;
        }

        // Accumulate triangle frames onto vertices with angle as weight
        for (int tri_index = 0; tri_index < mesh->GetTriangleCount();
             tri_index++)
        {
            auto idx1 = mesh->GetIndex(tri_index, 0);
            auto idx2 = mesh->GetIndex(tri_index, 1);
            auto idx3 = mesh->GetIndex(tri_index, 2);

            auto& p1 = mesh->GetVertex(idx1);
            auto& p2 = mesh->GetVertex(idx2);
            auto& p3 = mesh->GetVertex(idx3);

            auto& e1 = glm::normalize(p2.position - p1.position);
            auto& e2 = glm::normalize(p3.position - p1.position);
            auto& e3 = glm::normalize(p3.position - p2.position);

            float th1 = glm::acos(glm::dot(+e2, +e1));
            float th2 = glm::acos(glm::dot(+e3, -e1));
            float th3 = glm::acos(glm::dot(-e3, -e2));

            auto frame = TriangleFrame(
                p1.position, p2.position, p3.position, p1.uv, p2.uv, p3.uv);

            p1.normal += th1 * frame.normal;
            p2.normal += th2 * frame.normal;
            p3.normal += th3 * frame.normal;

            p1.tangent += th1 * frame.tangent;
            p2.tangent += th2 * frame.tangent;
            p3.tangent += th3 * frame.tangent;

            p1.bitangent += th1 * frame.bitangent;
            p2.bitangent += th2 * frame.bitangent;
            p3.bitangent += th3 * frame.bitangent;
        }

        // Normalize each tangent frame
        for (int v_index = 0; v_index < mesh->GetVertexCount(); v_index++)
        {
            auto& v = mesh->GetVertex(v_index);
            v.normal = glm::normalize(v.normal);
            v.tangent = glm::normalize(v.tangent);
            v.bitangent = glm::normalize(v.bitangent);
        }
    }

}  // namespace Merlin