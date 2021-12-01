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
    };
}

#endif