#include "selection_box.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "resource_manager.hpp"

SelectionBox::SelectionBox() {
    this->updateModel(0);

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

SelectionBox::~SelectionBox() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void SelectionBox::draw(const Camera *camera, const glm::vec4 &color) const {
    auto shader = ResourceManager::getShader("standard_shader");
    shader->use();
    shader->setUniform("color", glm::vec4((glm::vec3)color, 0.5f));
    shader->setUniform("projection", camera->getProjection());
    shader->setUniform("view", camera->getView());
    shader->setUniform("model", this->model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3 * (SAMPLES - 1 + 2) * 4 + 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

constexpr std::array<float, 2 * (SAMPLES + 1) * 4> SelectionBox::generateVertices() const {
    std::array<float, 2 * (SAMPLES + 1) * 4> result;
    const float signs[] = {
        -1.0f,  1.0f,
         1.0f,  1.0f,
         1.0f, -1.0f,
        -1.0f, -1.0f,
    };

    for (unsigned int i = 0; i < 4; i++) {
        const float x_sign = signs[i * 2 + 0];
        const float y_sign = signs[i * 2 + 1];

        result[(SAMPLES * 2 + 2) * i + 0] = x_sign * (1.0f - radius);
        result[(SAMPLES * 2 + 2) * i + 1] = y_sign * (1.0f - radius);

        for (unsigned int j = 0 ; j < SAMPLES; j++) {
            result[(SAMPLES * 2 + 2) * i + 2 + j * 2 + 0] = x_sign * (1.0f - radius + (radius * cos((float)j * M_PI / 2.0f / SAMPLES)));
            result[(SAMPLES * 2 + 2) * i + 2 + j * 2 + 1] = y_sign * (1.0f - radius + (radius * sin((float)j * M_PI / 2.0f / SAMPLES)));
        }
    }

    return result;
}

constexpr std::array<unsigned int, 3 * (SAMPLES - 1 + 2) * 4 + 6> SelectionBox::generateIndices() const {
    std::array<unsigned int, 3 * (SAMPLES - 1 + 2) * 4 + 6> result;

    for (int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < SAMPLES - 1; j++) {
            result[i * (SAMPLES - 1) * 3 + j * 3 + 0] = i * (SAMPLES + 1) + 0;
            result[i * (SAMPLES - 1) * 3 + j * 3 + 1] = i * (SAMPLES + 1) + j + 1;
            result[i * (SAMPLES - 1) * 3 + j * 3 + 2] = i * (SAMPLES + 1) + j + 2;
        }
    }

    result[(SAMPLES - 1) * 4 * 3 +  0] = (SAMPLES + 1) * 0;
    result[(SAMPLES - 1) * 4 * 3 +  1] = (SAMPLES + 1) * 1;
    result[(SAMPLES - 1) * 4 * 3 +  2] = (SAMPLES + 1) * 2;
    result[(SAMPLES - 1) * 4 * 3 +  3] = (SAMPLES + 1) * 0;
    result[(SAMPLES - 1) * 4 * 3 +  4] = (SAMPLES + 1) * 2;
    result[(SAMPLES - 1) * 4 * 3 +  5] = (SAMPLES + 1) * 3;

    for (unsigned int i = 0; i < 4; i++) {
        const unsigned int offset = i % 2 ? 1 : SAMPLES;
        const unsigned int other_corner = (i + 1) % 4;

        result[(SAMPLES - 1) * 4 * 3 + (i + 1) * 6 + 0] = (SAMPLES + 1) * i;
        result[(SAMPLES - 1) * 4 * 3 + (i + 1) * 6 + 1] = (SAMPLES + 1) * i + offset;
        result[(SAMPLES - 1) * 4 * 3 + (i + 1) * 6 + 2] = (SAMPLES + 1) * other_corner;

        result[(SAMPLES - 1) * 4 * 3 + (i + 1) * 6 + 3] = (SAMPLES + 1) * i + offset;
        result[(SAMPLES - 1) * 4 * 3 + (i + 1) * 6 + 4] = (SAMPLES + 1) * other_corner;
        result[(SAMPLES - 1) * 4 * 3 + (i + 1) * 6 + 5] = (SAMPLES + 1) * other_corner + offset;
    }

    return result;
}

void SelectionBox::updateModel(const int grid_position) {
        // thick line margin fix size/position x and y
    float tlmfsx, tlmfsy, tlmfpx, tlmfpy;
    const float margin = 0.01f, cell_offset = 0.12f, cell_size = 0.22f;
    //const float thickness_diff_half = (this->thick_thickness - this->thin_thickness) / 2;
    const float thickness_diff_half = (0.012 - 0.005) / 2;

    if (grid_position % 3 == 1) {
        tlmfsx = 0.0f;
        tlmfpx = 0.0f;
    } else if (grid_position % 3 == 2) {
        tlmfsx = thickness_diff_half;
        tlmfpx = -thickness_diff_half / 2;
    } else {
        tlmfsx = thickness_diff_half;
        tlmfpx = thickness_diff_half / 2;
    }

    if ((grid_position / 9) % 3 == 1) {
        tlmfsy = 0.0f;
        tlmfpy = 0.0f;
    } else if ((grid_position / 9) % 3 == 2) {
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
            -(1 - cell_offset - (grid_position % 9) * cell_size - tlmfpx),
             (1 - cell_offset - (grid_position / 9) * cell_size - tlmfpy),
             0.0f);

    this->model = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), scale);
}
