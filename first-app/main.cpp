#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstring>

const GLint WIDTH = 800, HEIGHT = 600;

void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
    std::cerr << "Error:" << description << std::endl;
}

GLuint VAO, VBO, shader, uniformXMove;

bool moveLeft = true;
float triOffset = 0;
float triMaxOffset = 0.6;
float triIncrement = 0.005;

static const char *vertexShader = "\n#version 330\n\nuniform float xMove;\n\nlayout (location = 0) in vec3 pos;\n\nvoid main() {\n    gl_Position = vec4(0.7 * pos.x + xMove, 0.7 * pos.y, 0.7 * pos.z, 1.0);\n}";
static const char *fragmentShader = "#version 330\n\nout vec4 colour;\n\nvoid main() {\n    colour = vec4(1.0, 0.0, 0.0, 1.0);\n}   ";

void CreateTriangle() {
    GLfloat vertices[] = {
            -1.0, -1.0, -0.0,
            1.0, -1.0, 0.0,
            0.0, 1.0, 0.0
    };

    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glCreateBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int AddShader(GLuint theProgram, const char *shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = {0};
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling shader %d: %s\n", shaderType, eLog);
        return 1;
    }

    glAttachShader(theProgram, theShader);
}

int CompileShaders() {
    shader = glCreateProgram();
    if (!shader) {
        printf("Failed to create shader!\n");
        return 1;
    }

    AddShader(shader, vertexShader, GL_VERTEX_SHADER);
    AddShader(shader, fragmentShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: %s\n", eLog);
        return 1;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if(!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: %s\n", eLog);
        return 1;
    }

    uniformXMove = glGetUniformLocation(shader, "xMove");

    return 0;
}

int main() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "GLFW initialisation failed" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test window", NULL, NULL);
    if (!mainWindow) {
        glfwTerminate();
        std::cerr << "Main window creation failed" << std::endl;
        return 1;
    }

    int bufferWidth, bufferHeight;

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(mainWindow);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialisation failed" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();

    int compileShaderErr = CompileShaders();
    if(compileShaderErr) {
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    while (!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();

        triOffset +=triIncrement*float(moveLeft ? -1 : 1);
        if(std::abs(triOffset) >= (triMaxOffset)) {
            moveLeft = !moveLeft;
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glUniform1f(uniformXMove, triOffset);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);

    }

    return 0;
}
