#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
    Shader(const char* vertex_code, const char* fragment_code, const char* geometry_code = nullptr);
    ~Shader();

    void use() const;

    void setUniform(const std::string &name, const bool value) const;
    void setUniform(const std::string &name, const int value) const;
    void setUniform(const std::string &name, const unsigned int value) const;
    void setUniform(const std::string &name, const float value) const;
    void setUniform(const std::string &name, const glm::vec2 &value) const;
    void setUniform(const std::string &name, const glm::vec3 &value) const;
    void setUniform(const std::string &name, const glm::vec4 &value) const;
    void setUniform(const std::string &name, const glm::mat2 &value) const;
    void setUniform(const std::string &name, const glm::mat3 &value) const;
    void setUniform(const std::string &name, const glm::mat4 &value) const;

private:
    unsigned int ID;

    void checkCompileErrors(const unsigned int shader, const std::string &type) const;
    void checkLinkingErrors(const unsigned int shader, const std::string &type) const;
};

#endif // SHADER_HPP
