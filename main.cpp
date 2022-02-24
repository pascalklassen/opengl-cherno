#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <malloc.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

// todo: remove
#define ASSERT(x) if (!(x)) __debugbreak(); 
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

struct ShaderSource
{
    std::string Vertex;
    std::string Fragment;
};

static ShaderSource ParseShader(const std::string& filepath)
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

static GLuint CompileShader(GLuint type, const std::string& source)
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
        char* message = (char*) _malloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "Failed to compile " 
            << (type == GL_VERTEX_SHADER ? "vertext" : "fragment")
            << " shader!" << std::endl;
        std::cout << message << std::endl;
        _freea(message);

        GLCall(glDeleteShader(id));

        return 0;
    }

    return id;
}

static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

int main()
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello World!", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        GLfloat vertices[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
            -0.5f,  0.5f,
        };

        GLuint indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        VertexBuffer vbo{ vertices, 4 * 2 * sizeof(GLfloat) };

        VertexBufferLayout layout;
        layout.Push<GLfloat>(2);

        VertexArray vao;
        vao.AddBuffer(vbo, layout);

        IndexBuffer ibo{ indices, 6 };

        ShaderSource source = ParseShader("Basic.shader");
        std::cout << "Vertex Shader:" << std::endl;
        std::cout << source.Vertex << std::endl;
        std::cout << "Fragment Shader:" << std::endl;
        std::cout << source.Fragment << std::endl;

        GLuint shader = CreateShader(source.Vertex, source.Fragment);
        GLCall(glUseProgram(shader));

        GLint location = glGetUniformLocation(shader, "u_Color");
        ASSERT(location != -1);
        GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

        // clear OpenGL states
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        GLfloat r = 0.0f;
        GLfloat inc = 0.05f;
        while (!glfwWindowShouldClose(window))
        {
            // Render here
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

            vao.Bind();
            ibo.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)
                inc = -0.05f;
            else if (r < 0.0f)
                inc = 0.05f;

            r += inc;

            glfwSwapBuffers(window);

            glfwPollEvents();
        }

        GLCall(glDeleteProgram(shader));
    }

    glfwTerminate();

    return 0;
}