#include "game.hpp"

#include <GLFW/glfw3.h>

#include "resource_manager.hpp"

Game::Game(int width, int height): width(width), height(height) {
    for (size_t i = 0; i < this->numbers.size(); i++) {
        this->numbers[i].type   = NUMBER_CHANGABLE;
        this->numbers[i].number = 0;
    }
}

Game::~Game() {
    delete this->camera;
    delete this->grid;
    delete this->selection_box;

    ResourceManager::clear();
}

void Game::init() {
    ResourceManager::loadShader("standard",
                                "assets/shaders/standard_vertex.glsl",
                                "assets/shaders/standard_fragment.glsl");
    ResourceManager::loadShader("text",
                                "assets/shaders/text_vertex.glsl",
                                "assets/shaders/text_fragment.glsl");

    ResourceManager::loadFont("open_sans", "assets/fonts/OpenSans-Regular.ttf", 144);

    this->camera        = new Camera(this->width, this->height);
    this->grid          = new Grid();
    this->selection_box = new SelectionBox();
}

void Game::update() {
    // This game does not have any update logic.
    // TODO: maybe use in animation
}

void Game::draw() const {
    this->grid->draw(this->camera, this->grid_color);
    this->selection_box->draw(this->camera, this->selection_color);

    auto font = ResourceManager::getFont("open_sans");
    for (size_t i = 0; i < this->numbers.size(); i++) {
        const Number &cell = this->numbers[i];

        if (cell.number > 0) {
            font->renderChar(this->camera,
                             (char)(cell.number + '0'),
                             (i % 9) * 0.22f - 1.0f + 0.12f,
                             1.0f - 0.12f - (i / 9) * 0.22f,
                             1.0f / 9.0f * 1.0f,
                             cell.type == NUMBER_FIXED ? this->fixed_number
                                                       : this->changable_number);
        }
    }
}

void Game::processInput(int key, int action) {
    if ((key == GLFW_KEY_UP || key == GLFW_KEY_K) && action & (GLFW_PRESS | GLFW_REPEAT)) {
        this->selected = (this->selected - 9 + 81) % 81;
        this->selection_box->updateModel(this->selected);
    } else if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_J) && action & (GLFW_PRESS | GLFW_REPEAT)) {
        this->selected = (this->selected + 9) % 81;
        this->selection_box->updateModel(this->selected);
    } else if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_H) && action & (GLFW_PRESS | GLFW_REPEAT)) {
        this->selected = this->selected - (this->selected % 9) + (this->selected + 8) % 9;
        this->selection_box->updateModel(this->selected);
    } else if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_L)
               && action & (GLFW_PRESS | GLFW_REPEAT)) {
        this->selected = this->selected - (this->selected % 9) + (this->selected + 1) % 9;
        this->selection_box->updateModel(this->selected);
    } else if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9 && action == GLFW_PRESS) {
        auto &cell = this->numbers[this->selected];
        if (cell.type != NUMBER_FIXED) { cell.number = key - '0'; }
    } else if (key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_9 && action == GLFW_PRESS) {
        auto &cell = this->numbers[this->selected];
        if (cell.type != NUMBER_FIXED) { cell.number = key - GLFW_KEY_KP_0; }
    } else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
        auto &cell = this->numbers[this->selected];
        if (cell.type != NUMBER_FIXED) { cell.number = 0; }
    } else {
        std::cout << "key: " << key << ", action: " << action << std::endl;
    }
}

void Game::updateScreenSize(int width, int height) {
    this->width  = width;
    this->height = height;
    this->camera->updateScreenSize(width, height);
}

const glm::vec4 &Game::getBackgroundColor() const { return this->background_color; }
