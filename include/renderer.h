#pragma once
#include "shader.h"
#include <camera.h>
#include "texture.h"
#include "block.h"
#include "skyBox.h"

enum faces
{
	FRONT = 0,
	BACK,
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
};

#include "chunks.h"

//todo move in blocks

class Chunk;
class ChunkManager;

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

	void render(Camera &c, Block b, glm::ivec3 pos);
	void render(Camera &c, Chunk &chunk);
	void render(Camera &c, ChunkManager &chunkManager, SkyBox &skyBox);

private:

	void init();

	DrawBlocksShader shader;
	
	GLuint faceVAO[6];
	GLuint faceBuffer[6];
	GLuint faceTextureIndexesBuffer[6];
	
	GLuint faceIndexBuffer;
	Texture texture;


	GLuint positionsbuffer[6];
	GLuint textureUVbuffer[6];


	std::vector<glm::ivec3> facesVector;
	std::vector<glm::vec2> uvVector;

	glm::ivec3 lastPosition = {};

};