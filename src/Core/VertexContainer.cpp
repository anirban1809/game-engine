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
#include <cstdio>
#include <iostream>
#include <vector>
#include "../../vendor/glm/glm.hpp"
#include "../../vendor/glm/gtc/matrix_transform.hpp"
#include "../../vendor/glm/gtc/type_ptr.hpp"
#include "../../include/Core/Camera.h"

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind the VAO to prevent accidental modifications
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);

    for (const auto& obj : objects) {
        textures.push_back(
            texture->LoadTexture(obj.GetMaterial().diffuseTextureFile));
    }

    // glBindTexture(
    //     GL_TEXTURE_2D,
    //     texturei,
    // glUniform1i(glGetUniformLocation(shaderProgramId, "texture1"), 0);
    // glEnable(GL_DEPTH_TEST);
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

void VertexContainer::ApplyTexture(uint32 shaderProgramId, uint32 textureId) {
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(glGetUniformLocation(shaderProgramId, "texture1"), 0);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(shaderProgramId);  // Activate Shader Program
    glUniform3f(glGetUniformLocation(shaderProgramId, "color"), 0.2f, 0.3f,
                0.3f);
    // Send matrices to the shader
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, "projection"), 1,
                       GL_FALSE, glm::value_ptr(camera->GetProjection()));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, "view"), 1,
                       GL_FALSE, glm::value_ptr(camera->GetView()));
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
