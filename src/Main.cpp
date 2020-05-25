#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"

#include <glad/glad.h>
#include <GLFW/GLFW3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>

unsigned int scrWidth = 800;
unsigned int scrHeight = 600;
const char* SCR_TITLE = "OpenGL Window";

const std::string VERTEX_SHADER = "res/shaders/vertex.glsl";
const std::string FRAGMENT_SHADER1 = "res/shaders/fragment.glsl";
const std::string FRAGMENT_SHADER2 = "res/shaders/fragment2.glsl";

const std::string FACE_TEXTURE = "res/textures/face.png";
const std::string SHELF_TEXTURE = "res/textures/container.jpg";
const std::string GRADIENT_TEXTURE = "res/textures/gradient.png";
const std::string WALL_TEXTURE = "res/textures/wall.jpg";

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
        std::cerr << "Failed to initialize GLFW\n";
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
        // front
       -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,    1.0f, 1.0f,

        // left
       -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,    1.0f, 1.0f,

       // right
       0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,    1.0f, 1.0f,

       // back
       0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,

      // top
     -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,
      0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,    1.0f, 1.0f,

      // bottom
     -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
     -0.5f, -0.5f,  0.5f,    0.0f, 1.0f,
      0.5f, -0.5f,  0.5f,    1.0f, 1.0f,
    };

    const unsigned int VBIndeces[] = {
         0,  2,  3,  0,  3,  1,
         4,  6,  7,  4,  7,  5,
         8, 10, 11,  8, 11,  9,
        12, 14, 15, 12, 15, 13,
        16, 18, 19, 16, 19, 17,
        20, 22, 23, 20, 23, 21,
    };

    ShaderProgram shader({ VERTEX_SHADER, FRAGMENT_SHADER1 });
    Mesh mesh(VBData, sizeof(VBData), { 3, 2 });
    mesh.addSubmesh(VBIndeces, sizeof(VBIndeces) / sizeof(unsigned int), &shader);

    Texture shelf(SHELF_TEXTURE, 0u);
    Texture face(FACE_TEXTURE, 1u);
    shader.addTexture(&shelf, "u_texture_shelf");
    shader.addTexture(&face, "u_texture_face");

    // draw over objects further away, but not over closer objects
    glEnable(GL_DEPTH_TEST);

    // set clear color (background color)
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // variables for the FPS counter
    double previousTime = glfwGetTime();
    double totalFrames = 0, seconds = 0;
    int FPS = 0;

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  -5.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        // clear the screen and the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < 10; ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(9.0f, 6.0f, 0.0f));
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::translate(view, cubePositions[i]);
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
            shader.addUniformMat4f("u_model", model);
            shader.addUniformMat4f("u_view", view);
            shader.addUniformMat4f("u_projection", projection);
            mesh.render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // FPS counter
        double currentTime = glfwGetTime();
        FPS++;
        if (currentTime - previousTime >= 1.0) {
            ++seconds;
            totalFrames += FPS;
            std::cout << "FPS: " << FPS << ", average FPS: " << totalFrames / seconds << '\n';
            FPS = 0;
            previousTime = currentTime;
        }
    }
    
    // clean up
    glfwTerminate();

    return 0;
}