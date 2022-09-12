#ifndef FILE_UTIL_HPP
#define FILE_UTIL_HPP
#include <filesystem>
#include <memory>
#include <string>

#include "Merlin/Render/mesh.hpp"

namespace Merlin
{
    class Texture2DData;
    Texture2DData LoadTexture(std::filesystem::path file_path);

    struct Vertex_XNTBUV;
    std::shared_ptr<Mesh<Vertex_XNTBUV>> LoadMesh(
        std::filesystem::path file_path);
}  // namespace Merlin
#endif