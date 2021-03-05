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

	void calculateFaces()
	{
		for(int i=0;i<6;i++)
		{
			positions[i].clear();
			UVs[i].clear();

			positions[i].reserve(200);
			UVs[i].reserve(200);

		}

		for (int x = 0; x < CHUNK_SIZE; x++)
			for (int z = 0; z < CHUNK_SIZE; z++)
				for (int y = 0; y < CHUNK_SIZE; y++)
				{
					
					auto &b = *getBlock(x, y, z);

					if(b.isAir())
					{
						continue;
					}

					//set faces
					//front
					if(z >= CHUNK_SIZE-1 || getBlock(x, y, z+1)->isTransparent())
					{
						positions[FRONT].emplace_back(x, y, z);
						UVs[FRONT].push_back(b.getPositionInAtlas(FRONT));
					}

					//back
					if (z <= 0 || getBlock(x, y, z - 1)->isTransparent())
					{
						positions[BACK].emplace_back(x, y, z);
						UVs[BACK].push_back(b.getPositionInAtlas(BACK));
					}

					//top
					if(y >= CHUNK_HEIGHT-1 || getBlock(x, y+1, z)->isTransparent())
					{
						positions[TOP].emplace_back(x, y, z);
						UVs[TOP].push_back(b.getPositionInAtlas(TOP));
					}
						
					//bottom
					if (y <= 0 || getBlock(x, y - 1, z)->isTransparent())
					{
						positions[BOTTOM].emplace_back(x, y, z);
						UVs[BOTTOM].push_back(b.getPositionInAtlas(BOTTOM));
					}
				
					//left
					if (x <= 0 || getBlock(x-1, y, z)->isTransparent())
					{
						positions[LEFT].emplace_back(x, y, z);
						UVs[LEFT].push_back(b.getPositionInAtlas(LEFT));
					}

					//right
					if (x >= CHUNK_SIZE-1 || getBlock(x + 1, y, z)->isTransparent())
					{
						positions[RIGHT].emplace_back(x, y, z);
						UVs[RIGHT].push_back(b.getPositionInAtlas(RIGHT));
					}
				}
			
	}

	glm::ivec2 &getChunkPosition()
	{
		return position;
	}

	glm::ivec2 &getChunkPositionx16()
	{
		return glm::ivec2(position) * glm::ivec2(16, 16);
	}

	friend class ChunksRenderer;

protected:

	std::vector<glm::ivec3> positions[6];
	std::vector<glm::ivec2> UVs[6];

	Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_HEIGHT];
	glm::ivec2 position;

	
};