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
#include <tuple>
#include <vector>

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
            "/Users/anirban/Documents/Code/engine/Sandbox/models/box2.obj");
        obj->ParseContent();

        Transformers::Scale::Apply(obj, 0.25);

        std::vector<std::tuple<vec3float>> vertices = obj->GetVertices();

        float xmin = std::get<0>((vertices)[0]);
        float ymin = std::get<1>((vertices)[0]);
        float xmax = std::get<0>((vertices)[0]);
        float ymax = std::get<1>((vertices)[0]);
        // First, project the points
        for (const auto &[x, y, z] : vertices) {
            if (x < xmin) {
                xmin = x;
            }
            if (x > xmax) {
                xmax = x;
            }
            if (y < ymin) {
                ymin = y;
            }
            if (y > ymax) {
                ymax = y;
            }
        }

        for (const auto &[x, y, z] : vertices) {
            float normX = (2.0f * (x - xmin) / (xmax - xmin)) - 1.0f;
            float normY = (2.0f * (y - ymin) / (ymax - ymin)) - 1.0f;

            stage.push_back(normX);
            stage.push_back(normY);
        }

        std::vector<Triangle> triangles = obj->GetTriangles();

        for (int i = 0; i < triangles.size(); i++) {
            indices.push_back(std::get<0>((triangles)[i]).v - 1);
            indices.push_back(std::get<1>((triangles)[i]).v - 1);
            indices.push_back(std::get<2>((triangles)[i]).v - 1);
        }

        Logger::Log(LOG_INFO, "Initializing Application");
        Logger::Log(LOG_INFO, "Engine Version: 0.0.2 (Feb '25)");

        shader = new Shader("./src/Shaders/vertex_shader.glsl",
                            "./src/Shaders/fragment_shader.glsl");

        // Define Two Squares in a Single Vertex Buffer (2D Positions)

        // stage = {
        //     // First Square (Left)
        //     0.0f, 0.0f,  // Bottom-left
        //     0.3f, 0.0f,  // Bottom-right
        //     0.3f, 0.3f,  // Top-right
        //     0.0f, 0.3f,  // Top-left

        //     // Second Square (Right)
        //     0.3f, 0.3f,  // Bottom-left
        //     0.8f, 0.3f,  // Bottom-right
        //     0.8f, 0.8f,  // Top-right
        //     0.3f, 0.8f,  // Top-left
        // };

        // // Define One Index Buffer for Both Squares
        // indices = {// First Square
        //            0, 1, 2, 2, 3, 0,

        //            // Second Square
        //            4, 5, 6, 6, 7, 4};

        container = new VertexContainer();
        container->Init(stage.data(), stage.size() * sizeof(float),
                        indices.data(), indices.size() * sizeof(indices));
        container->Bind();
    }

    void OnRender() {
        // vertices[0] += 1.0f;
        container->UpdateVertexBuffer(stage.data(),
                                      stage.size() * sizeof(float));

        unsigned int shaderId = shader->GetProgramId();
        std::vector<float> colors = {0.0f, 1.0f, 0.6f};

        container->Bind();
        container->Draw(shaderId, colors.data());

        shader->Use();
        container->Unbind();
    }
};