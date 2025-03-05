#include "../include/Core/Application.h"
#include "../include/Core/Shader.h"
#include "../include/Core/Logger.h"
#include "../include/Core/VertexContainer.h"
#include "../vendor/glew-2.2.0/include/GL/glew.h"
#include "../include/Loaders/Wavefront.h"
#include "Core/Types.h"
#include "Transformers/Scale.h"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <tuple>
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
        obj = new WavefrontObjLoader();
        obj->Load(
            "/Users/anirban/Documents/Code/game-engine/Sandbox/models/"
            "box2.obj");
        obj->ParseContent();

        std::vector<std::tuple<vec3float>> vertices = obj->GetVertices();

        for (const auto &[x, y, z] : vertices) {
            // float normX = (2.0f * (x - xmin) / (xmax - xmin)) - 1.0f;
            // float normY = (2.0f * (y - ymin) / (ymax - ymin)) - 1.0f;

            stage.push_back(x * 0.3);
            stage.push_back(y * 0.3);
        }

        std::vector<Triangle> triangles = obj->GetTriangles();

        for (int i = 0; i < triangles.size(); i++) {
            indices.push_back(std::get<0>((triangles)[i]).v);
            indices.push_back(std::get<1>((triangles)[i]).v);
            indices.push_back(std::get<2>((triangles)[i]).v);
        }

        Logger::Log(LOG_INFO, "Initializing Application");
        Logger::Log(LOG_INFO, "Engine Version: 0.0.2 (Feb '25)");

        shader = new Shader("./src/Shaders/vertex_shader.glsl",
                            "./src/Shaders/fragment_shader.glsl");

        printf("Stage Size: %lu\n", stage.size() / 2);
        printf("vertices size: %lu\n", vertices.size());

        // print size of index
        printf("Index Size: %lu\n", indices.size());

        container = new VertexContainer();
        container->Init(stage, stage.size(), indices, indices.size());
        container->Bind();
    }

    void OnRender() {
        // vertices[0] += 1.0f;
        // container->UpdateVertexBuffer(stage.data(),
        //                               stage.size() * sizeof(float));

        unsigned int shaderId = shader->GetProgramId();
        std::vector<float> colors = {0.0f, 1.0f, 0.6f};

        container->Bind();
        container->Draw(shaderId, colors.data());

        shader->Use();
        container->Unbind();
    }
};