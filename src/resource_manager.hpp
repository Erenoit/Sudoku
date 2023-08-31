#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <map>
#include <string>

#include <glad/glad.h>

#include "shader.hpp"


class ResourceManager {
public:
    static std::map<std::string, Shader*>    shaders;

    static Shader *loadShader(std::string name, const char *vertex_path, const char *fragment_path, const char *geometry_path = nullptr);
    static Shader *getShader(std::string name);
    static void      clear();
private:
    ResourceManager() { }
    static Shader *loadShaderFromFile(const char *vertex_path, const char *fragment_path, const char *geometry_path);
};

#endif // RESOURCE_MANAGER_HPP
