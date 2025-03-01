#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader {
   public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    ~Shader();

    void Use() const;
    unsigned int GetProgramId();

   private:
    unsigned int programID;
    std::string LoadShaderSource(const std::string &filePath);
    unsigned int CompileShader(unsigned int type, const std::string &source);
};

#endif
