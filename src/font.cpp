#include "font.hpp"

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "resource_manager.hpp"

Font::Font(const char *font_path, const unsigned int font_size, const char *characters_to_load) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Failed to initialize FreeType" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, font_path, 0, &face)) {
        std::cerr << "Failed to load font" << std::endl;
        return;
    }

    if (FT_Set_Pixel_Sizes(face, 0, font_size)) {
        std::cerr << "Failed to set font size" << std::endl;
        return;
    }

    // Disable byte-alignment restriction.
    // We get grayscale images, so we only need one byte per pixel
    // but OpenGL expects 4 bytes per pixel. This disables that restriction.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if (characters_to_load == nullptr) {
        characters_to_load = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&"
                             "*()-=_+[]{};':\",./<>?\\|`~ ";
    }

    for (const char *c = characters_to_load; *c != '\0'; c++) {
        if (FT_Load_Char(face, *c, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load glyph '" << *c << "'. Skipping..." << std::endl;
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {texture,
                               glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                               glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                               static_cast<unsigned int>(face->glyph->advance.x)};
        // this->character[*c] = character
        this->characters.insert(std::pair<char, Character>(*c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, this->indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Font::~Font() {
    for (auto &character : this->characters) glDeleteTextures(1, &character.second.textureID);
    this->characters.clear();
}

const Character &Font::getCharacter(const char c) const { return this->characters.at(c); }

void Font::renderText(Camera *camera,
                      const std::string &text,
                      float x,
                      float y,
                      float scale,
                      glm::vec4 color,
                      bool respect_horizontal_bearing) const {
    glm::mat4 position_matrix;
    glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0), glm::vec3(scale));

    auto shader = ResourceManager::getShader("text");
    shader->use();
    shader->setUniform("text_color", color);
    shader->setUniform("projection", camera->getProjection());
    shader->setUniform("view", camera->getView());

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);

    for (const char c : text) {
        const Character &ch = this->getCharacter(c);

        // Update position to write next character
        position_matrix = glm::translate(glm::mat4(1.0), glm::vec3(x, y, 0.0));
        shader->setUniform("model", position_matrix * scale_matrix);

        float normalizer = ch.size.x > ch.size.y ? 1.0f / ch.size.x : 1.0f / ch.size.y;
        float bearing_x  = respect_horizontal_bearing ? ch.bearing.x : 0.0f;

        // clang-format off
        float vertices[4 * 4] = {
            (bearing_x - ch.size.x / 2.0f) * normalizer, (ch.bearing.y - ch.size.y / 2.0f) * normalizer, 0.0f, 0.0f,
            (bearing_x - ch.size.x / 2.0f) * normalizer, (ch.bearing.y - ch.size.y * 1.5f) * normalizer, 0.0f, 1.0f,
            (bearing_x + ch.size.x / 2.0f) * normalizer, (ch.bearing.y - ch.size.y * 1.5f) * normalizer, 1.0f, 1.0f,
            (bearing_x + ch.size.x / 2.0f) * normalizer, (ch.bearing.y - ch.size.y / 2.0f) * normalizer, 1.0f, 0.0f,
        };
        // clang-format on

        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        float advance = respect_horizontal_bearing ? ch.advance >> 6 : ch.size.x;

        // FIXME: Some characters are not spaced correctly. for example e has a lot of space on the
        // right side, or l has too little space on th right side and collides with the next
        // character. (i.e. render 'Hello,')
        // Probably the reason is not accounting `normalizer` while calculating x for driving (not
        // the advance one).
        // FIXME: Also space does not work properly. (i.e. render 'Hello, World!')
        x += advance * normalizer * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
