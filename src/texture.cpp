#include "texture.h"
#include "stb_image/stb_image.h"
#include <fstream>
#include <iostream>

void Texture::bind(int slot)
{
	glBindTexture(GL_TEXTURE_2D, id);
	glActiveTexture(GL_TEXTURE0 + slot);
}

void Texture::load(std::string path)
{
	std::ifstream f(path, std::ios::binary);

	if (!f.is_open())
	{
		std::cout << "err loading " << path << "\n";
		return;
	}

	//most vexing parse here yay love cpp
	std::string ret{ std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>() };


	stbi_set_flip_vertically_on_load(true);

	int width = 0;
	int height = 0;
	int channels = 0;

	const unsigned char *decodedImage = stbi_load_from_memory((unsigned char*)ret.c_str(), 
		ret.size(), &width, &height, &channels, 4);


	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glActiveTexture(GL_TEXTURE0);

	//minecraft is a pixel game so free optimizations, no bilinear filtey yay
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, decodedImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 4);
	



	this->id = id;

	stbi_image_free((void*)decodedImage);
}

void Texture::clear()
{
	glDeleteTextures(1, &id);
	id = 0;
}
