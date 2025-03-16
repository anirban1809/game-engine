#ifndef SHADER_H
#define SHADER_H

#include "Core/Types.h"
#include <string>

class Shader {
   public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    ~Shader();

    void Use() const;
    uint32 GetProgramId();

   private:
    uint32 programID;
    std::string LoadShaderSource(const std::string &filePath);
    uint32 CompileShader(uint32 type, const std::string &source);
};

#endif
