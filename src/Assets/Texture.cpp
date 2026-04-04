#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include <stb_image.h>
#include <iostream>
#include <glad/glad.h>
#include <string>

void Texture::Load(const std::string filename) {
    std::cout << "Loading texture file " << filename << std::endl;
    int ncomp;
    uint8_t* stbiData = stbi_load(filename.c_str(), &m_width, &m_height, &ncomp, 0);
    if (!stbiData) {
        std::cout << "[ERROR::TEXTURE] failed to load image" << std::endl;
        return;
    }

    m_data.assign(stbiData, stbiData + (m_width * m_height * ncomp));
    stbi_image_free(stbiData);

    if (ncomp == 1) m_format = GL_RED;
    else if (ncomp == 2) m_format = GL_RG;
    else if (ncomp == 3) m_format = GL_RGB;
    else if (ncomp == 4) m_format = GL_RGBA;
    else std::cout << ("[ERROR::TEXTURE] number of channels must be in [1,4]") << std::endl;

    bool srgb = filename.find("diffuse") != std::string::npos;

    if (m_format == GL_RED) m_internalFormat = GL_R8;
    else if (m_format == GL_RG) m_internalFormat = GL_RG8;
    else if (m_format == GL_RGB) m_internalFormat = srgb ? GL_SRGB8 : GL_RGB8;
    else if (m_format == GL_RGBA) m_internalFormat = srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;
    else m_internalFormat = m_format;
}

void Texture::GenerateWhiteTexture() {
    std::cout << "Loading white texture\n";
    m_data = { 255, 255, 255, 255 };
    m_width = 1;
    m_height = 1;
    m_format = GL_RGBA;
    m_internalFormat = GL_RGBA8;
}
