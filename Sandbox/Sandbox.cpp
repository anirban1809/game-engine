#include "../include/Core/Application.h"
#include "../include/Core/Shader.h"
#include "../include/Core/Logger.h"
#include "../include/Core/VertexContainer.h"
#include "../vendor/glew-2.2.0/include/GL/glew.h"
#include "../include/Loaders/Wavefront.h"
#include "Core/Types.h"
#include "../include/Loaders/ObjLoader.h"
#include <string>
#include <vector>
#include <unistd.h>

class Sandbox : public Application {
   public:
    Sandbox(int width, int height, const char *title)
        : Application(width, height, title) {}

   protected:
    VertexContainer *container;
    Shader *shader;
    WavefrontObjLoader *obj;
    std::vector<float> stage;
    std::vector<uint32> indices;

    void OnInit() {
        ObjLoader *loader = new ObjLoader();
        loader->LoadMaterialFile(
            "/Users/anirban/Documents/Code/engine/Sandbox/models/"
            "threecubes.mtl");
        loader->LoadObjectFile(
            "/Users/anirban/Documents/Code/engine/Sandbox/models/"
            "threecubes.obj");

        std::vector<float> allvertices;
        std::vector<uint32> allIndices;

        for (auto const &object : loader->GetObjects()) {
            std::vector<float> vertices;
            for (const auto &[x, y, z, u, v] :
                 object.GetVerticesAndTextures()) {
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
                vertices.push_back(u);
                vertices.push_back(v);
            }

            allvertices.insert(allvertices.end(), vertices.begin(),
                               vertices.end());

            std::vector<uint32> localIndices = object.GetVertexIndices();

            for (int i = 0; i < object.GetVertexIndices().size(); i++) {
                localIndices[i] = localIndices[i] + object.vertexIndexOffset;
            }

            allIndices.insert(allIndices.end(), localIndices.begin(),
                              localIndices.end());
        }

        Logger::Log(LOG_INFO, "Initializing Application");
        Logger::Log(LOG_INFO, "Engine Version: 0.0.2 (Feb '25)");

        shader = new Shader("./src/Shaders/vertex_shader.glsl",
                            "./src/Shaders/fragment_shader.glsl");

        container = new VertexContainer();

        container->AddObjects(loader->GetObjects());

        container->Init(allvertices, allIndices, shader->GetProgramId());
        container->Bind();

        camera.SetCameraProjection(45.0f, 1.0f, 0.1f, 1000.0f);
        camera.SetCameraPosition(0.0f, 1.0f, 1.0f);
        camera.SetCameraLook(0.0f, 0.0f, 0.0f);
        container->AttachCamera(&camera);
    }

    void OnKeyPressed(int key) {
        if (key == GLFW_KEY_UP) { camera.TranslateZ(-0.25f); }
        if (key == GLFW_KEY_DOWN) { camera.TranslateZ(0.25f); }
        if (key == GLFW_KEY_LEFT) { camera.TranslateX(-0.25f); }
        if (key == GLFW_KEY_RIGHT) { camera.TranslateX(0.25f); }
    }

    void OnRender() {
        container->Bind();
        container->Draw(shader->GetProgramId());
        container->Unbind();
    }
};
