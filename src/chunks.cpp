#include "chunks.h"

void Chunk::calculateFaces()
{
	for (int i = 0; i<6; i++)
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

				if (b.isAir())
				{
					continue;
				}

				//set faces
				//front
				if (z >= CHUNK_SIZE - 1 || getBlock(x, y, z + 1)->isTransparent())
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
				if (y >= CHUNK_HEIGHT - 1 || getBlock(x, y + 1, z)->isTransparent())
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
				if (x <= 0 || getBlock(x - 1, y, z)->isTransparent())
				{
					positions[LEFT].emplace_back(x, y, z);
					UVs[LEFT].push_back(b.getPositionInAtlas(LEFT));
				}

				//right
				if (x >= CHUNK_SIZE - 1 || getBlock(x + 1, y, z)->isTransparent())
				{
					positions[RIGHT].emplace_back(x, y, z);
					UVs[RIGHT].push_back(b.getPositionInAtlas(RIGHT));
				}
			}

}

void Chunk::createAChunkStructure()
{
	int xPadd = position.x * 16;
	int yPadd = position.y * 16;

	this->clearBlockData();
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			for (int h = 0; h < 16; h++)
			{
				if (h==0 ||
					h <= 4*sin( ((i + xPadd)+(j + yPadd)) /2.f )
					)
				{
					Block b(BLOCKS::grass);
					*this->getBlock(i, h, j) = b;
				}
				
			}


		}
	}

	this->calculateFaces();

}

void ChunkManager::setGridSize(int size, glm::ivec2 playerPos)
{
	loadedChunks.clear();//todo

	loadedChunks.reserve(size * size);


	glm::ivec2 playerInChunk = playerPos / 16;
	glm::ivec2 cornesOfChunkRegion = playerInChunk - (size / 2);

	for(int x=0;x<size; x++)
	{
		for(int z=0; z<size; z++)
		{
			Chunk *c = new Chunk;

			c->position = cornesOfChunkRegion + glm::ivec2{ x, z };
			c->createAChunkStructure();

			c->calculateFaces();

			loadedChunks.push_back(c);
		}

	}


	this->playerPos = playerPos;
}
