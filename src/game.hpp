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
    const glm::vec4 &getBackground() const;

private:
    int width, height, selected = 0;
    Camera *camera;
    SelectionBox *selection_box;
    Grid *grid;

    const glm::vec4 background = glm::vec4(0x2D / 255.0f, 0x51 / 266.0f, 0x59 / 255.0f, 1.0f);
    const glm::vec4 primary    = glm::vec4(0x48 / 255.0f, 0x7D / 255.0f, 0x67 / 255.0f, 1.0f);
    const glm::vec4 secondary  = glm::vec4(0xC5 / 255.0f, 0xC0 / 255.0f, 0x97 / 255.0f, 1.0f);
    const glm::vec4 yellow     = glm::vec4(0xF6 / 255.0f, 0xCE / 255.0f, 0x6B / 255.0f, 1.0f);
    const glm::vec4 red        = glm::vec4(0xC7 / 255.0f, 0x2F / 255.0f, 0x28 / 255.0f, 1.0f);
};

#endif // GAME_HPP
