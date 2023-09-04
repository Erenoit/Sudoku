#ifndef GAME_HPP
#define GAME_HPP

#include <array>
#include <shared_mutex>
#include <vector>

#include "camera.hpp"
#include "grid.hpp"
#include "selection_box.hpp"

enum NumberType {
    NUMBER_FIXED,
    NUMBER_CHANGABLE,
    NUMBER_NOTE, // TODO: implement
};

struct Number {
    NumberType type;
    unsigned int number;
};

enum ErrorType {
    ERROR_ROW,
    ERROR_COLUMN,
    ERROR_BOX,
};

struct Error {
    ErrorType type;
    unsigned int index;
    unsigned int number;

    bool operator==(const Error &e) const {
        return type == e.type && index == e.index && number == e.number;
    }

    bool operator!=(const Error &e) const { return !(*this == e); }
};

class Game {
public:
    Game(int width, int height);
    ~Game();

    void init();
    void update();
    void draw() const;
    void processInput(int key, int action);
    void updateScreenSize(int width, int height);
    const glm::vec4 &getBackgroundColor() const;

private:
    int width, height;
    unsigned int selected             = 0;
    std::array<Number, 9 * 9> numbers = {};
    mutable std::shared_mutex error_mutex;
    size_t error_count                  = 0;
    std::array<Error, 3 * 9 * 9> errors = {};

    unsigned int errorVAO, errorVBO, errorEBO;
    // clang-format off
    std::array<float, 2 * 4> error_vertices = {
        -1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f,
         1.0f, -1.0f,
    };
    std::array<unsigned int, 3 * 2> error_indices = {0, 1, 2, 2, 3, 0};
    // clang-format on

    Camera *camera;
    SelectionBox *selection_box;
    Grid *grid;

    const glm::vec4 background_color = getColor(0x16161EFF);
    const glm::vec4 grid_color       = getColor(0x3b4261FF);
    const glm::vec4 selection_color  = getColor(0x004CFF7F);
    const glm::vec4 fixed_number     = getColor(0xE0AF68FF);
    const glm::vec4 changable_number = getColor(0x7AA2F7FF);
    // const glm::vec4 error            = getColor(0xF7768EFF);
    const glm::vec4 error            = getColor(0x874656FF);

    constexpr glm::vec4 getColor(int color) const {
        const float r = ((color & 0xFF000000) >> 24) / 255.0f;
        const float g = ((color & 0x00FF0000) >> 16) / 255.0f;
        const float b = ((color & 0x0000FF00) >> 8) / 255.0f;
        const float a = ((color & 0x000000FF) >> 0) / 255.0f;

        return glm::vec4(r, g, b, a);
    }

    void updateErrors();
    unsigned int checkRow(unsigned int row) const;
    unsigned int checkColumn(unsigned int column) const;
    unsigned int checkBox(unsigned int box) const;
    void drawErrors() const;
};

#endif // GAME_HPP
