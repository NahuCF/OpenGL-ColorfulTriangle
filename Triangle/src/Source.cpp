#define GLEW_STATIC

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

unsigned int VBO, EBO;

std::string vShader =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()"
"{"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);"
"}";

std::string fShader =
"#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n";

int shaderProgram()
{
    unsigned int vShaderContenedor = glCreateShader(GL_VERTEX_SHADER);
    const char* vsrc = vShader.c_str();
    glShaderSource(vShaderContenedor, 1, &vsrc, NULL);
    glCompileShader(vShaderContenedor);

    unsigned int fShaderContenedor = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fsrc = fShader.c_str();
    glShaderSource(fShaderContenedor, 1, &fsrc, NULL);
    glCompileShader(fShaderContenedor);

    int program = glCreateProgram();

    glAttachShader(program, vShaderContenedor);
    glAttachShader(program, fShaderContenedor);

    glLinkProgram(program);

    return program;
}

void VertexBufferObject()
{
    float vertices[]{
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    
    unsigned int indices[]{
        0, 1, 2,
        0, 2, 3
    };
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "No es un Hello World pelotudo", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }

    VertexBufferObject();

    int pene = shaderProgram();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUseProgram(pene);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}