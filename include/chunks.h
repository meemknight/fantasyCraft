#pragma once
#include "block.h"
#include <vector>
#include <string.h>
#include <glm/vec3.hpp>
#include "renderer.h"
#include <set>
#include <FastNoiseSIMD.h>

constexpr int CHUNK_SIZE = 16;
constexpr int CHUNK_HEIGHT = 256;


class Chunk
{
public:


	Block *getBlock(int x, int y, int z)
	{
		return &blocks[x][z][y];
	}

	void clearBlockData()
	{
		memset(blocks, 0, sizeof(blocks));
	}

	void calculateFaces();

	glm::ivec2 &getChunkPosition()
	{
		return position;
	}

	glm::ivec2 &getChunkPositionx16()
	{
		return glm::ivec2(position) * glm::ivec2(16, 16);
	}


	friend class ChunksRenderer;
	friend class ChunkManager;


protected:
	
	void updateNeighbours();
	void removeReferenceToNeighbours(); //todo rename mabe

	std::vector<glm::ivec3> positions[6];
	std::vector<glm::ivec2> UVs[6];

	Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_HEIGHT];
	glm::ivec2 position;

	Chunk *chunkInFront = 0;
	Chunk *chunkInBack = 0;
	Chunk *chunkAtLeft = 0;
	Chunk *chunkAtRight = 0;

};


class ChunkManager
{
public:
	
	ChunkManager();

	void setGridSize(int size, glm::ivec2 playerPos);

	void setPlayerPos(glm::vec2 playerPos);

	friend class ChunksRenderer;


	//this defines the rect in which the chunk manager is loaded
	glm::ivec2 bottomCorner;
	glm::ivec2 topCorner;


	bool rayCast(glm::ivec3 &pos , glm::vec3 start, glm::vec3 direction, float maxRaySize = 10);

	Block *getBlockRaw(glm::ivec3 pos, Chunk **c = nullptr);

	//rename todo
	glm::ivec2 getPlayerInChunk(glm::vec2 playerPos);

private:

	glm::ivec2 computeBottomCorner(glm::vec2 playerPos, int size);
	glm::ivec2 computeTopCorner(glm::vec2 playerPos, int size);



	void setNeighbours(std::set<int> &newCreatedChunks, std::set<int> &chunksToRecalculate);

	//this is a grid size x size
	int gridSize;
	std::vector< Chunk * > loadedChunks;
	glm::ivec2 playerPos;

	FastNoiseSIMD *heightNoise = FastNoiseSIMD::NewFastNoiseSIMD();
	
	void generateChunk(Chunk &c);

};