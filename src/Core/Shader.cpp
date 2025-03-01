#include "../../include/Core/Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

#include "../../vendor/glew-2.2.0/include/GL/glew.h"

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    std::string vertexCode = LoadShaderSource(vertexPath);
    std::string fragmentCode = LoadShaderSource(fragmentPath);

    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexCode);
    unsigned int fragmentShader =
        CompileShader(GL_FRAGMENT_SHADER, fragmentCode);

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

unsigned int Shader::GetProgramId() { return programID; }

Shader::~Shader() { glDeleteProgram(programID); }

void Shader::Use() const {
    glUseProgram(programID);
    GLint colorLoc = glGetUniformLocation(programID, "color");
    glUniform3f(colorLoc, 0.5f, 0.2f, 0.7f);
}

std::string Shader::LoadShaderSource(const std::string &filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::CompileShader(unsigned int type,
                                   const std::string &source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }

    return id;
}
