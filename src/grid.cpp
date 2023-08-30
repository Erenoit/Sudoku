#include "grid.hpp"

Grid::Grid(Shader shader, int width, int height): shader(shader), width(width), height(height) {
    this->generateVertices();
    this->generateIndices();

    this->updateModel();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Grid::~Grid() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Grid::draw(glm::vec4 color) const {
    this->shader.use();
    this->shader.setUniform("color", color);
    this->shader.setUniform("projection", this->projection);
    this->shader.setUniform("view", this->view);
    this->shader.setUniform("model", this->model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3 * 2 * 8 * 2, GL_UNSIGNED_INT, 0);
}

void Grid::updateSize(int width, int height) {
    this->width = width;
    this->height = height;
    this->updateModel();
}

void Grid::generateVertices() {
    const float line_coordinates_1d[8] = {
        -0.77f, -0.55f, -0.33f, -0.11f, 0.11f, 0.33f, 0.55f, 0.77f,
    };

    for (size_t i = 0; i < 8; i++) {
        float half_thickness;
        if (i % 3 == 2) {
            half_thickness = thick_thickness / 2;
        } else {
            half_thickness = thin_thickness / 2;
        }

        // Horizontal line
        this->vertices[i * 16 +  0] = line_coordinates_1d[i] - half_thickness;
        this->vertices[i * 16 +  1] = 1.0f;
        this->vertices[i * 16 +  2] = line_coordinates_1d[i] - half_thickness;
        this->vertices[i * 16 +  3] = -1.0f;
        this->vertices[i * 16 +  4] = line_coordinates_1d[i] + half_thickness;
        this->vertices[i * 16 +  5] = 1.0f;
        this->vertices[i * 16 +  6] = line_coordinates_1d[i] + half_thickness;
        this->vertices[i * 16 +  7] = -1.0f;

        // Vertical line
        this->vertices[i * 16 +  8] = 1.0f;
        this->vertices[i * 16 +  9] = line_coordinates_1d[i] - half_thickness;
        this->vertices[i * 16 + 10] = -1.0f;
        this->vertices[i * 16 + 11] = line_coordinates_1d[i] - half_thickness;
        this->vertices[i * 16 + 12] = 1.0f;
        this->vertices[i * 16 + 13] = line_coordinates_1d[i] + half_thickness;
        this->vertices[i * 16 + 14] = -1.0f;
        this->vertices[i * 16 + 15] = line_coordinates_1d[i] + half_thickness;
    }
}

void Grid::generateIndices() {
    for (unsigned int i = 0; i < 16; i++) {
        this->indices[i * 6 + 0] = i * 4 + 0;
        this->indices[i * 6 + 1] = i * 4 + 1;
        this->indices[i * 6 + 2] = i * 4 + 2;
        this->indices[i * 6 + 3] = i * 4 + 1;
        this->indices[i * 6 + 4] = i * 4 + 2;
        this->indices[i * 6 + 5] = i * 4 + 3;
    }
}

void Grid::updateModel() {
    glm::vec3 scale;
    if (this->height > this->width) {
        scale = glm::vec3(1.0f, (float)this->width / this->height, 1.0f);
    } else {
        scale = glm::vec3((float)this->height / this->width, 1.0f, 1.0f);
    }

    this->model = glm::scale(glm::mat4(1.0f), scale);
}

