#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(int width, int height): width(width), height(height) {
    this->updateScreenSize(width, height);
    this->projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
}

Camera::~Camera() {}

const glm::mat4 &Camera::getView() const { return this->view; }

const glm::mat4 &Camera::getProjection() const { return this->projection; }

void Camera::updateScreenSize(int width, int height) {
    this->width = width;
    this->height = height;

    glm::vec3 scale;
    if (this->height > this->width) {
        scale = glm::vec3(1.0f, (float)this->width / this->height, 1.0f);
    } else {
        scale = glm::vec3((float)this->height / this->width, 1.0f, 1.0f);
    }

    this->view = glm::scale(glm::mat4(1.0f), scale);
}
