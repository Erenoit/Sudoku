#include "shader.hpp"

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertex_code, const char* fragment_code, const char* geometry_code) {
    unsigned int vertex, fragment, geometry;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_code, nullptr);
    glCompileShader(vertex);
    this->checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_code, nullptr);
    glCompileShader(fragment);
    this->checkCompileErrors(fragment, "FRAGMENT");

    if (geometry_code != nullptr) {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometry_code, nullptr);
        glCompileShader(geometry);
        this->checkCompileErrors(geometry, "GEOMETRY");
    }

    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);
    if (geometry_code != nullptr) glAttachShader(this->ID, geometry);
    glLinkProgram(this->ID);
    this->checkLinkingErrors(this->ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(this->ID);
}

void Shader::use() const {
    glUseProgram(this->ID);
}

void Shader::setUniform(const std::string &name, const bool value) const {
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}
void Shader::setUniform(const std::string &name, const int value) const {
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::setUniform(const std::string &name, const unsigned int value) const {
    glUniform1ui(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::setUniform(const std::string &name, const float value) const {
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::setUniform(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::setUniform(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::setUniform(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::setUniform(const std::string &name, const glm::mat2 &value) const {
    glUniformMatrix2fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setUniform(const std::string &name, const glm::mat3 &value) const {
    glUniformMatrix3fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setUniform(const std::string &name, const glm::mat4 &value) const {
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::checkCompileErrors(const unsigned int shader, const std::string &type) const {
    int success;
    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
}

void Shader::checkLinkingErrors(const unsigned int shader, const std::string &type) const {
    int success;
    char infoLog[512];

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if(!success) {
         glGetProgramInfoLog(shader, 512, NULL, infoLog);
         std::cerr << "ERROR::SHADER::" << type << "::LINKING_FAILED\n" << infoLog << std::endl;
    };
}
