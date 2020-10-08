#define GLEW_STATIC

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

unsigned int VBO, VAO;

std::string vShader =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position, 1.0);\n"
"ourColor = color;\n"
"}\0";

std::string fShader =
"#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(ourColor, 1.0f);\n"
"}\n\0";;

int shaderProgram()
{
    unsigned int vShaderContenedor = glCreateShader(GL_VERTEX_SHADER);
    const char* vsrc = vShader.c_str();
    glShaderSource(vShaderContenedor, 1, &vsrc, NULL);
    glCompileShader(vShaderContenedor);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vShaderContenedor, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vShaderContenedor, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fShaderContenedor = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fsrc = fShader.c_str();
    glShaderSource(fShaderContenedor, 1, &fsrc, NULL);
    glCompileShader(fShaderContenedor);

    glGetShaderiv(fShaderContenedor, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fShaderContenedor, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    int program = glCreateProgram();

    glAttachShader(program, vShaderContenedor);
    glAttachShader(program, fShaderContenedor);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    return program;
}

void VertexBufferObject()
{
    GLfloat vertices[] = {
        // Positions         // Colors
         0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  0.5f, 1.0f, 0.0f,  // Bottom Left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Top 
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind VAO
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "esto no es un word pelotudo", NULL, NULL);
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
        
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        glUseProgram(pene);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    
    glfwTerminate();
    return 0;
}