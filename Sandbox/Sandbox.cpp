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
#include "../vendor/glm/glm.hpp"

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
    float yCam = 0;

    void OnInit() {
        obj = new WavefrontObjLoader();
        obj->LoadFile(
            "/Users/anirban/Documents/Code/engine/Sandbox/models/box2.obj");
        obj->ParseContent();

        std::vector<glm::vec3> vertices;

        for (const auto &[x, y, z] : obj->GetVertices()) {
            vertices.emplace_back(x * 0.105, y * 0.105, z * 0.105);
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

        container = new VertexContainer();
        container->Init(vertices, vertices.size(), indices, indices.size());
        container->Bind();

        camera.SetCameraProjection(90.0f, 1.0f, 0.1f, 100.0f);
        camera.SetCameraPosition(0.7f, 2.0f, 1.0f);
        camera.SetCameraLook(0.0f, 0.0f, 0.0f);
        container->AttachCamera(&camera);

        /**
        glm::mat4 projection =
        glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
        glm::mat4 view =
        glm::lookAt(glm::vec3(0.7f, yCam, 1.0f),  // Camera position
        glm::vec3(0.0f, 0.0f, 0.0f),  // Look at origin
        glm::vec3(0.0f, 1.0f, 0.0f)   // Up vector
        );

        *
        */
    }

    void OnRender() {
        unsigned int shaderId = shader->GetProgramId();

        camera.TranslateY(0.025f);

        container->Bind();
        container->Draw(shaderId);
        shader->Use();
        container->Unbind();
    }
};