#include <cstring>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "game.hpp"
#include "resource_manager.hpp"

#define START_WIDTH  800
#define START_HEIGHT 600

Game game(START_WIDTH, START_HEIGHT);

static void error_callback(int error, const char *description) {
    std::cerr << " GLFW Error(" << error << "): " << description << std::endl;
}

static void screen_size_change_callback(GLFWwindow *window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
    game.updateScreenSize(width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    (void)window;
    (void)scancode;
    (void)mode;
    game.processInput(key, action);
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

    GLFWwindow *window = glfwCreateWindow(START_WIDTH, START_HEIGHT, "Sudoku", nullptr, nullptr);
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

    game.init();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        game.update();

        glm::vec4 background = game.getBackground();
        glClearColor(background.r, background.g, background.b, background.a);
        glClear(GL_COLOR_BUFFER_BIT);
        game.draw();

        glfwSwapBuffers(window);
    }

    ResourceManager::clear();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
