#include "ShaderProgram.h"

#include <glad/glad.h>
#include <GLFW/GLFW3.h>

#include <iostream>
#include <string>

unsigned int SCR_WIDTH = 960;
unsigned int SCR_HEIGHT = 540;
const char* SCR_TITLE = "OpenGL Window";
const std::string VERTEX_SHADER = "res/shaders/vertex.glsl";
const std::string FRAGMENT_SHADER = "res/shaders/fragment.glsl";

// Whenever the window size changes, this callback function executes
void framebuffer_size_callback(GLFWwindow* /* window */, int width, int height) {
    SCR_WIDTH = width;
    SCR_HEIGHT = height;

    // tell OpenGL the new dimensions of the window
    glViewport(0, 0, width, height);
}

// Called every frame inside the render loop
void processInput(GLFWwindow* window) {
    // if the escape key is pressed, tell the window to close
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    // initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create the main window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, SCR_TITLE, nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Tie the buffer swap rate (the FPS) to your monitor's refresh rate
    // Ex: My monitor has a refresh rate of 60hz, so the max FPS of this 
    // game (for me) will be 60 FPS
    glfwSwapInterval(1);

    // call the framebuffer_size_callback function when the window is resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << '\n';

    ShaderProgram shader({ VERTEX_SHADER, FRAGMENT_SHADER });

    // set clear color (background color)
    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        // clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // render graphics stuff here
        shader.bind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up
    glfwTerminate();

    return 0;
}