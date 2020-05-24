#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"

#include <glad/glad.h>
#include <GLFW/GLFW3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <cmath>

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

typedef std::pair<float, float> Point;
Point avg(const Point& p1, const Point& p2);
void recur(const Mesh& mesh, ShaderProgram* shader, Point bl, Point br, Point top, int invScale);

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
    // glfwSwapInterval(1);

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
          170.0f, 294.4486f, 0.0f,  0.0f, 1.0f, 0.0f,  0.5f, 0.5f,
         -170.0f, 294.4486f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.5f,
            0.0f,   0.0f,    0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 0.0f,
    };

    const unsigned int VBIndeces[] = {
        0, 1, 2
    };

    ShaderProgram shader({ VERTEX_SHADER, FRAGMENT_SHADER1 });
    Mesh mesh(VBData, sizeof(VBData), { 3, 3, 2 });
    mesh.addSubmesh(VBIndeces, sizeof(VBIndeces) / sizeof(unsigned int), &shader);

    // set clear color (background color)
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    Texture gradient(GRADIENT_TEXTURE, 0);
    shader.addTexture(&gradient, "u_texture_gradient");

    // draw only the outlines of the triangles
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // variables for the FPS counter
    double previousTime = glfwGetTime();
    double totalFrames = 0, seconds = 0;
    int FPS = 0;
    
    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        // clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        float x = (1.0f + static_cast<float>(std::sin(glfwGetTime()))) / 4.0f;
        float y = (1.0f + static_cast<float>(std::cos(glfwGetTime()))) / 4.0f;
        shader.addUniform1f("u_offsetX", x);
        shader.addUniform1f("u_offsetY", y);
        recur(mesh, &shader, Point(60.0f, 0.0f), Point(740.0f, 0.0f), Point(400.0f, 588.897f), 1);

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

Point avg(const Point& p1, const Point& p2) {
    return Point((p1.first + p2.first) / 2.0f, (p1.second + p2.second) / 2.0f);
}

// Method to render equailateral triangles to form the Sierpinski triangle fractal
// bl, br, and top form an equilateral triangle
// bl = the bottom left point
// br = the bottom right point
// top = the top point
void recur(const Mesh& mesh, ShaderProgram* shader, Point bl, Point br, Point top, int invScale) {
    if (invScale <= 64) {
        // the 3 midpoints along the edges of the triangle made from bl, br, and top
        // this triangle is upside down and inside the triangle made from bl, br, and top
        Point tl = avg(top, bl);    // the top left point
        Point tr = avg(top, br);    // the top left point
        Point bottom = avg(bl, br); // the bottom point

        glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f -1.0f, 1.0f);
        // the vertex positions are set up so that the bottom point is (0, 0)
        proj = glm::translate(proj, glm::vec3(bottom.first, bottom.second, 0.0f));
        // scale by 0.5 for each new level of triangles
        float scale = 1.0f / static_cast<float>(invScale);
        proj = glm::scale(proj, glm::vec3(scale, scale, 1.0f));
        shader->addUniformMat4f("u_trans", proj);

        mesh.render();

        recur(mesh, shader, bl,     bottom, tl,  invScale * 2); // bottom left
        recur(mesh, shader, bottom, br,     tr,  invScale * 2); // bottom right
        recur(mesh, shader, tl,     tr,     top, invScale * 2); // top
    }
}