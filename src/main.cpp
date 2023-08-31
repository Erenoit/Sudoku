#include <iostream>
#include <cstring>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "grid.hpp"
#include "resource_manager.hpp"

#define START_WIDTH  800
#define START_HEIGHT 600

static glm::vec4 background = glm::vec4(0x2D / 255.0f, 0x51 / 266.0f, 0x59 / 255.0f, 1.0f);
static glm::vec4 primary    = glm::vec4(0x48 / 255.0f, 0x7D / 255.0f, 0x67 / 255.0f, 1.0f);
static glm::vec4 secondary  = glm::vec4(0xC5 / 255.0f, 0xC0 / 255.0f, 0x97 / 255.0f, 1.0f);
static glm::vec4 yellow     = glm::vec4(0xF6 / 255.0f, 0xCE / 255.0f, 0x6B / 255.0f, 1.0f);
static glm::vec4 red        = glm::vec4(0xC7 / 255.0f, 0x2F / 255.0f, 0x28 / 255.0f, 1.0f);

Grid *g;

static void error_callback(int error, const char* description) {
    std::cerr << " GLFW Error(" << error << "): " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    else if (g != nullptr && action & (GLFW_PRESS | GLFW_REPEAT))
            switch (key) {
                case GLFW_KEY_UP:    g->goUp();    break;
                case GLFW_KEY_DOWN:  g->goDown();  break;
                case GLFW_KEY_LEFT:  g->goLeft();  break;
                case GLFW_KEY_RIGHT: g->goRight(); break;
                default:
                    std::cout << "Key: " << key << " Scancode: " << scancode << " Action: " << action << " Mods: " << mods << std::endl;
                    break;
            }

}

static void screen_size_change_callback(GLFWwindow *window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
    if (g != nullptr) {
        g->updateSize(width, height);
    }
}

int main() {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW: " << strerror(errno) << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing (MSAA
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(START_WIDTH, START_HEIGHT, "Sudoku", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create a window: " << strerror(errno) << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetWindowSizeLimits(window, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, screen_size_change_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSwapInterval(1); // Enable vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, START_WIDTH, START_HEIGHT);

    ResourceManager::loadShader("standard_shader", "assets/shaders/standard_vertex.glsl", "assets/shaders/standard_fragment.glsl");

    Grid grid = *new Grid(START_WIDTH, START_HEIGHT);
    g = &grid;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(background[0], background[1], background[2], background[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        grid.draw(primary);

        glfwSwapBuffers(window);
    }

    ResourceManager::clear();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
