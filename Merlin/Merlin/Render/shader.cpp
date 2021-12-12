#include "Merlin/Render/shader.hpp"
#include "Merlin/Platform/OpenGL/opengl_shader.hpp"


namespace Merlin
{
    Shader* Shader::Create(const std::string& vertex_source, const std::string& fragment_source)
    {
        return new OpenGLShader(vertex_source, fragment_source);
    }
}
