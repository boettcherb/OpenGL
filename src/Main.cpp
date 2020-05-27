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

const std::string COLORED_CUBE_VS = "res/shaders/coloredCube_vertex.glsl";
const std::string COLORED_CUBE_FS = "res/shaders/coloredCube_fragment.glsl";
const std::string LIGHT_SOURCE_VS = "res/shaders/lightSource_vertex.glsl";
const std::string LIGHT_SOURCE_FS = "res/shaders/lightSource_fragment.glsl";

// create camera object with initial position
static Camera g_camera(glm::vec3(0.0f, 0.0f, 3.0f));

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

    // draw over objects further away, but not over closer objects
    glEnable(GL_DEPTH_TEST);

    // don't render the back faces of triangles
    // the back face has vertices with a clockwise winding order
    glEnable(GL_CULL_FACE);

    // set clear color (background color)
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    const float CUBE_DATA[] = {
         // front
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         // left
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
         // right
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         // back
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
         // top
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         // bottom
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
    };

    const unsigned int CUBE_INDICES[] = {
         0,  3,  2,  0,  1,  3,
         4,  7,  6,  4,  5,  7,
         8, 11, 10,  8,  9, 11,
        12, 15, 14, 12, 13, 15,
        16, 19, 18, 16, 17, 19,
        20, 23, 22, 20, 21, 23,
    };
    const int NUM_INDICES = sizeof(CUBE_INDICES) / sizeof(unsigned int);

    // light source
    ShaderProgram lightSourceShader(LIGHT_SOURCE_VS, LIGHT_SOURCE_FS);
    Mesh lightSourceMesh(CUBE_DATA, sizeof(CUBE_DATA), { 3 });
    lightSourceMesh.addSubmesh(CUBE_INDICES, NUM_INDICES, &lightSourceShader);

    // colored cube
    ShaderProgram coloredCubeShader(COLORED_CUBE_VS, COLORED_CUBE_FS);
    coloredCubeShader.addUniform3f("u_objectColor", 1.0f, 0.5f, 0.31f);
    coloredCubeShader.addUniform3f("u_lightColor", 1.0f, 1.0f, 1.0f);
    Mesh coloredCubeMesh(CUBE_DATA, sizeof(CUBE_DATA), { 3 });
    coloredCubeMesh.addSubmesh(CUBE_INDICES, NUM_INDICES, &coloredCubeShader);

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

        glm::mat4 model = glm::mat4(1.0f);
        coloredCubeShader.addUniformMat4f("u_model", model);
        model = glm::translate(model, glm::vec3(3.0f, 2.0f, -3.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightSourceShader.addUniformMat4f("u_model", model);
        
        glm::mat4 view = g_camera.getViewMatrix();
        coloredCubeShader.addUniformMat4f("u_view", view);
        lightSourceShader.addUniformMat4f("u_view", view);

        float scrRatio = static_cast<float>(scrWidth) / static_cast<float>(scrHeight);
        glm::mat4 projection = glm::perspective(glm::radians(g_camera.getZoom()), scrRatio, 0.1f, 100.0f);
        coloredCubeShader.addUniformMat4f("u_projection", projection);
        lightSourceShader.addUniformMat4f("u_projection", projection);

        coloredCubeMesh.render();
        lightSourceMesh.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // clean up
    glfwTerminate();

    return 0;
}