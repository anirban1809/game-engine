#include "../../../include/Core/Types.h"
#include "../../../include/Core/Texture.h"
#include "../../vendor/glew-2.2.0/include/GL/glew.h"
#include <cstdio>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "../../vendor/stb/stb_image.h"

uint32 Texture::LoadTexture(const std::string& path) {
    printf("Loading texture\n");
    uint32 textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture wrapping & filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image
    int width, height, nrChannels;
    unsigned char* data =
        stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    stbi_image_free(data);
    return textureID;
}
