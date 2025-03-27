#include "Core/Application.h"
#include "Core/Events.h"
#include "Core/FrameBuffer.h"
#include "Core/VertexContainer.h"
#include "Core/Logger.h"
#include "UI/UIEngine.h"
#include "UI/ImGui/ImGuiLayer.h"

#include "UI/ImGui/Panels/ExamplePanel.h"
#include "UI/ImGui/Panels/FrameBufferPanel.h"

#include <filesystem>
#include <iostream>
#include <memory>

class Editor : public Application {
   public:
    Editor(int width, int height, const char *title)
        : Application(width, height, title),
          uiEngine(std::move(std::make_unique<ImGuiLayer>())) {}

   protected:
    VertexContainer *container;
    Shader *shader;
    std::vector<float> stage;
    std::vector<uint32> indices;
    bool value = false;
    bool leftMouseDown = false;
    double lastX = 0.0f;
    double lastY = 0.0f;
    bool firstMouse = false;
    float yaw = -90.0f;  // Initialized so that the initial front is along -Z.
    float pitch = 0.0f;
    float mouseSensitivity = 0.1f;
    float movementSpeed = 1.0f;
    UIEngine uiEngine;
    ImGuiLayer *imGuiBackend;
    FrameBuffer *scenebuffer;

    void OnInit() {
        scenebuffer = new FrameBuffer(1920.0f, 1080.0f);
        uiEngine.Init(window->GetGLFWWindow());

        uiEngine.GetUIManager().AddPanel(CreatePanel<ExamplePanel>("Example1"));
        uiEngine.GetUIManager().AddPanel(CreatePanel<ExamplePanel>("Example2"));

        uiEngine.GetUIManager().AddPanel(CreatePanel<FramebufferPanel>(
            "Scene", scenebuffer->GetFrameTexture(),
            glm::vec2(1920.0f, 1080.0f)));

        ObjLoader *loader = new ObjLoader();

        loader->LoadObjectFile(
            "/Users/anirban/Documents/Code/engine/editor/models/testscene.obj");
        loader->LoadMaterialFile(
            "/Users/anirban/Documents/Code/engine/editor/models/testscene.mtl");

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

        std::cout << std::filesystem::current_path() << std::endl;

        shader = new Shader(
            "/Users/anirban/Documents/Code/engine/editor/Shaders/"
            "vertex_shader.glsl",
            "/Users/anirban/Documents/Code/engine/editor/Shaders/"
            "fragment_shader.glsl");

        container = new VertexContainer(shader);

        container->AddObjects(loader->GetObjects());

        container->Init(allvertices, allIndices, shader->GetProgramId());
        container->Bind();

        camera.SetCameraProjection(45.0f, 1.0f, 0.1f, 1000.0f);
        camera.SetCameraPosition(0.0f, 100.0f, 100.0f);
        camera.SetCameraLook(0.0f, 0.0f, 0.0f);

        light.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
        light.SetPosition(glm::vec3(0.0, 100.0, 0.0));

        container->AttachCamera(&camera);
        container->AttachLight(&light);
    }

    void OnKeyPressed(int key) {
        // Retrieve current camera position and look target.
        glm::vec3 position = camera.GetCameraPosition();
        glm::vec3 look = camera.GetCameraLook();

        // Calculate forward vector (direction the camera is facing)
        glm::vec3 forward = glm::normalize(look - position);
        // Compute right vector from forward and world up vector.
        glm::vec3 right =
            glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

        if (key == GLFW_KEY_UP) { camera.TranslateZ(-0.25f); }
        if (key == GLFW_KEY_DOWN) { camera.TranslateZ(0.25f); }
        if (key == GLFW_KEY_LEFT) { camera.TranslateX(-0.25f); }
        if (key == GLFW_KEY_RIGHT) { camera.TranslateX(0.25f); }

        if (key == KeyEvent::KEY_W) {
            std::cout << "Moving Forward" << std::endl;
            glm::vec3 delta = forward * movementSpeed;
            camera.SetCameraPosition(position.x + delta.x, position.y + delta.y,
                                     position.z + delta.z);
            camera.SetCameraLook(look.x + delta.x, look.y + delta.y,
                                 look.z + delta.z);
        }
        if (key == KeyEvent::KEY_S) {
            glm::vec3 delta = forward * movementSpeed;
            camera.SetCameraPosition(position.x - delta.x, position.y - delta.y,
                                     position.z - delta.z);
            camera.SetCameraLook(look.x - delta.x, look.y - delta.y,
                                 look.z - delta.z);
        }

        if (key == GLFW_KEY_A) {
            glm::vec3 delta = right * movementSpeed;
            camera.SetCameraPosition(position.x - delta.x, position.y - delta.y,
                                     position.z - delta.z);
            camera.SetCameraLook(look.x - delta.x, look.y - delta.y,
                                 look.z - delta.z);
        }

        if (key == GLFW_KEY_D) {
            glm::vec3 delta = right * movementSpeed;
            camera.SetCameraPosition(position.x + delta.x, position.y + delta.y,
                                     position.z + delta.z);
            camera.SetCameraLook(look.x + delta.x, look.y + delta.y,
                                 look.z + delta.z);
        }
    }

    void OnMousePressed(int button) {
        if (button == MouseEvent::MOUSE_BUTTON_LEFT) {
            std::cout << "Left mouse button clicked!" << std::endl;
            leftMouseDown = true;
            firstMouse = true;
        }
    }

    void OnMouseReleased(int button) {
        if (button == MouseEvent::MOUSE_BUTTON_LEFT) {
            std::cout << "Left mouse button released!" << std::endl;
            leftMouseDown = false;
        }
    }

    void OnMouseMoved(double xpos, double ypos) {
        if (!leftMouseDown) { return; }

        if (firstMouse) {
            // On the first mouse event, initialize the last positions.
            lastX = (float)xpos;
            lastY = (float)ypos;
            firstMouse = false;
            return;
        }

        // Calculate the offset from the last frame.
        float xoffset = -(float)xpos + lastX;
        float yoffset =
            -lastY + (float)ypos;  // reversed: moving up is positive

        lastX = (float)xpos;
        lastY = (float)ypos;

        // Scale offsets by sensitivity.
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        // Update yaw and pitch.
        yaw += xoffset;
        pitch += yoffset;

        // Constrain pitch to prevent flipping.
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        // Compute the new front direction.
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);

        // Update the camera look target.
        // (Camera look = camera position + front vector)
        glm::vec3 newLook = camera.GetCameraPosition() + front;
        camera.SetCameraLook(newLook.x, newLook.y, newLook.z);
    }

    void OnUpdate() {}

    void OnRender() {
        uiEngine.BeginFrame();
        uiEngine.RenderPanels();
        uiEngine.EndFrame();

        scenebuffer->Bind();
        container->Bind();
        container->Draw(shader->GetProgramId());
        container->Unbind();
        scenebuffer->Unbind();
    }
};
