#include "block.h"

glm::ivec2 frontFaces[BLOCKS_COUNT] =
{
	{0,0},
	{3, 15},

};

glm::ivec2 backFaces[BLOCKS_COUNT] =
{
	{0,0},
	{3, 15},

};

glm::ivec2 topFaces[BLOCKS_COUNT] =
{
	{0,0},
	{0, 15},

};

glm::ivec2 bottomFaces[BLOCKS_COUNT] =
{
	{0,0},
	{2, 15},

};

glm::ivec2 leftFaces[BLOCKS_COUNT] =
{
	{0,0},
	{3, 15},

};

glm::ivec2 rightFaces[BLOCKS_COUNT] =
{
	{0,0},
	{3, 15},

};

static glm::ivec2 *faces[6] =
{
	frontFaces, backFaces, topFaces, bottomFaces, leftFaces, rightFaces
};


glm::ivec2 Block::getPositionInAtlas(int face)
{
	return faces[face][type];
}


static const char *blockNames[BLOCKS_COUNT]
{
	"air",
	"grass block"
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
