#ifndef SHADER_HPP
#define SHADER_HPP
#include <string>
#include <glm/glm.hpp>
#include <memory>


namespace Merlin
{
    class Shader
    {
    public:
        static std::shared_ptr<Shader> CreateFromSource(
            const std::string& vertex_source,
            const std::string& fragment_source);
        static std::shared_ptr<Shader> CreateFromFiles(
            const std::string& vertex_path,
            const std::string& fragment_path);

        virtual ~Shader() {}
        virtual void Bind() = 0;
        virtual void UnBind() = 0;

        virtual void SetUniformMat4(const std::string& name, glm::mat4 value) = 0;
        virtual void SetUniformMat3(const std::string& name, glm::mat3 value) = 0;
        virtual void SetUniformFloat4(const std::string& name, glm::vec4 value) = 0;
        virtual void SetUniformFloat3(const std::string& name, glm::vec3 value) = 0;
        virtual void SetUniformFloat2(const std::string& name, glm::vec2 value) = 0;
        virtual void SetUniformFloat(const std::string& name, float value) = 0;

        virtual void SetUniformInt(const std::string& name, int value) = 0;
    };
}

#endif