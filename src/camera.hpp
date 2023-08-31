#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera {
public:
    Camera(int width, int height);
    ~Camera();

    const glm::mat4 &getView() const;
    const glm::mat4 &getProjection() const;

    void updateScreenSize(int width, int height);

private:
    int width, height;
    glm::mat4 view, projection;
};

#endif // CAMERA_HPP
