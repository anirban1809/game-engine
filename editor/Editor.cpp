#include "Editor.h"

#include "Core/Types.h"
#include "UI/ImGui/Panels/ScenePropsPanel.h"
#include "UI/ImGui/Panels/NodePropsPanel.h"
#include "UI/ImGui/Panels/FrameBufferPanel.h"
#include "UI/ImGui/Panels/AssetLibraryPanel.h"
#include "UI/ImGui/Panels/LoadObjectPanel.h"
#include "importer.h"

#include <assimp/mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Editor::Editor(int width, int height, const char* title)
    : Application(width, height, title),
      state(fs, assets),
      uiEngine(std::make_unique<ImGuiLayer>(state)) {}

void Editor::DefineUI() {
    std::shared_ptr<ImGuiLayoutContainer> lc =
        CreateLayoutContainer<ImGuiLayoutContainer>(1, 14);

    lc->SetGap(10.0f);

    std::shared_ptr<ImGuiLayoutContainer> lc1 =
        CreateLayoutContainer<ImGuiLayoutContainer>(2, 1);
    std::shared_ptr<ImGuiLayoutContainer> lc2 =
        CreateLayoutContainer<ImGuiLayoutContainer>(1, 1);
    std::shared_ptr<ImGuiLayoutContainer> lc3 =
        CreateLayoutContainer<ImGuiLayoutContainer>(1, 1);

    lc1->AddElement(CreatePanel<ScenePropsPanel>(state), 1);
    lc1->AddElement(CreatePanel<AssetLibraryPanel>(state), 1);

    lc2->AddElement(CreatePanel<FramebufferPanel>("Scene", state, scenebuffer),
                    1);
    lc3->AddElement(CreatePanel<NodePropsPanel>(state), 1);

    lc->AddElement(lc1, 3);
    lc->AddElement(lc2, 9);
    lc->AddElement(lc3, 2);

    uiEngine.GetUIManager().AddLayoutContainer(lc);
    uiEngine.GetUIManager().AddPanel(CreatePanel<LoadObjectPanel>(state));
}

void Editor::OnInit() {
    scenebuffer = new FrameBuffer(1000.0f, 1000.0f);
    uiEngine.Init(window->GetGLFWWindow());
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    if (!primary) {
        std::cerr << "Failed to get primary monitor!" << std::endl;
        return;
    }

    // Step 2: Get monitor work area
    int x, y, width, height;
    glfwGetMonitorWorkarea(primary, &x, &y, &width, &height);

    // Step 3: Print it
    std::cout << "Usable screen area: " << width << "x" << height
              << " at position (" << x << ", " << y << ")" << std::endl;

    DefineUI();
    state.fs = fs;
    ObjLoader* loader = new ObjLoader();

    // loader->LoadObjectFile(
    //     "/Users/anirban/Documents/Code/engine/editor/models/"
    //     "testscene.obj");
    // loader->LoadMaterialFile(
    //     "/Users/anirban/Documents/Code/engine/editor/models/"
    //     "testscene.mtl");

    std::vector<float> vertexArray;
    std::vector<uint32> indexArray;

    if (Importer::Load("/Users/anirban/Documents/Code/engine/editor/models/"
                       "Chair.obj",
                       vertexArray, indexArray)) {
        std::cout << "Loaded mesh!" << std::endl;
        std::cout << "Unique vertices: " << vertexArray.size() / 8 << std::endl;
        std::cout << "Indices: " << indexArray.size() << std::endl;
    } else {
        std::cerr << "Failed to load mesh." << std::endl;
    }

    // for (auto const& object : loader->GetObjects()) {
    //     std::vector<float> vertices;
    //     for (const auto& [x, y, z, u, v, nx, ny, nz] :
    //          object.GetAllVertexData()) {
    //         vertices.push_back(x);
    //         vertices.push_back(y);
    //         vertices.push_back(z);
    //         vertices.push_back(u);
    //         vertices.push_back(v);
    //         vertices.push_back(nx);
    //         vertices.push_back(ny);
    //         vertices.push_back(nz);
    //     }

    //     allvertices.insert(allvertices.end(), vertices.begin(),
    //     vertices.end());

    //     std::vector<uint32> localIndices = object.GetVertexIndices();

    //     for (int i = 0; i < object.GetVertexIndices().size(); i++) {
    //         localIndices[i] = localIndices[i] + object.vertexIndexOffset;
    //     }

    //     allIndices.insert(allIndices.end(), localIndices.begin(),
    //                       localIndices.end());
    // }

    // std::vector<float> planeVertices = {// positions
    //                                     -1.0f, 0.0f, -1.0f, 1.0f,  0.0f,
    //                                     -1.0f, 1.0f,  0.0f, 1.0f,  -1.0f,
    //                                     0.0f, 1.0f};

    // std::vector<unsigned int> planeIndices = {0, 1, 2, 2, 3, 0};

    Logger::Log(LOG_INFO, "Initializing Application");
    Logger::Log(LOG_INFO, "Engine Version: 0.0.2 (Feb '25)");

    std::cout << std::filesystem::current_path() << std::endl;

    shader = new Shader(
        "/Users/anirban/Documents/Code/engine/editor/Shaders/"
        "vertex_shader.glsl",
        "/Users/anirban/Documents/Code/engine/editor/Shaders/"
        "fragment_shader.glsl");

    Shader* gridShader = new Shader(
        "/Users/anirban/Documents/Code/engine/editor/Shaders/grid/"
        "grid_vertex_shader.glsl",
        "/Users/anirban/Documents/Code/engine/editor/Shaders/grid/"
        "grid_fragment_shader.glsl");

    container = new VertexContainer(shader, gridShader);

    // container->AddObjects(loader->GetObjects());

    std::vector<float> planeVertices = {// positions
                                        -1.0f, 0.0f, -1.0f, 1.0f,  0.0f, -1.0f,
                                        1.0f,  0.0f, 1.0f,  -1.0f, 0.0f, 1.0f};

    std::vector<unsigned int> planeIndices = {0, 1, 2, 2, 3, 0};

    container->Init(vertexArray, indexArray, shader->GetProgramId());
    container->InitGrid(planeVertices, planeIndices);
    container->Bind();

    camera.SetCameraProjection(45.0f, 1.0f, 0.1f, 1000.0f);
    camera.SetCameraPosition(0.0f, 100.0f, 100.0f);
    camera.SetCameraLook(0.0f, 0.0f, 0.0f);

    light.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
    light.SetPosition(glm::vec3(0.0, 15.0, 0.0));

    container->AttachCamera(&camera);
    container->AttachLight(&light);
}

void Editor::OnKeyPressed(int key) {
    // Retrieve current camera position and look target.
    glm::vec3 position = camera.GetCameraPosition();
    glm::vec3 look = camera.GetCameraLook();

    // Calculate forward vector (direction the camera is facing)
    glm::vec3 forward = glm::normalize(look - position);
    // Compute right vector from forward and world up vector.
    glm::vec3 right =
        glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

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

    if (key == KeyEvent::KEY_A) {
        glm::vec3 delta = right * movementSpeed;
        camera.SetCameraPosition(position.x - delta.x, position.y - delta.y,
                                 position.z - delta.z);
        camera.SetCameraLook(look.x - delta.x, look.y - delta.y,
                             look.z - delta.z);
    }

    if (key == KeyEvent::KEY_D) {
        glm::vec3 delta = right * movementSpeed;
        camera.SetCameraPosition(position.x + delta.x, position.y + delta.y,
                                 position.z + delta.z);
        camera.SetCameraLook(look.x + delta.x, look.y + delta.y,
                             look.z + delta.z);
    }
}

void Editor::OnMousePressed(int button) {
    if (!state.isFrameBufferPanelHovered) { return; }
    if (button == MouseEvent::MOUSE_BUTTON_LEFT) {
        std::cout << "Left mouse button clicked!" << std::endl;
        leftMouseDown = true;
        firstMouse = true;
    }
}

void Editor::OnMouseReleased(int button) {
    if (!state.isFrameBufferPanelHovered) { return; }
    if (button == MouseEvent::MOUSE_BUTTON_LEFT) {
        std::cout << "Left mouse button released!" << std::endl;
        leftMouseDown = false;
    }
}

void Editor::OnMouseMoved(double xpos, double ypos) {
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
    float yoffset = -lastY + (float)ypos;  // reversed: moving up is positive

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

void Editor::OnUpdate() {}

void Editor::OnRender() {
    scenebuffer->Bind();
    scenebuffer->Clear();
    container->Bind();
    container->Draw(shader->GetProgramId());
    container->Unbind();

    scenebuffer->Unbind();

    uiEngine.BeginFrame();
    uiEngine.RenderPanels();
    uiEngine.EndFrame();
}
