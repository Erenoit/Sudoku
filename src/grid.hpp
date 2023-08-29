#ifndef GRID_HPP
#define GRID_HPP

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

class Grid {
public:
    Grid(const Shader shader, int width, int height);
    ~Grid();

    void draw(glm::vec4 color) const;
    void updateSize(int width, int height);

private:
    int width, height;
    const Shader shader;
    const float thin_thickness  = 0.005f;
    const float thick_thickness = 0.012f;
    unsigned int VAO, VBO, EBO;
    float vertices[3 * 4 * 8 * 2];
    unsigned int indices[3* 2 * 8 * 2];
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);

    // TODO: Generate them at compile time
    void generateVertices();
    void generateIndices();
};

#endif // GRID_HPP
