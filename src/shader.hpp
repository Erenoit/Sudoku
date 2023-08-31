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

    void setUniform(const std::string &name, bool value) const;
    void setUniform(const std::string &name, int value) const;
    void setUniform(const std::string &name, unsigned int value) const;
    void setUniform(const std::string &name, float value) const;
    void setUniform(const std::string &name, glm::vec2 value) const;
    void setUniform(const std::string &name, glm::vec3 value) const;
    void setUniform(const std::string &name, glm::vec4 value) const;
    void setUniform(const std::string &name, glm::mat2 value) const;
    void setUniform(const std::string &name, glm::mat3 value) const;
    void setUniform(const std::string &name, glm::mat4 value) const;

private:
    unsigned int ID;

    void checkCompileErrors(unsigned int shader, std::string type) const;
    void checkLinkingErrors(unsigned int shader, std::string type) const;
};

#endif // SHADER_HPP
