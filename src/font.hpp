#ifndef FONT_HPP
#define FONT_HPP

#include <map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "camera.hpp"

struct Character {
    unsigned int textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

// TODO: UTF-8 support
class Font {
public:
    Font(const char *font_path,
         const unsigned int font_size,
         const char *characters_to_load = nullptr);
    ~Font();

    const Character &getCharacter(const char c) const;
    void renderText(Camera *camera,
                    const std::string &text,
                    float x,
                    float y,
                    float scale,
                    glm::vec4 color,
                    bool respect_horizontal_bearing) const;

private:
    std::map<char, Character> characters;
    unsigned int VAO, VBO, EBO;
    const unsigned int indices[6] = {0, 1, 2, 0, 2, 3};
};

#endif // FONT_HPP
