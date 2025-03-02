#ifndef __WAVEFRONT_H__
#define __WAVEFRONT_H__

#include "../Core/Types.h"
#include <string>
#include <tuple>
#include <vector>

#define vec3float float, float, float
#define Triangle std::tuple<FaceVertex, FaceVertex, FaceVertex>

typedef struct {
    uint32 v;   // vertex
    uint32 vt;  // texture
    uint32 vn;  // normal
} FaceVertex;

typedef struct {
    float x;
    float y;
    float z;
} VectorNormal;

typedef struct {
    float u;
    float v;
} VectorTexture;

enum class LineType {
    Coordinates,
    NormalVectors,
    Indices,
};

class WavefrontObjLoader {
   public:
    void Load(const std::string& filePath);
    void ParseContent();
    std::vector<std::tuple<vec3float>>& GetVertices();
    std::vector<Triangle>& GetTriangles();
    std::vector<VectorTexture>& GetVectorTextures();
    std::vector<VectorNormal>& GetVectorNormals();

   private:
    std::string fileContent;
    std::vector<std::tuple<vec3float>> vertices;
    std::vector<Triangle> triangles;
    std::vector<VectorTexture> textures;
    std::vector<VectorNormal> normals;

    std::tuple<vec3float> ParseVertices(const std::string& line);
    VectorNormal ParseNormals(const std::string& line);
    Triangle ParseFaces(const std::string& line);
    VectorTexture ParseTextures(const std::string& line);
};

#endif  // __WAVEFRONT_H__