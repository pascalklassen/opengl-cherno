#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <malloc.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

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
            -0.5f, -0.5f, 0.0f, 0.0f,
             0.5f, -0.5f, 1.0f, 0.0f,
             0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f
        };

        GLuint indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexBuffer vbo{ vertices, 4 * 4 * sizeof(GLfloat) };

        VertexBufferLayout layout;
        layout.Push<GLfloat>(2);
        layout.Push<GLfloat>(2);

        VertexArray vao;
        vao.AddBuffer(vbo, layout);

        IndexBuffer ibo{ indices, 6 };

        Shader shader{ "Basic.shader" };
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

        Texture texture{ "opengl-logo.png" };
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);
        shader.SetUniformMat4f("u_MVP", proj);

        vao.Unbind();
        vbo.Unbind();
        ibo.Unbind();
        shader.Unbind();

        Renderer renderer;

        GLfloat r = 0.0f;
        GLfloat inc = 0.05f;
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            renderer.Draw(vao, ibo, shader);

            if (r > 1.0f)
                inc = -0.05f;
            else if (r < 0.0f)
                inc = 0.05f;

            r += inc;

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
    }

    glfwTerminate();

    return 0;
}
