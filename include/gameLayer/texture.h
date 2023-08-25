#pragma once
#include <string>
#include <glad/glad.h>

class Texture
{
public:
	Texture() = default;
	Texture(std::string path) { load(path); };

	void bind(int slot);
	void load(std::string path);
	void clear();

private:

	GLuint id = 0;

};