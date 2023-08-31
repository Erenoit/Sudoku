#include "resource_manager.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

std::map<std::string, Shader*> ResourceManager::shaders;


Shader *ResourceManager::loadShader(std::string name, const char *vertex_path, const char *fragment_path, const char *geometry_path) {
    shaders[name] = loadShaderFromFile(vertex_path, fragment_path, geometry_path);
    return shaders[name];
}

Shader *ResourceManager::getShader(std::string name) {
    return shaders[name];
}


void ResourceManager::clear() {
    for (auto shader : shaders)
        delete &shader.second;

    //for (auto iter : Textures)
    //    glDeleteTextures(1, &iter.second.ID);
}

Shader *ResourceManager::loadShaderFromFile(const char *vertex_path, const char *frgment_path, const char *geometry_path) {
    std::string vertex_code, fragment_code, geometry_code;

    try {
        std::ifstream vertex_file(vertex_path), fragment_file(frgment_path);
        std::stringstream vertex_stream, fragment_stream;

        vertex_stream << vertex_file.rdbuf();
        fragment_stream << fragment_file.rdbuf();

        vertex_file.close();
        fragment_file.close();

        vertex_code = vertex_stream.str();
        fragment_code = fragment_stream.str();

        if (geometry_path != nullptr) {
            std::ifstream geometry_file(geometry_path);
            std::stringstream geometry_stream;
            geometry_stream << geometry_file.rdbuf();
            geometry_file.close();
            geometry_code = geometry_stream.str();
        }
    } catch (std::exception e) {
        std::cerr << "ERROR::SHADER: Failed to read shader files: " << e.what() << std::endl;
    }

    const char *vertex_cstr = vertex_code.c_str();
    const char *fragment_cstr = fragment_code.c_str();
    const char *geometry_cstr = geometry_path != nullptr ? geometry_code.c_str() : nullptr;

    Shader *shader = new Shader(vertex_cstr, fragment_cstr, geometry_cstr);
    return shader;
}
