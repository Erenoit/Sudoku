#ifndef GAME_HPP
#define GAME_HPP

#include "camera.hpp"
#include "grid.hpp"
#include "selection_box.hpp"

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
    int width, height, selected = 0;
    Camera *camera;
    SelectionBox *selection_box;
    Grid *grid;

    const glm::vec4 background_color = getColor(0x16161EFF);
    const glm::vec4 grid_color       = getColor(0x3b4261FF);
    const glm::vec4 selection_color  = getColor(0x004CFF7F);
    const glm::vec4 fixed_number     = getColor(0xE0AF68FF);
    const glm::vec4 changable_number = getColor(0x7AA2F7FF);
    const glm::vec4 error            = getColor(0xF7768EFF);

    constexpr glm::vec4 getColor(int color) const {
        const float r = ((color & 0xFF000000) >> 24) / 255.0f;
        const float g = ((color & 0x00FF0000) >> 16) / 255.0f;
        const float b = ((color & 0x0000FF00) >> 8) / 255.0f;
        const float a = ((color & 0x000000FF) >> 0) / 255.0f;

        return glm::vec4(r, g, b, a);
    }
};

#endif // GAME_HPP
