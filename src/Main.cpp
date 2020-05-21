#include <glad/glad.h>
#include <GLFW/GLFW3.h>

#include <iostream>

const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;
const char* SCR_TITLE = "OpenGL Window";

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

    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << '\n';

    // set clear color (background color)
    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // clear the screen
        glClear(GL_COLOR_BUFFER_BIT);


        // render graphics stuff here


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up
    glfwTerminate();

    return 0;
}
