#ifndef SHADER_HPP
#define SHADER_HPP
#include <string>


namespace Merlin
{
    class Shader
    {
    public:
        virtual ~Shader() {}
        virtual void Bind() = 0;
        virtual void UnBind() = 0;

        static Shader* Create(const std::string& vertex_source, const std::string& fragment_source);
    };
}

#endif