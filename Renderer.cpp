#include "Renderer.h"

#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ; // do nothing
}

bool GLLogCall(const char* func, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[ERROR] [OpenGL] (" << error << "): "
            << func << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}
