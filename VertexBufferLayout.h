#pragma once

#include <gl/glew.h>

#include <vector>

#include "Debug.h"

struct VertexBufferElement
{
    GLuint type;
    GLuint count;
    GLubyte normalized;

    static GLuint GetSizeOfType(GLuint type)
    {
        switch (type)
        {
        case GL_FLOAT:          return 4;
        case GL_UNSIGNED_INT:   return 4;
        case GL_UNSIGNED_BYTE:  return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    GLuint m_Stride;

public:
    VertexBufferLayout()
        : m_Stride{ 0 } 
    {};

    template <typename T>
    void Push(GLuint count);

    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
    inline GLuint GetStride() const { return m_Stride; }
};
