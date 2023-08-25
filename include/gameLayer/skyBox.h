#pragma once
#include <glad/glad.h>	
#include <shader.h>
#include <glm/mat4x4.hpp>

class SkyBox
{
public:

	void create();
	void clear();

	void clearTexture();


	void loadTextures(const char *textures[]);
	void loadTexturesFromCrossTexture(const char* texture);


	void render(const glm::mat4 &viewProjection);

private:

	GLuint cubemapTexture = 0;
	Shader shader;
	GLint u_cubeMap = -1;
	GLint u_viewProjection = -1;

	GLuint vao = 0;
	GLuint cubeBuffer = 0;
};