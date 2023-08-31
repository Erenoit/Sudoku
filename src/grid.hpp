#ifndef GRID_HPP
#define GRID_HPP

#include <array>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "selection_box.hpp"

class Grid {
public:
    Grid(int width, int height);
    ~Grid();

    void draw(glm::vec4 color) const;
    void updateSize(int width, int height);
    void goUp();
    void goDown();
    void goLeft();
    void goRight();

private:
    int width, height, selected = 0;
    const float thin_thickness  = 0.005f;
    const float thick_thickness = 0.012f;

    unsigned int VAO, VBO, EBO;
    const std::array<float, 2 * 4 * 8 * 2> vertices       = generateVertices();
    const std::array<unsigned int, 3 * 2 * 8 * 2> indices = generateIndices();

    glm::mat4 model = glm::mat4(1.0f);
    const glm::mat4 view = glm::mat4(1.0f);
    const glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);

    SelectionBox sb;

    constexpr std::array<float, 2 * 4 * 8 * 2> generateVertices() const;
    constexpr std::array<unsigned int, 3 * 2 * 8 * 2> generateIndices() const;
    void updateModel();
    void updateSelected();
};

#endif // GRID_HPP
