#pragma once
#include "block.h"
#include <vector>
#include <string.h>
#include <glm/vec3.hpp>
#include "renderer.h"

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

	void createAChunkStructure();


	friend class ChunksRenderer;
	friend class ChunkManager;

protected:

	std::vector<glm::ivec3> positions[6];
	std::vector<glm::ivec2> UVs[6];

	Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_HEIGHT];
	glm::ivec2 position;

	
};


class ChunkManager
{
public:
	
	void setGridSize(int size, glm::ivec2 playerPos);

	void setPlayerPos(glm::vec2 playerPos);

	friend class ChunksRenderer;


	//this defines the rect in which the chunk manager is loaded
	glm::ivec2 bottomCorner;
	glm::ivec2 topCorner;

private:

	glm::ivec2 computeBottomCorner(glm::vec2 playerPos, int size);
	glm::ivec2 computeTopCorner(glm::vec2 playerPos, int size);

	glm::ivec2 getPlayerInChunk(glm::vec2 playerPos);

	int getChunkIndex(int x, int z);

	//this is a grid size x size
	int gridSize;
	std::vector< Chunk * > loadedChunks;
	glm::ivec2 playerPos;

};