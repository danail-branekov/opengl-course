#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT=600;

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
    std::cerr << "Error:" << description << std::endl;
}

int main() {
    glfwSetErrorCallback(error_callback);

    if(!glfwInit()) {
        std::cerr << "GLFW initialisation failed" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test window", NULL, NULL);
    if(!mainWindow) {
        glfwTerminate();
        std::cerr << "Main window creation failed" << std::endl;
        return 1;
    }

    int bufferWidth, bufferHeight;

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(mainWindow);
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialisation failed" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    while(!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);

    }








    return 0;
}
