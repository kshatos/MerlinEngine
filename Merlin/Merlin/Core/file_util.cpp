#include "Merlin/Core/file_util.hpp"
#include "Merlin/Render/texture2d_data.hpp"
#include "Merlin/Render/mesh_vertex.hpp"
#include "Merlin/Core/logger.hpp"
#include "stb/stb_image.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <stdexcept>
#include <algorithm>


namespace Merlin
{
    Texture2DData load_texture(std::string filepath)
    {
        int width, height, channel_count;
        // TODO: Add options for fewer channels and flip
        //stbi_set_flip_vertically_on_load(1);
        unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channel_count, STBI_rgb_alpha);
        if (data==nullptr)
        {
            ME_LOG_ERROR("Failed to load texture at: " + filepath);
            throw std::runtime_error("Failed to load texture at: " + filepath);
        }
        Texture2DData texture_data(width, height, 4, data);
        stbi_image_free(data);

        return texture_data;
    }

    std::shared_ptr<Mesh<Vertex_XNTBUV>> load_mesh(std::string filepath)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(
            filepath,
            aiProcess_Triangulate | aiProcess_FlipUVs);

        if (
            !scene ||
            scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
            !scene->mRootNode)
        {
            ME_LOG_ERROR("Unable to load mesh at: " + filepath);
            return nullptr;
        }

        int totalVertexCount = 0;
        int totalTriangleCount = 0;
        for (int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
        {
            auto mesh = scene->mMeshes[meshIndex];
            if (!mesh->HasPositions() ||
                !mesh->HasNormals() ||
                !mesh->HasTextureCoords(0))
                continue;

            totalVertexCount += mesh->mNumVertices;
            totalTriangleCount += mesh->mNumFaces;
        }

        auto merlinMesh = std::make_shared<Mesh<Vertex_XNTBUV>>();
        merlinMesh->SetVertexCount(totalVertexCount);
        merlinMesh->SetTriangleCount(totalTriangleCount);

        int vertexOffset = 0;
        int triangleOffset = 0;
        for (int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
        {
            auto mesh = scene->mMeshes[meshIndex];
            if (!mesh->HasPositions() ||
                !mesh->HasNormals() ||
                !mesh->HasTextureCoords(0))
                continue;

            for (int vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++)
            {
                auto& vertex = merlinMesh->GetVertex(vertexIndex + vertexOffset);

                vertex.position.x = mesh->mVertices[vertexIndex].x;
                vertex.position.y = mesh->mVertices[vertexIndex].y;
                vertex.position.z = mesh->mVertices[vertexIndex].z;

                vertex.normal.x = mesh->mNormals[vertexIndex].x;
                vertex.normal.y = mesh->mNormals[vertexIndex].y;
                vertex.normal.z = mesh->mNormals[vertexIndex].z;

                vertex.uv.x = mesh->mTextureCoords[0][vertexIndex].x;
                vertex.uv.y = mesh->mTextureCoords[0][vertexIndex].y;
            }

            for (int triangleIndex = 0; triangleIndex < mesh->mNumFaces; triangleIndex++)
            {
                merlinMesh->GetIndex(triangleOffset + triangleIndex, 0) = mesh->mFaces[triangleIndex].mIndices[0] + vertexOffset;
                merlinMesh->GetIndex(triangleOffset + triangleIndex, 1) = mesh->mFaces[triangleIndex].mIndices[1] + vertexOffset;
                merlinMesh->GetIndex(triangleOffset + triangleIndex, 2) = mesh->mFaces[triangleIndex].mIndices[2] + vertexOffset;
            }

            vertexOffset += mesh->mNumVertices;
            triangleOffset += mesh->mNumFaces;
        }

        return merlinMesh;
    }
}