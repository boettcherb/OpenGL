#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"

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

const std::string FACE_TEXTURE = "res/textures/face.png";
const std::string SHELF_TEXTURE = "res/textures/container.jpg";

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
        std::cerr << "Failed to create GLFW window\n";
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
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << '\n';

    const float VBData[] = {
        //    position               color        texture coords
        -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 
         0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 1.0f,
         0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f,
    };

    const unsigned int VBIndeces[] = {
        0, 1, 3,
        0, 2, 3,
    };

    ShaderProgram shader({ VERTEX_SHADER, FRAGMENT_SHADER1 });

    Mesh mesh(VBData, sizeof(VBData), { 3, 3, 2 });
    mesh.addSubmesh(VBIndeces, sizeof(VBIndeces) / sizeof(unsigned int), &shader);

    const int FACE_TEXTURE_SLOT = 0;
    const int SHELF_TEXTURE_SLOT = 1;
    Texture face(FACE_TEXTURE, FACE_TEXTURE_SLOT);
    Texture shelf(SHELF_TEXTURE, SHELF_TEXTURE_SLOT);
    shader.addTexture(&face, "u_texture_face");
    shader.addTexture(&shelf, "u_texture_shelf");

    // set clear color (background color)
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // draw only the outlines of the triangles
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        // clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // render graphics stuff here
        mesh.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up
    glfwTerminate();

    return 0;
}