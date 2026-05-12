#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include <stb_image.h>
#include <iostream>
#include <glad/glad.h>
#include <string>
#include <vector>

void Texture::Load(const std::string& filename) {
    std::cout << "Loading texture file " << filename << '\n';
    int ncomp;
    uint8_t* stbiData = stbi_load(filename.c_str(), &width, &height, &ncomp, 0);
    if (!stbiData) {
        std::cout << "[ERROR::TEXTURE] failed to load " << filename << std::endl;
        return;
    }

    data.assign(stbiData, stbiData + (width * height * ncomp));
    stbi_image_free(stbiData);

    if (ncomp == 1) format = GL_RED;
    else if (ncomp == 2) format = GL_RG;
    else if (ncomp == 3) format = GL_RGB;
    else if (ncomp == 4) format = GL_RGBA;
    else std::cout << ("[ERROR::TEXTURE] number of channels must be in [1,4]") << std::endl;

    bool srgb = filename.find("diffuse") != std::string::npos;

    if (format == GL_RED) internalFormat = GL_R8;
    else if (format == GL_RG) internalFormat = GL_RG8;
    else if (format == GL_RGB) internalFormat = srgb ? GL_SRGB8 : GL_RGB8;
    else if (format == GL_RGBA) internalFormat = srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;
    else internalFormat = format;
}

void Texture::GenerateWhiteTexture() {
    data = { 255, 255, 255, 255 };
    width = 1;
    height = 1;
    format = GL_RGBA;
    internalFormat = GL_RGBA8;
}
