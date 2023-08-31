#ifndef SELECTED_BOX_HPP
#define SELECTED_BOX_HPP

#include <array>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "camera.hpp"

#define SAMPLES 50 // Number of triangles to single round corner

class SelectionBox {
public:
    SelectionBox();
    ~SelectionBox();

    void draw(const Camera *camera, const glm::vec4 &color) const;
    void updateModel(const int grid_position);

private:
    const float radius = 0.15f;

    unsigned int VAO, VBO, EBO;
    const std::array<float, 2 * (SAMPLES + 1) * 4> vertices = generateVertices();
    const std::array<unsigned int, 3 * (SAMPLES - 1 + 2) * 4 + 6> indices = generateIndices();

    glm::mat4 model = glm::mat4(1.0f);

    constexpr std::array<float, 2 * (SAMPLES + 1) * 4> generateVertices() const;
    constexpr std::array<unsigned int, 3 * (SAMPLES - 1 + 2) * 4 + 6> generateIndices() const;
};

#endif // SELECTED_BOX_HPP
