#ifndef SHADER_H
#define SHADER_H

#include "Types.h"
#include <string>

class Shader {
   public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    int32 GetUniformLocation(uint32 programId, const char *uniformName);

    void SetShaderUniformInt(int32 location, int32 value);

    std::string LoadShaderSource(const std::string &filePath);

    uint32 CompileShader(uint32 type, const std::string &source);

    ~Shader();

    void Use() const;
    uint32 GetProgramId();

   private:
    uint32 programID;
};

#endif
