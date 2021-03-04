#include "..\include\game.h"
#include <iostream>

void Game::onCreate(int screenW, int screenH)
{

	updateWindowMetrics(screenW, screenH);

	//create 2D renderer
	renderer2d.create();

	font.createFromFile(RESOURCES_PATH "roboto_black.ttf");
	texture.loadFromFile(RESOURCES_PATH "test.jpg");

	glGenVertexArrays(1, &frontFaceVAO);
	glBindVertexArray(frontFaceVAO);

	glGenBuffers(1, &frontFaceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, frontFaceBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(facePositions), facePositions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glGenBuffers(1, &frontFaceIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, frontFaceIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faceIndeces), faceIndeces, GL_STATIC_DRAW);

	
	glBindVertexArray(0);


	shader.loadFromFiles(RESOURCES_PATH "vertex.vert", RESOURCES_PATH "fragment.frag");

}

void Game::onUpdate(float deltaTime, const GameInput &input)
{


	const float speed = 100 * deltaTime;

	if(input.getKey(GameInput::RIGHT_CLICK).isHeld())
	{
		posX -= speed;
	}

	if (input.getKey(GameInput::RIGHT).isHeld())
	{
		posX += speed;
	}
	

	

	glBindVertexArray(frontFaceVAO);
	shader.bind();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

}

void Game::updateWindowMetrics(int screenW, int screenH)
{
	this->screenW = screenW;
	this->screenH = screenH;

	renderer2d.updateWindowMetrics(screenW, screenH);

}
