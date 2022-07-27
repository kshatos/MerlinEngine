#ifndef FILE_UTIL_HPP
#define FILE_UTIL_HPP
#include "Merlin/Render/mesh.hpp"
#include <string>
#include <memory>


namespace Merlin
{
    class Texture2DData;
    Texture2DData load_texture(std::string filepath);

    struct Vertex_XNTBUV;
    std::shared_ptr<Mesh<Vertex_XNTBUV>> load_mesh(std::string filepath);
}
#endif