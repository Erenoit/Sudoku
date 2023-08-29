#include "shader.hpp"

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertex_shader_code;
    std::string fragment_shader_code;
    std::ifstream vertex_shader_file;
    std::ifstream fragent_shader_file;

    vertex_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fragent_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        vertex_shader_file.open(vertexPath);
        fragent_shader_file.open(fragmentPath);

        std::stringstream vertex_shader_stream, fragment_shader_stream;
        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragent_shader_file.rdbuf();

        vertex_shader_file.close();
        fragent_shader_file.close();

        vertex_shader_code   = vertex_shader_stream.str();
        fragment_shader_code = fragment_shader_stream.str();
    } catch(std::ifstream::failure e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << e.what() << std::endl;
    }

    const char* vertex_shader_cstr = vertex_shader_code.c_str();
    const char* fragment_hader_cstr = fragment_shader_code.c_str();

    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader_cstr, NULL);
    glCompileShader(vertex);
    this->checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_hader_cstr, NULL);
    glCompileShader(fragment);
    this->checkCompileErrors(fragment, "FRAGMENT");

    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);
    glLinkProgram(this->ID);
    this->checkLinkingErrors(this->ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const {
    glUseProgram(this->ID);
}

void Shader::setUniform(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}
void Shader::setUniform(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::setUniform(const std::string &name, unsigned int value) const {
    glUniform1ui(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::setUniform(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::setUniform(const std::string &name, glm::vec2 value) const {
    glUniform2fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::setUniform(const std::string &name, glm::vec3 value) const {
    glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::setUniform(const std::string &name, glm::vec4 value) const {
    glUniform4fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::setUniform(const std::string &name, glm::mat2 value) const {
    glUniformMatrix2fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setUniform(const std::string &name, glm::mat3 value) const {
    glUniformMatrix3fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setUniform(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::checkCompileErrors(unsigned int shader, std::string type) const {
    int success;
    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
}

void Shader::checkLinkingErrors(unsigned int shader, std::string type) const {
    int success;
    char infoLog[512];

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if(!success) {
         glGetProgramInfoLog(shader, 512, NULL, infoLog);
         std::cerr << "ERROR::SHADER::" << type << "::LINKING_FAILED\n" << infoLog << std::endl;
    };
}
