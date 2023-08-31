#ifndef SELECTED_BOX_HPP
#define SELECTED_BOX_HPP

#include <array>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define SAMPLES 10 // Number of triangles to round corners

class SelectionBox {
public:
    SelectionBox();
    ~SelectionBox();

    void updateModel(glm::mat4 model);
    void draw(glm::vec4 color) const;

private:
    const float radius = 0.15f;

    unsigned int VAO, VBO, EBO;
    const std::array<float, 2 * (SAMPLES + 1) * 4> vertices = generateVertices();
    const std::array<unsigned int, 3 * (SAMPLES - 1 + 2) * 4 + 6> indices = generateIndices();

    glm::mat4 model = glm::mat4(1.0f);
    const glm::mat4 view = glm::mat4(1.0f);
    const glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);

    constexpr std::array<float, 2 * (SAMPLES + 1) * 4> generateVertices() const;
    constexpr std::array<unsigned int, 3 * (SAMPLES - 1 + 2) * 4 + 6> generateIndices() const;
};

#endif // SELECTED_BOX_HPP
