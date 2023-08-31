#include "resource_manager.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

std::map<std::string, std::shared_ptr<Shader>> ResourceManager::shaders;

std::shared_ptr<Shader> ResourceManager::loadShader(const std::string &name,
                                                    const char *vertex_path,
                                                    const char *fragment_path,
                                                    const char *geometry_path) {
    shaders[name] = loadShaderFromFile(vertex_path, fragment_path, geometry_path);
    return shaders[name];
}

std::shared_ptr<Shader> ResourceManager::getShader(const std::string &name) {
    return shaders[name];
}

void ResourceManager::clear() {
    shaders.clear();

    // for (auto iter : Textures)
    //     glDeleteTextures(1, &iter.second.ID);
}

std::shared_ptr<Shader> ResourceManager::loadShaderFromFile(const char *vertex_path,
                                                            const char *frgment_path,
                                                            const char *geometry_path) {
    std::string vertex_code, fragment_code, geometry_code;

    try {
        std::ifstream vertex_file(vertex_path), fragment_file(frgment_path);
        std::stringstream vertex_stream, fragment_stream;

        vertex_stream << vertex_file.rdbuf();
        fragment_stream << fragment_file.rdbuf();

        vertex_file.close();
        fragment_file.close();

        vertex_code   = vertex_stream.str();
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

    const char *vertex_cstr   = vertex_code.c_str();
    const char *fragment_cstr = fragment_code.c_str();
    const char *geometry_cstr = geometry_path != nullptr ? geometry_code.c_str() : nullptr;

    std::shared_ptr<Shader> shader =
        std::make_shared<Shader>(vertex_cstr, fragment_cstr, geometry_cstr);
    return shader;
}
