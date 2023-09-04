#include "game.hpp"

#include <algorithm>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <mutex>

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

    glDeleteVertexArrays(1, &this->errorVAO);
    glDeleteBuffers(1, &this->errorVBO);
    glDeleteBuffers(1, &this->errorEBO);
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

    glGenVertexArrays(1, &this->errorVAO);
    glGenBuffers(1, &this->errorVBO);
    glGenBuffers(1, &this->errorEBO);

    glBindVertexArray(this->errorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->errorVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(this->error_vertices),
                 &this->error_vertices,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->errorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(this->error_indices),
                 &this->error_indices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Game::update() {
    // This game does not have any update logic.
    // TODO: maybe use in animation
}

void Game::draw() const {
    // Draw red background for errors
    this->drawErrors();

    this->grid->draw(this->camera, this->grid_color);
    this->selection_box->draw(this->camera, this->selection_color);

    // Draw numbers on the grid
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
        this->updateErrors();
    } else if (key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_9 && action == GLFW_PRESS) {
        auto &cell = this->numbers[this->selected];
        if (cell.type != NUMBER_FIXED) { cell.number = key - GLFW_KEY_KP_0; }
        this->updateErrors();
    } else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
        auto &cell = this->numbers[this->selected];
        if (cell.type != NUMBER_FIXED) { cell.number = 0; }
        this->updateErrors();
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

void Game::updateErrors() {
    std::unique_lock<std::shared_mutex> error_lock(this->error_mutex, std::defer_lock);

    // check the errors already in the vector still valid
    for (size_t i = 0; i < this->error_count;) {
        Error *err = &this->errors[i];
        if (err->number == 0) {
            i++;
            continue;
        }

        if (err->type == ERROR_ROW) {
            if (unsigned int error = this->checkRow(err->index)) {
                err->number = error;
                i++;
            } else {
                (void)std::remove(this->errors.begin(), this->errors.end(), *err);
                this->error_count--;
            }
        } else if (err->type == ERROR_COLUMN) {
            if (unsigned int error = this->checkColumn(err->index)) {
                i++;
                err->number = error;
            } else {
                (void)std::remove(this->errors.begin(), this->errors.end(), *err);
                this->error_count--;
            }
        } else if (err->type == ERROR_BOX) {
            if (unsigned int error = this->checkBox(err->index)) {
                i++;
                err->number = error;
            } else {
                (void)std::remove(this->errors.begin(), this->errors.end(), *err);
                this->error_count--;
            }
        }
    }

    // Check for new errors
    const unsigned int row = this->selected / 9;
    if (unsigned int error = this->checkRow(row)) {
        Error new_err = {ERROR_ROW, row, error};
        if (std::find(this->errors.begin(), this->errors.end(), new_err) == this->errors.end())
            this->errors[this->error_count++] = new_err;
    }

    const unsigned int column = this->selected % 9;
    if (unsigned int error = this->checkColumn(column)) {
        Error new_err = {ERROR_COLUMN, column, error};
        if (std::find(this->errors.begin(), this->errors.end(), new_err) == this->errors.end())
            this->errors[this->error_count++] = new_err;
    }

    const unsigned int box = (this->selected / 9) / 3 * 3 + (this->selected % 9) / 3;
    if (unsigned int error = this->checkBox(box)) {
        Error new_err = {ERROR_BOX, box, error};
        if (std::find(this->errors.begin(), this->errors.end(), new_err) == this->errors.end())
            this->errors[this->error_count++] = new_err;
    }
}

unsigned int Game::checkRow(unsigned int row) const {
    int found_numbers = 0;
    for (unsigned int i = row * 9; i < row * 9 + 9; i++) {
        if (this->numbers[i].number == 0) { continue; }

        int n = 1 << (this->numbers[i].number - 1);
        if (found_numbers & n) {
            return this->numbers[i].number;
        } else {
            found_numbers |= n;
        }
    }

    return 0;
}

unsigned int Game::checkColumn(unsigned int column) const {
    int found_numbers = 0;
    for (unsigned int i = column; i < 81; i += 9) {
        if (this->numbers[i].number == 0) { continue; }

        int n = 1 << (this->numbers[i].number - 1);
        if (found_numbers & n) {
            return this->numbers[i].number;
        } else {
            found_numbers |= n;
        }
    }

    return 0;
}

unsigned int Game::checkBox(unsigned int box) const {
    int found_numbers = 0;
    for (unsigned int i = 0; i < 9; i++) {
        unsigned int index = (box / 3) * 27 + (box % 3) * 3 + (i / 3) * 9 + (i % 3);
        if (this->numbers[index].number == 0) { continue; }

        int n = 1 << (this->numbers[index].number - 1);
        if (found_numbers & n) {
            return this->numbers[index].number;
        } else {
            found_numbers |= n;
        }
    }

    return 0;
}

void Game::drawErrors() const {
    std::shared_lock<std::shared_mutex> lock(this->error_mutex, std::defer_lock);

    auto shader = ResourceManager::getShader("standard");
    shader->use();
    shader->setUniform("color", this->error);
    shader->setUniform("projection", this->camera->getProjection());
    shader->setUniform("view", this->camera->getView());

    glBindVertexArray(errorVAO);

    glm::mat4 scale;
    glm::mat4 position;

    for (size_t i = 0; i < this->error_count; i++) {
        const Error &err = this->errors[i];

        if (err.type == ERROR_ROW) {
            scale    = glm::scale(glm::mat4(1.0f), glm::vec3(0.11f * 9, 0.11f, 1.0f));
            position = glm::translate(glm::mat4(1.0f),
                                      glm::vec3(0.0f, 1.0f - 0.22f * err.index - 0.12f, 0.0f));
        } else if (err.type == ERROR_COLUMN) {
            scale    = glm::scale(glm::mat4(1.0f), glm::vec3(0.11f, 0.11f * 9, 1.0f));
            position = glm::translate(glm::mat4(1.0f),
                                      glm::vec3(-1.0f + 0.22f * err.index + 0.12f, 0.0f, 0.0f));
        } else if (err.type == ERROR_BOX) {
            scale    = glm::scale(glm::mat4(1.0f), glm::vec3(0.11f * 3, 0.11f * 3, 1.0f));
            position = glm::translate(glm::mat4(1.0f),
                                      glm::vec3(-1.0f + 0.34f + 0.22f * 3 * (err.index % 3),
                                                1.0f - 0.34f - 0.22f * 3 * (err.index / 3),
                                                0.0f));
        }

        shader->setUniform("model", position * scale);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}
