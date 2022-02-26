#pragma once

#include <gl/glew.h>

#include <string>

class Texture
{
private:
    GLuint m_RendererID;
    std::string m_FilePath;
    GLubyte* m_LocalBuffer;
    GLint m_Width, m_Height, m_BPP;
public:
    Texture(const std::string& filepath);
    ~Texture();

    void Bind(GLuint slot = 0) const;
    void Unbind() const;

    inline GLint GetWidth() const { return m_Width; }
    inline GLint GetHeight() const { return m_Height; }
};
