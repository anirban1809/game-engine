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

void VertexContainer::Init(float* vertexBuffer, uint32 vertexBufferSize,
                           uint32* indexBuffer, uint32 indexBufferSize) {
    vertices = vertexBuffer;
    indices = indexBuffer;

    vertexSize = vertexBufferSize;
    indexSize = indexBufferSize;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // Step 2: Generate & Bind VBO (Vertex Buffer Object)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
    glCullFace(1);

    // Step 3: Generate & Bind EBO (Element Buffer Object)
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    // Step 5: Unbind VAO to prevent accidental modifications
    glBindVertexArray(0);
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

void VertexContainer::UpdateVertexBuffer(float* newVertices, uint32 size) {
    if (size != vertexSize) {
        std::cerr << "Error: New vertex data size does not match existing size."
                  << std::endl;
        return;
    }

    // Update stored vertices
    vertices = newVertices;

    // Update VBO on the GPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * @brief Binds the VAO and renders the object using indexed drawing.
 *
 * Uses `glDrawElements` to render the object based on the provided index
 * buffer.
 */

void SetColor(unsigned int colorLocation, const float* colors,
              unsigned int colorPointer) {
    glUniform3f(colorLocation, colors[colorPointer], colors[colorPointer + 1],
                colors[colorPointer + 2]);
}

void VertexContainer::Bind() const { glBindVertexArray(VAO); }

void VertexContainer::Draw(unsigned int shaderProgramId, float* colors) const {
    GLint colorLoc = glGetUniformLocation(shaderProgramId, "color");
    SetColor(colorLoc, colors, 0);
    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
}

/**
 * @brief Unbinds the VAO and VBO.
 *
 * This prevents unintended modifications to the currently active OpenGL state.
 */
void VertexContainer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
