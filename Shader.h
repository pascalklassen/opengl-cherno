#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

struct ShaderSource
{
    std::string Vertex;
    std::string Fragment;
};

class Shader
{
private:
    std::string m_FilePath;
    GLuint m_RendererID;
    std::unordered_map<std::string, GLint> m_UniformLocations;
public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& name, GLint value);
    void SetUniform1f(const std::string& name, GLfloat value);
    void SetUniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    void SetUniformMat4f(const std::string& name, glm::mat4& value);
private:
    ShaderSource ParseShader(const std::string& filepath);
    GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    GLuint CompileShader(GLuint type, const std::string& source);
    GLint GetUniformLocation(const std::string& name);
};
