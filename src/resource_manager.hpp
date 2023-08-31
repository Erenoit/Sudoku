#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <map>
#include <memory>
#include <string>

#include <glad/glad.h>

#include "shader.hpp"


class ResourceManager {
public:
    static std::map<std::string, std::shared_ptr<Shader>> shaders;

    static std::shared_ptr<Shader> loadShader(const std::string &name, const char *vertex_path, const char *fragment_path, const char *geometry_path = nullptr);
    static std::shared_ptr<Shader> getShader(const std::string &name);

    static void      clear();
private:
    ResourceManager() { }

    static std::shared_ptr<Shader> loadShaderFromFile(const char *vertex_path, const char *fragment_path, const char *geometry_path);
};

#endif // RESOURCE_MANAGER_HPP
