#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"

#include <glad/glad.h>
#include <GLFW/GLFW3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>

static unsigned int scrWidth = 800;
static unsigned int scrHeight = 600;
const char* SCR_TITLE = "OpenGL Window";

const std::string VERTEX_SHADER = "res/shaders/vertex.glsl";
const std::string FRAGMENT_SHADER1 = "res/shaders/fragment.glsl";
const std::string FRAGMENT_SHADER2 = "res/shaders/fragment2.glsl";

const std::string FACE_TEXTURE = "res/textures/face.png";
const std::string SHELF_TEXTURE = "res/textures/container.jpg";
const std::string GRADIENT_TEXTURE = "res/textures/gradient.png";
const std::string WALL_TEXTURE = "res/textures/wall.jpg";

static Camera g_camera;

// This callback function executes whenever the window size changes
static void framebuffer_size_callback(GLFWwindow* /* window */, int width, int height) {
    scrWidth = width;
    scrHeight = height;

    // tell OpenGL the new dimensions of the window
    glViewport(0, 0, width, height);
}

// This callback function executes whenever the user moves the mouse
void mouse_callback(GLFWwindow* /* window */, double xpos, double ypos) {
    g_camera.processMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
}

// This callback function executes whenever the user moves the mouse scroll wheel
void scroll_callback(GLFWwindow* /* window */, double /* offsetX */, double offsetY) {
    g_camera.processMouseScroll(static_cast<float>(offsetY));
}

// Called every frame inside the render loop
static void processInput(GLFWwindow* window, float deltaTime) {
    // if the escape key is pressed, tell the window to close
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // WASD for the camera
    const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        g_camera.processKeyboard(Camera::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        g_camera.processKeyboard(Camera::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        g_camera.processKeyboard(Camera::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        g_camera.processKeyboard(Camera::RIGHT, deltaTime);
    }
}

// print the FPS to the screen every second
static void displayFPS() {
    static int FPS = 0;
    static double previousTime = glfwGetTime();
    double currentTime = glfwGetTime();
    ++FPS;
    if (currentTime - previousTime >= 1.0) {
        std::cout << "FPS: " << FPS << '\n';
        FPS = 0;
        previousTime = currentTime;
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Tie the buffer swap rate (the FPS) to your monitor's refresh rate
    glfwSwapInterval(1);

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

    // variables for deltaTime
    double previousTime = glfwGetTime();
    double deltaTime = 0.0f;
    
    // render loop
    while (!glfwWindowShouldClose(window)) {
        // calculate deltaTime and process input
        double currentTime = glfwGetTime();
        deltaTime = currentTime - previousTime;
        previousTime = currentTime;
        processInput(window, static_cast<float>(deltaTime));

        displayFPS();

        // clear the screen and the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // the MVP matrices
        glm::mat4 model = glm::mat4(1.0f);
        shader.addUniformMat4f("u_model", model);
        
        glm::mat4 view = g_camera.getViewMatrix();
        shader.addUniformMat4f("u_view", view);

        float scrRatio = static_cast<float>(scrWidth) / static_cast<float>(scrHeight);
        glm::mat4 projection = glm::perspective(glm::radians(g_camera.getZoom()), scrRatio, 0.1f, 100.0f);
        shader.addUniformMat4f("u_projection", projection);

        mesh.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // clean up
    glfwTerminate();

    return 0;
}