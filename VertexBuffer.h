#pragma once

#include <gl/glew.h>

class VertexBuffer
{
private:
    GLuint m_RendererID;

public:
    VertexBuffer(const void* data, GLuint size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
};
