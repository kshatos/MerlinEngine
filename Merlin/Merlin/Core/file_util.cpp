#include "Merlin/Core/file_util.hpp"

#include <algorithm>
#include <stdexcept>

#include "Merlin/Core/logger.hpp"
#include "Merlin/Render/mesh_vertex.hpp"
#include "Merlin/Render/texture2d_data.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "stb/stb_image.h"

namespace Merlin
{
    Texture2DData LoadTexture(std::string file_path)
    {
        int width, height, channel_count;
        // TODO: Add options for fewer channels and flip
        // stbi_set_flip_vertically_on_load(1);
        unsigned char* data = stbi_load(
            file_path.c_str(), &width, &height, &channel_count, STBI_rgb_alpha);
        if (data == nullptr)
        {
            ME_LOG_ERROR("Failed to load texture at: " + file_path);
            throw std::runtime_error("Failed to load texture at: " + file_path);
        }
        Texture2DData texture_data(width, height, 4, data);
        stbi_image_free(data);

        return texture_data;
    }

    std::shared_ptr<Mesh<Vertex_XNTBUV>> LoadMesh(std::string file_path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(
            file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
            !scene->mRootNode)
        {
            ME_LOG_ERROR("Unable to load mesh at: " + file_path);
            return nullptr;
        }

        int total_vertex_count = 0;
        int total_triangle_count = 0;
        for (int mesh_index = 0; mesh_index < scene->mNumMeshes; mesh_index++)
        {
            auto mesh = scene->mMeshes[mesh_index];
            if (!mesh->HasPositions() || !mesh->HasNormals() ||
                !mesh->HasTextureCoords(0))
                continue;

            total_vertex_count += mesh->mNumVertices;
            total_triangle_count += mesh->mNumFaces;
        }

        auto merlin_mesh = std::make_shared<Mesh<Vertex_XNTBUV>>();
        merlin_mesh->SetVertexCount(total_vertex_count);
        merlin_mesh->SetTriangleCount(total_triangle_count);

        int vertex_offset = 0;
        int triangle_offset = 0;
        for (int mesh_index = 0; mesh_index < scene->mNumMeshes; mesh_index++)
        {
            auto mesh = scene->mMeshes[mesh_index];
            if (!mesh->HasPositions() || !mesh->HasNormals() ||
                !mesh->HasTextureCoords(0))
                continue;

            for (int vertex_index = 0; vertex_index < mesh->mNumVertices;
                 vertex_index++)
            {
                auto& vertex =
                    merlin_mesh->GetVertex(vertex_index + vertex_offset);

                vertex.position.x = mesh->mVertices[vertex_index].x;
                vertex.position.y = mesh->mVertices[vertex_index].y;
                vertex.position.z = mesh->mVertices[vertex_index].z;

                vertex.normal.x = mesh->mNormals[vertex_index].x;
                vertex.normal.y = mesh->mNormals[vertex_index].y;
                vertex.normal.z = mesh->mNormals[vertex_index].z;

                vertex.uv.x = mesh->mTextureCoords[0][vertex_index].x;
                vertex.uv.y = mesh->mTextureCoords[0][vertex_index].y;
            }

            for (int triangle_index = 0; triangle_index < mesh->mNumFaces;
                 triangle_index++)
            {
                merlin_mesh->GetIndex(triangle_offset + triangle_index, 0) =
                    mesh->mFaces[triangle_index].mIndices[0] + vertex_offset;
                merlin_mesh->GetIndex(triangle_offset + triangle_index, 1) =
                    mesh->mFaces[triangle_index].mIndices[1] + vertex_offset;
                merlin_mesh->GetIndex(triangle_offset + triangle_index, 2) =
                    mesh->mFaces[triangle_index].mIndices[2] + vertex_offset;
            }

            vertex_offset += mesh->mNumVertices;
            triangle_offset += mesh->mNumFaces;
        }

        return merlin_mesh;
    }
}  // namespace Merlin