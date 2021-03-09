#include "chunks.h"
#include <algorithm>
#include <stack>
#include <set>


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
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{

				auto &b = *getBlock(x, y, z);

				if (b.isAir())
				{
					continue;
				}

				//set faces
				//front
				if(z >= CHUNK_SIZE - 1)
				{
					//if there is a chunk
					if(chunkInFront)
					{
						if(chunkInFront->getBlock(x, y, 0)->isTransparent())
						{
							positions[FRONT].emplace_back(x, y, z);
							UVs[FRONT].push_back(b.getPositionInAtlas(FRONT));
						}
					}
					
				}else
				if (getBlock(x, y, z + 1)->isTransparent())
				{
					positions[FRONT].emplace_back(x, y, z);
					UVs[FRONT].push_back(b.getPositionInAtlas(FRONT));
				}

				//back
				if(z <= 0)
				{
					if(chunkInBack)
					{
						if (chunkInBack->getBlock(x, y, CHUNK_SIZE-1)->isTransparent())
						{
							positions[BACK].emplace_back(x, y, z);
							UVs[BACK].push_back(b.getPositionInAtlas(BACK));
						}
					}

				}else
				if (getBlock(x, y, z - 1)->isTransparent())
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
				if(x <= 0)
				{
					if(chunkAtLeft)
					{
						if(chunkAtLeft->getBlock(CHUNK_SIZE-1, y, z)->isTransparent())
						{
							positions[LEFT].emplace_back(x, y, z);
							UVs[LEFT].push_back(b.getPositionInAtlas(LEFT));
						}
					}

				}else
				if (getBlock(x - 1, y, z)->isTransparent())
				{
					positions[LEFT].emplace_back(x, y, z);
					UVs[LEFT].push_back(b.getPositionInAtlas(LEFT));
				}

				//right
				if (x >= CHUNK_SIZE - 1)
				{
					if(chunkAtRight)
					{
						if(chunkAtRight->getBlock(0, y, z)->isTransparent())
						{
							positions[RIGHT].emplace_back(x, y, z);
							UVs[RIGHT].push_back(b.getPositionInAtlas(RIGHT));
						}
					}

				}else
				if (getBlock(x + 1, y, z)->isTransparent())
				{
					positions[RIGHT].emplace_back(x, y, z);
					UVs[RIGHT].push_back(b.getPositionInAtlas(RIGHT));
				}
			}

}


//todo move
static FastNoiseSIMD *heightNoise = FastNoiseSIMD::NewFastNoiseSIMD();

void Chunk::updateNeighbours()
{
	if(chunkInFront)
	{
		chunkInFront->chunkInBack = this;
	}

	if(chunkInBack)
	{
		chunkInBack->chunkInFront = this;
	}

	if(chunkAtLeft)
	{
		chunkAtLeft->chunkAtRight = this;
	}

	if(chunkAtRight)
	{
		chunkAtRight->chunkAtLeft = this;
	}

}

void Chunk::removeReferenceToNeighbours()
{
	if (chunkInFront)
	{
		chunkInFront->chunkInBack = nullptr;
		chunkInFront = nullptr;
	}

	if (chunkInBack)
	{
		chunkInBack->chunkInFront = nullptr;
		chunkInBack = nullptr;
	}

	if (chunkAtLeft)
	{
		chunkAtLeft->chunkAtRight = nullptr;
		chunkAtLeft = nullptr;
	}

	if (chunkAtRight)
	{
		chunkAtRight->chunkAtLeft = nullptr;
		chunkAtRight = nullptr;
	}


}

ChunkManager::ChunkManager()
{
	heightNoise->SetNoiseType(FastNoiseSIMD::NoiseType::Perlin);
	{
		float scale = 1;
		heightNoise->SetAxisScales(scale, 1, scale);
		heightNoise->SetFrequency(0.020);
		heightNoise->SetFractalOctaves(3);
		heightNoise->SetPerturbFractalOctaves(3);

	}

}

void ChunkManager::setGridSize(int size, glm::ivec2 playerPos)
{
	gridSize = size;
	this->playerPos = playerPos;

	loadedChunks.clear();//todo

	loadedChunks.reserve(size * size);

	bottomCorner = computeBottomCorner(playerPos, size);
	topCorner = computeTopCorner(playerPos, size);

	std::set<int> newCreatedChunks;

	for(int x=0;x<size; x++)
	{
		for(int z=0; z<size; z++)
		{
			Chunk *c = new Chunk;

			c->position = bottomCorner + glm::ivec2{ x, z };

			generateChunk(*c);

			loadedChunks.push_back(c);

			newCreatedChunks.insert(loadedChunks.size()-1);
		}

	}

	std::set<int> chunksToRecalculate;
	setNeighbours(newCreatedChunks, chunksToRecalculate);

	for(auto id : newCreatedChunks)
	{
		loadedChunks[id]->calculateFaces();
	}

	
}

void ChunkManager::setPlayerPos(glm::vec2 playerPos)
{
	auto newBottomCorner = computeBottomCorner(playerPos, gridSize);
	auto newTopCorner = computeTopCorner(playerPos, gridSize);

	//how should the chunk structure be moved
	auto moveDelta = newBottomCorner - bottomCorner;

	auto playerInChunk = getPlayerInChunk(playerPos);
	//std::cout << playerInChunk.x << " " << playerInChunk.y << "\n";
	//std::cout << moveDelta.x << " " << moveDelta.y << "\n";

	if(moveDelta.x ==0 && moveDelta.y == 0)
	{
		//do nothing

	}else
	{
		//std::cout << "\n\nrecreate:\n";
		std::vector<int> unusedChunks;
		unusedChunks.reserve(gridSize * gridSize);

		for(int i=0; i< loadedChunks.size(); i++)
		{
			glm::vec2 pos = loadedChunks[i]->getChunkPosition();

			if(
				pos .x < newBottomCorner.x || pos.x >= newTopCorner.x
				||
				pos.y < newBottomCorner.y || pos.y >= newTopCorner.y
				)
			{
				unusedChunks.push_back(i);
				//std::cout << pos.x << " " << pos.y << "\n";

				//clear data
				loadedChunks[i]->removeReferenceToNeighbours();

			}
		}
	
		std::set<int> newCreatedChunks;
		//newCreatedChunks.reserve(unusedChunks.size());

		for (int x = 0; x < gridSize; x++)
		{
			for (int z = 0; z < gridSize; z++)
			{
				glm::ivec2 pos = newBottomCorner + glm::ivec2{ x, z };
				
				auto it = std::find_if(loadedChunks.begin(), loadedChunks.end(), 
					[pos](Chunk *c){return c->getChunkPosition() == pos;}
				);

				if(it == loadedChunks.end())
				{
					int id = unusedChunks[unusedChunks.size() - 1];
					unusedChunks.pop_back();

					newCreatedChunks.insert(id);

					loadedChunks[id]->position = newBottomCorner + glm::ivec2{ x, z };
			
					generateChunk(*loadedChunks[id]);

				}

			}

		}
		
		std::set<int> chunksToRecalculate;

		setNeighbours(newCreatedChunks, chunksToRecalculate);
		
		newCreatedChunks.merge(chunksToRecalculate);

		for(auto id : newCreatedChunks)
		{

			loadedChunks[id]->calculateFaces();
		
		}


		//todo for later
		//for(auto id: chunksToRecalculate)
		//{
		//	//todo here we can implement calculate faces only for edges 
		//	loadedChunks[id]->calculateFaces();
		//}

	}

	bottomCorner = newBottomCorner;
	topCorner = newTopCorner;
	this->playerPos = playerPos;
}

void ChunkManager::generateChunk(Chunk &c)
{

	//this->clearBlockData();

	int xPadd = c.position.x * 16;
	int yPadd = c.position.y * 16;


	float *testNoise
		= heightNoise->GetSimplexFractalSet(xPadd, 0, yPadd, CHUNK_SIZE, (1), CHUNK_SIZE, 1);


	auto getNoiseVal = [testNoise](int x, int y, int z)
	{
		return testNoise[x * CHUNK_SIZE * (1) + y * CHUNK_SIZE + z];
	};


	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			int height = 50 + getNoiseVal(i, 0, j) * 30;

			for (int h = 0; h < CHUNK_HEIGHT; h++)
			{
				if (h < height - 1)
				{
					Block b(BLOCKS::stone);
					*c.getBlock(i, h, j) = b;
				}
				else
				if (h < height)
				{
					Block b(BLOCKS::dirt);
					*c.getBlock(i, h, j) = b;
				}
				else
				if (h == height)
				{
					Block b(BLOCKS::grass);
					*c.getBlock(i, h, j) = b;
				}
				else
				{
					Block b(BLOCKS::air);
					*c.getBlock(i, h, j) = b;
				}

			}


		}
	}

	FastNoiseSIMD::FreeNoiseSet(testNoise);

}

glm::ivec2 ChunkManager::computeBottomCorner(glm::vec2 playerPos, int size)
{
	auto playerInChunk = getPlayerInChunk(playerPos);

	glm::ivec2 cornesOfChunkRegion = playerInChunk - (size / 2);
	return cornesOfChunkRegion;

}

glm::ivec2 ChunkManager::computeTopCorner(glm::vec2 playerPos, int size)
{
	auto playerInChunk = getPlayerInChunk(playerPos);

	glm::ivec2 cornesOfChunkRegion = playerInChunk - (size / 2);
	return cornesOfChunkRegion + size;

}

glm::ivec2 ChunkManager::getPlayerInChunk(glm::vec2 playerPos)
{

	//glm::ivec2 playerInChunk = glm::ivec2(playerPos.x / 16, playerPos.y / 16);
	//
	//if (playerPos.x < 0)
	//{
	//	playerInChunk.x--;
	//}
	//
	//if (playerPos.y < 0)
	//{
	//	playerInChunk.y--;
	//}
	//
	//return playerInChunk;
	

	glm::ivec2 playerInChunk;
	
	if (playerPos.x < 0)
	{
		playerInChunk.x = (playerPos.x+0.001) / 16;
		playerInChunk.x--;
	}else
	{
		playerInChunk.x = playerPos.x / 16;
	}
	
	if (playerPos.y < 0)
	{
		playerInChunk.y = (playerPos.y + 0.001) / 16;
		playerInChunk.y--;
	}else
	{
		playerInChunk.y = playerPos.y / 16;
	}
	
	return playerInChunk;

}


bool ChunkManager::rayCast(glm::ivec3 &pos, glm::vec3 start, glm::vec3 direction, float maxRaySize)
{
	pos = {0,0,0};

	glm::vec3 p = start;

	direction = glm::normalize(direction);
	direction *= 0.1;

	for (float moveSoFar = 0; moveSoFar <= maxRaySize; moveSoFar += 0.1)
	{
		if (p.y < 0) { return 0; }
		if (p.y >= CHUNK_HEIGHT) { return 0; }

		glm::ivec3 blockPos = p;
		if (p.x < 0) { blockPos.x--; }
		if (p.z < 0) { blockPos.z--; }

		auto b = getBlockRaw(blockPos);

		if(b==nullptr)
		{
			return 0;
		}

		if(b->getType() != air)
		{
			pos = blockPos;

			return 1;
		}

		p += direction;
	}

	return 0;
}

Block *ChunkManager::getBlockRaw(glm::ivec3 pos, Chunk **c)
{
	if (c)
	{
		*c = nullptr;
	}


	glm::ivec2 chunkPos = getPlayerInChunk({pos.x, pos.z});

	auto it = std::find_if(loadedChunks.begin(), loadedChunks.end(),
					[pos = chunkPos ](Chunk *c) { return c->getChunkPosition() == pos; }
	);

	if (it != loadedChunks.end())
	{
		glm::ivec3 blockPos = pos;

		if(pos.x < 0)
		{
			blockPos.x -= chunkPos.x * CHUNK_SIZE;
			//blockPos.x = CHUNK_SIZE - blockPos.x;
		}else
		{
			blockPos.x -= chunkPos.x * CHUNK_SIZE;
		}

		if(pos.z < 0)
		{
			blockPos.z -= chunkPos.y * CHUNK_SIZE;
			//blockPos.z = CHUNK_SIZE  - blockPos.z;
		}else
		{
			blockPos.z -= chunkPos.y * CHUNK_SIZE;
		}


		if(c)
		{
			*c = *it;
		}

		return (*it)->getBlock(blockPos.x, blockPos.y, blockPos.z);

	}

	return nullptr;
}

void ChunkManager::setNeighbours(std::set<int> &newCreatedChunks, std::set<int> &chunksToRecalculate)
{
	for (auto id : newCreatedChunks)
	{

		int x = loadedChunks[id]->getChunkPosition().x;
		int z = loadedChunks[id]->getChunkPosition().y;

		glm::ivec2 posFront = glm::ivec2{ x, z + 1 };
		glm::ivec2 posBack	= glm::ivec2{ x, z - 1 };
		glm::ivec2 posLeft	= glm::ivec2{ x - 1, z };
		glm::ivec2 posRight = glm::ivec2{ x + 1, z };

		
		if (!loadedChunks[id]->chunkInFront)
		{
			auto itFront = std::find_if(loadedChunks.begin(), loadedChunks.end(),
						[pos = posFront](Chunk *c) { return c->getChunkPosition() == pos; });

			if (itFront != loadedChunks.end())
			{
				loadedChunks[id]->chunkInFront = *itFront;

				auto ind = itFront - loadedChunks.begin();
				chunksToRecalculate.insert(ind);
			}

		}
		
		if(!loadedChunks[id]->chunkInBack)
		{
			auto itBack = std::find_if(loadedChunks.begin(), loadedChunks.end(),
				[pos = posBack](Chunk *c) { return c->getChunkPosition() == pos; });

			if (itBack != loadedChunks.end())
			{
				loadedChunks[id]->chunkInBack = *itBack;

				auto ind = itBack - loadedChunks.begin();
				chunksToRecalculate.insert(ind);
			}
		
		}

		
		if(!loadedChunks[id]->chunkAtLeft)
		{
			auto itLeft = std::find_if(loadedChunks.begin(), loadedChunks.end(),
				[pos = posLeft](Chunk *c) { return c->getChunkPosition() == pos; });

			if (itLeft != loadedChunks.end())
			{
				loadedChunks[id]->chunkAtLeft = *itLeft;


				auto ind = itLeft - loadedChunks.begin();
				chunksToRecalculate.insert(ind);
			}
		}

		if(!loadedChunks[id]->chunkAtRight)
		{
			auto itRight = std::find_if(loadedChunks.begin(), loadedChunks.end(),
				[pos = posRight](Chunk *c) { return c->getChunkPosition() == pos; });

			if (itRight != loadedChunks.end())
			{
				loadedChunks[id]->chunkAtRight = *itRight;
		
				auto ind = itRight - loadedChunks.begin();
				chunksToRecalculate.insert(ind);
			}

		}

		loadedChunks[id]->updateNeighbours();

	}

}
