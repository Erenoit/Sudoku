#include "grid.hpp"

#include "resource_manager.hpp"

Grid::Grid() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Grid::~Grid() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Grid::draw(const Camera *camera, const glm::vec4 &color) const {
    auto shader = ResourceManager::getShader("standard_shader");
    shader->use();
    shader->setUniform("color", color);
    shader->setUniform("projection", camera->getProjection());
    shader->setUniform("view", camera->getView());
    shader->setUniform("model", this->model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3 * 2 * 8 * 2, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

constexpr std::array<float, 2 * 4 * 8 * 2> Grid::generateVertices() const {
    std::array<float, 2 * 4 * 8 * 2> result;
    const float line_coordinates_1d[8] = {
      -0.77f, -0.55f, -0.33f, -0.11f, 0.11f, 0.33f, 0.55f, 0.77f,
    };

    for (size_t i = 0; i < 8; i++) {
        float half_thickness;
        if (i % 3 == 2) {
            half_thickness = this->thick_thickness / 2;
        } else {
            half_thickness = this->thin_thickness / 2;
        }

        // Horizontal line
        result[i * 16 +  0] = line_coordinates_1d[i] - half_thickness;
        result[i * 16 +  1] = 0.99f;
        result[i * 16 +  2] = line_coordinates_1d[i] - half_thickness;
        result[i * 16 +  3] = -0.99f;
        result[i * 16 +  4] = line_coordinates_1d[i] + half_thickness;
        result[i * 16 +  5] = 0.99f;
        result[i * 16 +  6] = line_coordinates_1d[i] + half_thickness;
        result[i * 16 +  7] = -0.99f;

        // Vertical line
        result[i * 16 +  8] = 0.99f;
        result[i * 16 +  9] = line_coordinates_1d[i] - half_thickness;
        result[i * 16 + 10] = -0.99f;
        result[i * 16 + 11] = line_coordinates_1d[i] - half_thickness;
        result[i * 16 + 12] = 0.99f;
        result[i * 16 + 13] = line_coordinates_1d[i] + half_thickness;
        result[i * 16 + 14] = -0.99f;
        result[i * 16 + 15] = line_coordinates_1d[i] + half_thickness;
    }

    return result;
}

constexpr std::array<unsigned int, 3 * 2 * 8 * 2> Grid::generateIndices() const {
    std::array<unsigned int, 3 * 2 * 8 * 2> result;

    for (unsigned int i = 0; i < 16; i++) {
        result[i * 6 + 0] = i * 4 + 0;
        result[i * 6 + 1] = i * 4 + 1;
        result[i * 6 + 2] = i * 4 + 2;
        result[i * 6 + 3] = i * 4 + 1;
        result[i * 6 + 4] = i * 4 + 2;
        result[i * 6 + 5] = i * 4 + 3;
    }

    return result;
}
