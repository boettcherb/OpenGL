#include "ShaderProgram.h"
#include "Mesh.h"

#include <glad/glad.h>
#include <GLFW/GLFW3.h>

#include <iostream>
#include <string>

unsigned int scrWidth = 960;
unsigned int scrHeight = 540;
const char* SCR_TITLE = "OpenGL Window";
const std::string VERTEX_SHADER = "res/shaders/vertex.glsl";
const std::string FRAGMENT_SHADER1 = "res/shaders/fragment.glsl";
const std::string FRAGMENT_SHADER2 = "res/shaders/fragment2.glsl";

// Whenever the window size changes, this callback function executes
void framebuffer_size_callback(GLFWwindow* /* window */, int width, int height) {
    scrWidth = width;
    scrHeight = height;

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
    GLFWwindow* window = glfwCreateWindow(scrWidth, scrHeight, SCR_TITLE, nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Tie the buffer swap rate (the FPS) to your monitor's refresh rate
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

    const float VB[] = {
        //  3D Position            Color
        // X     Y      Z      R     G     B
         0.8f,  0.8f,  0.0f,  1.0f, 0.0f, 0.0f, // 0
         0.8f,  0.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 1
         0.8f, -0.8f,  0.0f,  0.0f, 0.0f, 0.0f, // 2
         0.0f,  0.8f,  0.0f,  0.5f, 0.5f, 0.5f, // 3
         0.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // 4
         0.0f, -0.8f,  0.0f,  0.0f, 1.0f, 0.0f, // 5
        -0.8f,  0.8f,  0.0f,  0.0f, 0.0f, 1.0f, // 6
        -0.8f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // 7
        -0.8f, -0.8f,  0.0f,  1.0f, 1.0f, 1.0f, // 8
    };

    const unsigned int IBData[] = {
        0, 1, 4,
        2, 5, 4,
        8, 7, 4,
        4, 6, 3,
        1, 2, 4,
        4, 3, 0,
        4, 5, 8,
        7, 6, 4,
    };

    ShaderProgram shader1({ VERTEX_SHADER, FRAGMENT_SHADER1 });
    Mesh rect(VB, sizeof(VB), { 3, 3 });
    rect.addSubmesh(IBData, sizeof(IBData) / sizeof(unsigned int), &shader1);

    // set clear color (background color)
    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

    // draw only the outlines of the triangles
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        // clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // render graphics stuff here
        rect.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up
    glfwTerminate();

    return 0;
}