#include "VertexBufferLayout.h"

template<>
void VertexBufferLayout::Push<GLfloat>(GLuint count)
{
    m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<>
void VertexBufferLayout::Push<GLuint>(GLuint count)
{
    m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::Push<GLubyte>(GLuint count)
{
    m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}
