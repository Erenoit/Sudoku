#include "game.hpp"

#include <GLFW/glfw3.h>

#include "resource_manager.hpp"

Game::Game(int width, int height): width(width), height(height) { }

Game::~Game() {
    delete this->camera;
    delete this->grid;
    delete this->selection_box;
}

void Game::init() {
    ResourceManager::loadShader("standard",
                                "assets/shaders/standard_vertex.glsl",
                                "assets/shaders/standard_fragment.glsl");

    this->camera        = new Camera(this->width, this->height);
    this->grid          = new Grid();
    this->selection_box = new SelectionBox();
}

void Game::update() {
    // This game does not have any update logic.
    // TODO: maybe use in animation
}

void Game::draw() const {
    this->grid->draw(this->camera, this->primary);
    this->selection_box->draw(this->camera, this->secondary);
}

void Game::processInput(int key, int action) {
    if (key == GLFW_KEY_UP && action & (GLFW_PRESS | GLFW_REPEAT)) {
        this->selected = (this->selected - 9 + 81) % 81;
        this->selection_box->updateModel(this->selected);
    } else if (key == GLFW_KEY_DOWN && action & (GLFW_PRESS | GLFW_REPEAT)) {
        this->selected = (this->selected + 9) % 81;
        this->selection_box->updateModel(this->selected);
    } else if (key == GLFW_KEY_LEFT && action & (GLFW_PRESS | GLFW_REPEAT)) {
        this->selected = this->selected - (this->selected % 9) + (this->selected + 8) % 9;
        this->selection_box->updateModel(this->selected);
    } else if (key == GLFW_KEY_RIGHT && action & (GLFW_PRESS | GLFW_REPEAT)) {
        this->selected = this->selected - (this->selected % 9) + (this->selected + 1) % 9;
        this->selection_box->updateModel(this->selected);
    }
}

void Game::updateScreenSize(int width, int height) {
    this->width  = width;
    this->height = height;
    this->camera->updateScreenSize(width, height);
}

const glm::vec4 &Game::getBackground() const { return this->background; }
