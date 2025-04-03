/**
 * @file VertexContainer.cpp
 * @brief Implementation of the VertexContainer class for managing vertex and
 * index buffers in OpenGL.
 *
 * This class abstracts the creation, management, and rendering of OpenGL Vertex
 * Array Objects (VAO), Vertex Buffer Objects (VBO), and Element Buffer Objects
 * (EBO). It facilitates indexed rendering by storing vertex and index data
 * efficiently.
 */
#include "../../include/Core/VertexContainer.h"
#include "../../include/Core/Types.h"
#include "../../vendor/glew-2.2.0/include/GL/glew.h"
#include <vector>
#include "../../vendor/glm/glm.hpp"
#include "../../vendor/glm/gtc/matrix_transform.hpp"
#include "../../vendor/glm/gtc/type_ptr.hpp"
#include "../../include/Core/Camera.h"
#include "../../include/Core/Light.h"

VertexContainer::VertexContainer(Shader* _shader) { shader = _shader; }

void VertexContainer::Init(std::vector<float>& vertexBuffer,
                           std::vector<uint32>& indexBuffer,
                           uint32 shaderProgramId) {
    vertices = vertexBuffer.data();
    indices = indexBuffer.data();

    vertexSize = vertexBuffer.size();
    indexSize = indexBuffer.size();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind VBO (Vertex Buffer Object)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Upload vertex data: vertexSize floats * size of float (in bytes)
    glBufferData(GL_ARRAY_BUFFER, vertexSize * sizeof(float), vertices,
                 GL_STATIC_DRAW);

    // Generate and bind EBO (Element Buffer Object)
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Upload index data: indexSize indices * size of uint32 (in bytes)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(uint32), indices,
                 GL_STATIC_DRAW);

    // Setup the vertex attribute pointer:
    // We assume each vertex is (x,y) -> 2 floats per vertex.

    // Attribute 0: Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    // Attribute 1: Texture Coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Attribute 2: Normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind the VAO to prevent accidental modifications
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);

    // Enable face culling
    glEnable(GL_CULL_FACE);
    // Cull back faces
    glCullFace(GL_BACK);
    // Specify front face winding order (optional, depending on your vertex
    // order)
    glFrontFace(GL_CCW);

    for (const auto& obj : objects) {
        textures.push_back(
            texture->LoadTexture(obj.GetMaterial().diffuseTextureFile));
    }
}

/**
 * @brief Destructor for the VertexContainer class.
 *
 * Cleans up allocated OpenGL resources including VAO, VBO, and EBO.
 */
VertexContainer::~VertexContainer() {
    glDeleteBuffers(1, &VBO);       // Delete Vertex Buffer
    glDeleteBuffers(1, &EBO);       // Delete Index Buffer
    glDeleteVertexArrays(1, &VAO);  // Delete Vertex Array
}

void VertexContainer::UpdateVertexBuffer(const std::vector<float>& newVertices,
                                         uint32 size) {
    // Update stored pointer (assuming external memory management)
    vertices = newVertices.data();

    // Update VBO on the GPU with the new vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize * sizeof(float), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexContainer::Bind() const { glBindVertexArray(VAO); }

void VertexContainer::AttachCamera(Camera* cam) { camera = cam; }
void VertexContainer::AttachLight(Light* l) { light = l; }

void VertexContainer::ApplyTexture(uint32 shaderProgramId, uint32 textureId) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    int32 location = shader->GetUniformLocation(shaderProgramId, "texture1");
    shader->SetShaderUniformInt(location, 0);

    glEnable(GL_DEPTH_TEST);

    shader->Use();
    shader->SetShaderUniform3Float(
        shader->GetUniformLocation(shaderProgramId, "color"), 0.2f, 0.3f, 0.3f);

    glm::mat4 model = glm::mat4(1.0f);

    int32 modelUniform = shader->GetUniformLocation(shaderProgramId, "model");
    int32 projectionUniform =
        shader->GetUniformLocation(shaderProgramId, "projection");
    int32 viewUniform = shader->GetUniformLocation(shaderProgramId, "view");
    int32 viewPosUniform =
        shader->GetUniformLocation(shaderProgramId, "viewPos");

    shader->SetShaderUniformMat4Float(modelUniform, model);
    shader->SetShaderUniformMat4Float(viewUniform, camera->GetView());
    shader->SetShaderUniformMat4Float(projectionUniform,
                                      camera->GetProjection());

    shader->SetShaderUniformVec3Float(viewPosUniform,
                                      camera->GetCameraPosition());

    int32 lightPosUniform =
        shader->GetUniformLocation(shaderProgramId, "lightPos");
    int32 lightColorUniform =
        shader->GetUniformLocation(shaderProgramId, "lightColor");

    shader->SetShaderUniformVec3Float(lightPosUniform, light->GetPosition());
    shader->SetShaderUniformVec3Float(lightColorUniform, light->GetColor());

    glm::vec3 materialDiffuse(1.0f, 1.0f, 1.0f);

    shader->SetShaderUniformVec3Float(
        shader->GetUniformLocation(shaderProgramId, "diffuseColor"),
        materialDiffuse);
}

void VertexContainer::AddObjects(const std::vector<Object>& _objects) {
    objects = _objects;
}

void VertexContainer::Draw(uint32 shaderProgramId) {
    uint32 offset = 0;
    for (int i = 0; i < objects.size(); i++) {
        ApplyTexture(shaderProgramId, textures[i]);
        glDrawElements(GL_TRIANGLES, objects[i].GetVertexIndices().size(),
                       GL_UNSIGNED_INT, (void*)(offset * sizeof(uint32)));

        offset += objects[i].GetVertexIndices().size();
    }
}

void VertexContainer::Unbind() const {
    delete texture;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
