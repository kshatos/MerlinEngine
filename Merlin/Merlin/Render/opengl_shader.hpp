#ifndef OPENGL_SHADER_HPP
#define OPENGL_SHADER_HPP
#include <stdint.h>
#include "Merlin/Render/shader.hpp"


namespace Merlin
{
    class OpenGLShader : public Shader
    {
        uint32_t id;

        bool CompilationSucceeded(int shader_id);

    public:
        OpenGLShader(
            const std::string& vertex_source,
            const std::string& fragment_source);
        ~OpenGLShader();
        void Bind() override;
        void UnBind() override;

        void SetUniformMat4(const std::string& name, glm::mat4 value) override;
        void SetUniformFloat4(const std::string& name, glm::vec4 value) override;
        void SetUniformFloat3(const std::string& name, glm::vec3 value) override;
        void SetUniformFloat2(const std::string& name, glm::vec2 value) override;
        void SetUniformFloat(const std::string& name, float value) override;
        void SetUniformInt(const std::string& name, int value) override;
    };
}

#endif