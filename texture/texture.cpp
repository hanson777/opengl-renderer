#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

void Texture::Load(const std::string filename) {
    std::cout << "Loading texture file " << filename << std::endl;
    int width, height, ncomp;
    uint8_t* data = stbi_load(filename.c_str(), &width, &height, &ncomp, 0);
    if (!data) {
        std::cout << "[ERROR::TEXTURE] failed to load image" << std::endl;
        return;
    }

    GLenum format;
    if (ncomp == 1) format = GL_RED;
    else if (ncomp == 2) format = GL_RG;
    else if (ncomp == 3) format = GL_RGB;
    else if (ncomp == 4) format = GL_RGBA;
    else static_assert("[ERROR::TEXTURE] number of components must be in [1,4]");

    BindTexture(data, format, width, height, filename.find("diffuse") != std::string::npos);
    stbi_image_free(data);
}

void Texture::GenerateWhiteTexture() {
    std::cout << "Loading white texture\n";
    uint8_t data[] = { 255, 255, 255, 255 };
    int width = 1, height = 1;

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::BindTexture(uint8_t* data, GLenum format, int width, int height, bool srgb) {
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    std::cout << "Texture ID: " << m_id << std::endl;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum internalFormat;
    if (format == GL_RED) internalFormat = GL_R8;
    else if (format == GL_RG) internalFormat = GL_RG8;
    else if (format == GL_RGB) internalFormat = srgb ? GL_SRGB8 : GL_RGB8;
    else if (format == GL_RGBA) internalFormat = srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;
    else internalFormat = format;

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}
