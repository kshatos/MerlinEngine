#include <glad/glad.h>
#include "Merlin/Platform/OpenGL/opengl_shader.hpp"
#include "Merlin/Core/logger.hpp"
#include<glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>


namespace Merlin
{
    OpenGLShader* OpenGLShader::CreateFromFiles(
        const std::string& vertex_path,
        const std::string& fragment_path)
    {
        std::string vertex_source;
        std::string fragment_source;

        {
            std::ifstream file(vertex_path, std::ios::in);
            if (file)
            {
                std::stringstream string_stream;
                string_stream << file.rdbuf();
                vertex_source = string_stream.str();
            }
            else
            {
                ME_LOG_ERROR("Unable to load vertex shader at: " + vertex_path);
            }
        }
        {
            std::ifstream file(fragment_path, std::ios::in);
            if (file)
            {
                std::stringstream string_stream;
                string_stream << file.rdbuf();
                fragment_source = string_stream.str();
            }
            else
                ME_LOG_ERROR("Unable to load fragment shader at: " + fragment_path);
        }

        return new OpenGLShader(vertex_source, fragment_source);
    }

    OpenGLShader::OpenGLShader(
        const std::string& vertex_source,
        const std::string& fragment_source)
    {
        const char* vShaderCode = vertex_source.c_str();
        const char* fShaderCode = fragment_source.c_str();

        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        if (!CompilationSucceeded("Vertex", vertex))
        {
            glDeleteShader(vertex);
            return;
        }

        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        if (!CompilationSucceeded("Fragment", fragment))
        {
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            return;
        }

        m_id = glCreateProgram();
        glAttachShader(m_id, vertex);
        glAttachShader(m_id, fragment);
        glLinkProgram(m_id);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_id);
    }

    void OpenGLShader::Bind()
    {
        glUseProgram(m_id);
    }

    void OpenGLShader::UnBind()
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetUniformMat4(const std::string& name, glm::mat4 value)
    {
        auto location = glGetUniformLocation(m_id, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniformMat3(const std::string& name, glm::mat3 value)
    {
        auto location = glGetUniformLocation(m_id, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniformFloat4(const std::string& name, glm::vec4 value)
    {
        auto location = glGetUniformLocation(m_id, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetUniformFloat3(const std::string& name, glm::vec3 value)
    {
        auto location = glGetUniformLocation(m_id, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::SetUniformFloat2(const std::string& name, glm::vec2 value)
    {
        auto location = glGetUniformLocation(m_id, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::SetUniformFloat(const std::string& name, float value)
    {
        auto location = glGetUniformLocation(m_id, name.c_str());
        glUniform1f(location, value);
    }


    void OpenGLShader::SetUniformInt(const std::string& name, int value)
    {
        auto location = glGetUniformLocation(m_id, name.c_str());
        glUniform1i(location, value);
    }

    bool OpenGLShader::CompilationSucceeded(std::string name, int shader_id)
    {
        int compile_status;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);
        if (!compile_status)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader_id, 512, NULL, infoLog);

            std::ostringstream oss;
            oss << name << " shader compilation failed: " << std::endl << std::string(infoLog);
            ME_LOG_ERROR(oss.str());
            return false;
        }
        return true;
    }

}
