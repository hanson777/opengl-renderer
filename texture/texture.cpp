#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

void Texture::Load(const std::string filename) {
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

	BindTexture(data, format, width, height);
	stbi_image_free(data);

	m_width = width;
	m_height = height;
}

void Texture::BindTexture(uint8_t* data, GLenum format, int width, int height) {
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
}
