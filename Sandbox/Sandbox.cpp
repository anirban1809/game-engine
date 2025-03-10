#include "../include/Core/Application.h"
#include "../include/Core/Shader.h"
#include "../include/Core/Logger.h"
#include "../include/Core/VertexContainer.h"
#include "../vendor/glew-2.2.0/include/GL/glew.h"
#include "../include/Loaders/Wavefront.h"
#include "Core/Types.h"
#include "../include/Loaders/ObjLoader.h"
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

    void OnInit() {
        obj = new WavefrontObjLoader();
        obj->LoadObjectFile(
            "/Users/anirban/Documents/Code/engine/Sandbox/models/box2.obj");
        obj->ParseObjectContent();

        ObjLoader *loader = new ObjLoader();
        loader->LoadObjectFile(
            "/Users/anirban/Documents/Code/engine/Sandbox/models/box2.obj");

        std::vector<float> vertices;
        for (const auto &[x, y, z, u, v] : obj->GetVerticesAndTextures()) {
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(u);
            vertices.push_back(v);
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
        container->Init(vertices, indices, shader->GetProgramId());
        container->Bind();

        camera.SetCameraProjection(45.0f, 1.0f, 0.1f, 1000.0f);
        camera.SetCameraPosition(0.0f, 1.0f, 1.0f);
        camera.SetCameraLook(0.0f, 0.0f, 0.0f);
        container->AttachCamera(&camera);
    }

    void OnKeyPressed(int key) {
        if (key == GLFW_KEY_UP) {
            camera.TranslateZ(-0.25f);
        }
        if (key == GLFW_KEY_DOWN) {
            camera.TranslateZ(0.25f);
        }

        if (key == GLFW_KEY_LEFT) {
            camera.TranslateX(-0.25f);
        }
        if (key == GLFW_KEY_RIGHT) {
            camera.TranslateX(0.25f);
        }
    }

    void OnRender() {
        container->Bind();
        container->Draw(shader->GetProgramId());
        container->Unbind();
    }
};