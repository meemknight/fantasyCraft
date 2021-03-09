#include "block.h"

glm::ivec2 frontFaces[BLOCKS_COUNT] =
{
	{0,0},
	{3, 15}, //grass
	{2, 15}, // dirt
	{1, 15}, //stone

};

glm::ivec2 backFaces[BLOCKS_COUNT] =
{
	{0,0},
	{3, 15}, //grass
	{2, 15}, // dirt
	{1, 15}, //stone

};

glm::ivec2 topFaces[BLOCKS_COUNT] =
{
	{0,0},
	{0, 15}, //grass
	{2, 15}, // dirt
	{1, 15}, //stone

};

glm::ivec2 bottomFaces[BLOCKS_COUNT] =
{
	{0,0},
	{2, 15}, //grass
	{2, 15}, // dirt
	{1, 15}, //stone

};

glm::ivec2 leftFaces[BLOCKS_COUNT] =
{
	{0,0},
	{3, 15}, //grass
	{2, 15}, // dirt
	{1, 15}, //stone

};

glm::ivec2 rightFaces[BLOCKS_COUNT] =
{
	{0,0},
	{3, 15}, //grass
	{2, 15}, // dirt
	{1, 15}, //stone

};

static glm::ivec2 *faces[6] =
{
	frontFaces, backFaces, topFaces, bottomFaces, leftFaces, rightFaces
};


glm::ivec2 Block::getPositionInAtlas(int face)
{
	return faces[face][type];
}


static const char *blockNames[BLOCKS_COUNT + 1]
{
	"air",
	"grass block",
	"dirt block",
	"stone block",
	""
};

const char *Block::getBlockName() const
{
	return blockNames[type];
}

std::ostream &operator<<(std::ostream &os, const Block &block)
{
	os << block.getBlockName();
	return os;
}
