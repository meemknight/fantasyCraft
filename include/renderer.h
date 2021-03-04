#pragma once
#include "shader.h"
#include <camera.h>
#include "texture.h"

//todo move in blocks
enum faces
{
	FRONT = 0,
	BACK,
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
};


class ChunksRenderer
{
public:

	ChunksRenderer()
	{
		init();
	}

	~ChunksRenderer()
	{
		//todo
	}

	void render(Camera c, glm::ivec3 pos);

private:

	void init();

	DrawBlocksShader shader;
	
	GLuint faceVAO[6];
	GLuint faceBuffer[6];
	GLuint faceTextureIndexesBuffer[6];
	
	GLuint faceIndexBuffer;
	Texture texture;

};