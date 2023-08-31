#ifndef GRID_HPP
#define GRID_HPP

#include <array>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "camera.hpp"

class Grid {
public:
    Grid();
    ~Grid();

    void draw(const Camera *camera, const glm::vec4 &color) const;

private:
    const float thin_thickness  = 0.005f;
    const float thick_thickness = 0.012f;

    unsigned int VAO, VBO, EBO;
    const std::array<float, 2 * 4 * 8 * 2> vertices       = generateVertices();
    const std::array<unsigned int, 3 * 2 * 8 * 2> indices = generateIndices();

    const glm::mat4 model = glm::mat4(1.0f);

    constexpr std::array<float, 2 * 4 * 8 * 2> generateVertices() const;
    constexpr std::array<unsigned int, 3 * 2 * 8 * 2> generateIndices() const;
};

#endif // GRID_HPP
