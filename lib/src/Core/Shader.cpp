#include "../../include/Core/Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

#include "../../../vendor/glew-2.2.0/include/GL/glew.h"

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    std::string vertexCode = LoadShaderSource(vertexPath);
    std::string fragmentCode = LoadShaderSource(fragmentPath);

    uint32 vertexShader = CompileShader(GL_VERTEX_SHADER, vertexCode);
    uint32 fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentCode);

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

uint32 Shader::GetProgramId() { return programID; }

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

uint32 Shader::CompileShader(uint32 type, const std::string &source) {
    uint32 id = glCreateShader(type);
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

int32 Shader::GetUniformLocation(uint32 programId, const char *uniformName) {
    return glGetUniformLocation(programId, uniformName);
}

void Shader::SetShaderUniformInt(int32 location, int32 value) {
    glUniform1i(location, value);
}