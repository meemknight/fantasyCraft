#pragma once
#include <glm/vec2.hpp>
#include <iostream>

enum BLOCKS
{
	air = 0, 
	grass,
	dirt,
	stone,
	ice,
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

	bool isTranslucent() const
	{
		if(type == ice)
		{
			return true;
		}

		return false;
	}

	bool isOpaque() const
	{
		if(type == air || type == ice)
		{
			return false;
		}

		return true;
	}

	bool isAir() const
	{
		return type == air;
	}

	const char *getBlockName() const;

	glm::ivec2 getPositionInAtlas(int face);

	friend std::ostream &operator<<(std::ostream &os, const Block &block);

	Block & operator=(const Block &other)
	{
		this->type = other.type;
		return *this;
	}

private:
	unsigned char type = 0;
};

