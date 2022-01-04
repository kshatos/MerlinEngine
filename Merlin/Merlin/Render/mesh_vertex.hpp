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
}
#endif