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
            "testscene.mtl");
        loader->LoadObjectFile(
            "/Users/anirban/Documents/Code/engine/Sandbox/models/"
            "testscene.obj");

        std::vector<float> allvertices;
        std::vector<uint32> allIndices;

        for (auto const &object : loader->GetObjects()) {
            std::vector<float> vertices;
            for (const auto &[x, y, z, u, v, nx, ny, nz] :
                 object.GetAllVertexData()) {
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
                vertices.push_back(u);
                vertices.push_back(v);
                vertices.push_back(nx);
                vertices.push_back(ny);
                vertices.push_back(nz);
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

        light.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
        light.SetPosition(glm::vec3(0.0, 10.0, 0.0));

        container->AttachCamera(&camera);
        container->AttachLight(&light);
    }

    void OnKeyPressed(int key) {
        if (key == GLFW_KEY_UP) { camera.TranslateZ(-0.25f); }
        if (key == GLFW_KEY_DOWN) { camera.TranslateZ(0.25f); }
        if (key == GLFW_KEY_LEFT) { camera.TranslateX(-0.25f); }
        if (key == GLFW_KEY_RIGHT) { camera.TranslateX(0.25f); }

        if (key == GLFW_KEY_W) { light.UpdateLightPositionX(0.25f); }
        if (key == GLFW_KEY_S) { light.UpdateLightPositionX(-0.25f); }

        if (key == GLFW_KEY_A) { light.UpdateLightPositionZ(0.25f); }
        if (key == GLFW_KEY_D) { light.UpdateLightPositionZ(-0.25f); }

        if (key == GLFW_KEY_1) { light.UpdateLightPositionY(0.25f); }
        if (key == GLFW_KEY_2) { light.UpdateLightPositionY(-0.25f); }
    }

    void OnRender() {
        container->Bind();
        container->Draw(shader->GetProgramId());
        container->Unbind();
    }
};
