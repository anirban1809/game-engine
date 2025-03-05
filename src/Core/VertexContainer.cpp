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

void VertexContainer::Init(std::vector<float> vertexBuffer,
                           uint32 vertexBufferSize,
                           std::vector<uint32> indexBuffer,
                           uint32 indexBufferSize) {
    // Assume vertexBufferSize is the number of floats (each vertex has 2
    // floats) and indexBufferSize is the number of indices.
    vertices = vertexBuffer.data();
    indices = indexBuffer.data();

    vertexSize = vertexBufferSize;  // Total number of floats in vertex buffer.
    indexSize = indexBufferSize;    // Total number of indices.

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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO to prevent accidental modifications
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
    // Verify the new data has the same count as the original.
    if (size != vertexSize) {
        std::cerr << "Error: New vertex data size does not match existing size."
                  << std::endl;
        return;
    }

    // Update stored pointer (assuming external memory management)
    vertices = newVertices;

    // Update VBO on the GPU with the new vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize * sizeof(float), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexContainer::Bind() const { glBindVertexArray(VAO); }

void VertexContainer::Draw(unsigned int shaderProgramId, float* colors) const {
    // Optionally, set a uniform color:
    // GLint colorLoc = glGetUniformLocation(shaderProgramId, "color");
    // glUniform3f(colorLoc, colors[0], colors[1], colors[2]);

    // Draw using glDrawElements. The second parameter is the number of indices.
    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
}

void VertexContainer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
