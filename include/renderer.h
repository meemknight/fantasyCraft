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
	}

	//deprecated
	void render(Camera &c, Block b, glm::ivec3 pos);
	
	//deprecated
	void render(Camera &c, Chunk &chunk);

	void render(Camera &c, ChunkManager &chunkManager, SkyBox &skyBox);

	bool &getAo() { return ao; }

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
	GLuint aoBuffer[6];
	GLuint faceMask;


	std::vector<glm::ivec3> facesVector;
	std::vector<glm::vec2> uvVector;
	std::vector<uint8_t> aoVector;

	glm::ivec3 lastPosition = {};

	bool ao = true;
};