#include "grid.hpp"
#include "resource_manager.hpp"

Grid::Grid(int width, int height) : width(width), height(height) {
    this->updateModel();

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

void Grid::draw(glm::vec4 color) const {
    auto shader = ResourceManager::getShader("standard_shader");
    shader->use();
    shader->setUniform("color", color);
    shader->setUniform("projection", this->projection);
    shader->setUniform("view", this->view);
    shader->setUniform("model", this->model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3 * 2 * 8 * 2, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    this->sb.draw(glm::vec4(0xC5 / 255.0f, 0xC0 / 255.0f, 0x97 / 255.0f, 0.5f));
}

void Grid::updateSize(int width, int height) {
    this->width = width;
    this->height = height;
    this->updateModel();
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

void Grid::updateModel() {
    glm::vec3 scale;
    if (this->height > this->width) {
        scale = glm::vec3(1.0f, (float)this->width / this->height, 1.0f);
    } else {
        scale = glm::vec3((float)this->height / this->width, 1.0f, 1.0f);
    }

    this->model = glm::scale(glm::mat4(1.0f), scale);
    this->updateSelected();
}

void:: Grid::updateSelected() {
    // thick line margin fix size/position x and y
    float tlmfsx, tlmfsy, tlmfpx, tlmfpy;
    const float margin = 0.01f, cell_offset = 0.12f, cell_size = 0.22f;
    const float thickness_diff_half = (this->thick_thickness - this->thin_thickness) / 2;

    if (selected % 3 == 1) {
        tlmfsx = 0.0f;
        tlmfpx = 0.0f;
    } else if (selected % 3 == 2) {
        tlmfsx = thickness_diff_half;
        tlmfpx = -thickness_diff_half / 2;
    } else {
        tlmfsx = thickness_diff_half;
        tlmfpx = thickness_diff_half / 2;
    }

    if ((selected / 9) % 3 == 1) {
        tlmfsy = 0.0f;
        tlmfpy = 0.0f;
    } else if ((selected / 9) % 3 == 2) {
        tlmfsy = thickness_diff_half;
        tlmfpy = -thickness_diff_half / 2;
    } else {
        tlmfsy = thickness_diff_half;
        tlmfpy = thickness_diff_half / 2;
    }

    glm::vec3 scale = glm::vec3(
            1.0f / 9.0f - margin - tlmfsx,
            1.0f / 9.0f - margin - tlmfsy,
            1.0f);
    glm::vec3 position = glm::vec3(
            -(1 - cell_offset - (this->selected % 9) * cell_size - tlmfpx),
             (1 - cell_offset - (this->selected / 9) * cell_size - tlmfpy),
             0.0f);

    sb.updateModel(this->model * glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), scale));
}

void Grid::goUp() { 
    this->selected = (this->selected - 9 + 81) % 81;
    this->updateModel();
}

void Grid::goDown() { 
    this->selected = (this->selected + 9) % 81;
    this->updateModel();
}

void Grid::goLeft() {
    this->selected = this->selected - (this->selected % 9) + (this->selected + 8) % 9;
    this->updateModel();
}

void Grid::goRight() {
    this->selected = this->selected - (this->selected % 9) + (this->selected + 1) % 9;
    this->updateModel();
}
