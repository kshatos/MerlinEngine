#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP
#include <memory>
#include <vector>
#include <string>


namespace Merlin
{
    class Cubemap
    {
    public:
        static std::shared_ptr<Cubemap> Create(const std::vector<std::string>& face_paths);

        virtual ~Cubemap() {};

        virtual void Bind() = 0;
        virtual void UnBind() = 0;
    };
}
#endif