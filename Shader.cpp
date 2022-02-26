#include "Shader.h"

#include <gl/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Debug.h"

Shader::Shader(const std::string& filepath)
    : m_FilePath{ filepath }, m_RendererID{ 0 }
{
    ShaderSource source{ ParseShader(filepath) };
    m_RendererID = CreateShader(source.Vertex, source.Fragment);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, GLint value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, GLfloat value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& value)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]));
}

ShaderSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream file{ filepath };

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream shaderstreams[2];
    ShaderType type = ShaderType::NONE;
    while (std::getline(file, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            if (type != ShaderType::NONE)
            {
                shaderstreams[(int)type] << line << "\n";
            }
        }
    }

    return
    {
        shaderstreams[(int)ShaderType::VERTEX].str(),
        shaderstreams[(int)ShaderType::FRAGMENT].str()
    };
}

GLuint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(GLuint program = glCreateProgram());
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

GLuint Shader::CompileShader(GLuint type, const std::string& source)
{
    GLCall(GLuint id = glCreateShader(type));
    const char* src = source.c_str();

    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    GLint result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE)
    {
        GLint length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)_malloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "[OpenGL Error] Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader!" << std::endl;
        std::cout << message << std::endl;
        _freea(message);

        GLCall(glDeleteShader(id));

        return 0;
    }

    return id;
}

GLint Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocations.find(name) != m_UniformLocations.end())
        return m_UniformLocations[name];

    GLCall(GLint location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "[OpenGL Warning] Uniform '" << name << "' doesn't exits!" << std::endl;
    
    m_UniformLocations[name] = location;
    return location;
}
