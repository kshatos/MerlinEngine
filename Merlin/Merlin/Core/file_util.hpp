#ifndef FILE_UTIL_HPP
#define FILE_UTIL_HPP
#include <string>


namespace Merlin
{
    class Texture2DData;
    Texture2DData load_texture(std::string filepath);
}
#endif