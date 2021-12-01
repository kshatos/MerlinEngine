#include <glad/glad.h>
#include "Merlin/Render/opengl_shader.hpp"
#include "Merlin/Core//logger.hpp"
#include <sstream>


namespace Merlin
{
    OpenGLShader::OpenGLShader(
        const std::string& vertex_source,
        const std::string& fragment_source)
    {
        const char* vShaderCode = vertex_source.c_str();
        const char* fShaderCode = fragment_source.c_str();

        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        if (!CompilationSucceeded(vertex))
        {
            glDeleteShader(vertex);
            return;
        }

        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        if (!CompilationSucceeded(fragment))
        {
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            return;
        }

        id = glCreateProgram();
        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        glLinkProgram(id);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(id);
    }

    void OpenGLShader::Bind()
    {
        glUseProgram(id);
    }

    void OpenGLShader::UnBind()
    {
        glUseProgram(0);
    }

    bool OpenGLShader::CompilationSucceeded(int shader_id)
    {
        int compile_status;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);
        if (!compile_status)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader_id, 512, NULL, infoLog);

            std::ostringstream oss;
            oss << "Vertex shader compilation failed: " << std::endl << std::string(infoLog);
            ME_LOG_ERROR(oss.str());
            return false;
        }
        return true;
    }

}
