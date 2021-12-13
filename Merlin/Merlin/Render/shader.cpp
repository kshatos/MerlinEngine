#include "Merlin/Render/shader.hpp"
#include "Merlin/Platform/OpenGL/opengl_shader.hpp"


namespace Merlin
{
    std::shared_ptr<Shader> Shader::CreateFromSource(
        const std::string& vertex_source,
        const std::string& fragment_source)
    {
        return std::shared_ptr<Shader>(new OpenGLShader(vertex_source, fragment_source));
    }

    std::shared_ptr<Shader> Shader::CreateFromFiles(
        const std::string& vertex_path,
        const std::string& fragment_path)
    {
        return std::shared_ptr<Shader>(OpenGLShader::CreateFromFiles(vertex_path, fragment_path));
    }

}
