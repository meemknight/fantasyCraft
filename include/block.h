#pragma once
#include <glm/vec2.hpp>
#include <iostream>

enum BLOCKS
{
	air, 
	grass,
	BLOCKS_COUNT
};

class Block
{
public:
	Block() {};
	Block(int type):type(type){};

	Block(Block &other) { this->type = other.type; }

	uint8_t &getType() { return type; }
	uint8_t getType() const { return type; }

	bool isTransparent() const
	{
		return !isOpaque();
	}

	bool isOpaque() const
	{
		if(type == air)
		{
			return false;
		}

		return true;
	}

	const char *getBlockName() const;

	glm::ivec2 getPositionInAtlas(int face);

	friend std::ostream &operator<<(std::ostream &os, const Block &block);

	Block & operator=(const Block &other)
	{
		return *this;
	}

private:
	uint8_t type = 0;
};

