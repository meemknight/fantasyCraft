#include "game.h"
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"

void Game::onCreate(int screenW, int screenH)
{

	updateWindowMetrics(screenW, screenH);

	//create 2D renderer
	renderer2d.create();

	font.createFromFile(RESOURCES_PATH "roboto_black.ttf");
	texture.load(RESOURCES_PATH "test.jpg");

	glGenVertexArrays(1, &frontFaceVAO);
	glBindVertexArray(frontFaceVAO);

	glGenBuffers(1, &frontFaceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, frontFaceBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(facePositions), facePositions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glGenBuffers(1, &frontFaceTextureIndexesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, frontFaceTextureIndexesBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(faceTexture), faceTexture, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	

	glGenBuffers(1, &frontFaceIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, frontFaceIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faceIndeces), faceIndeces, GL_STATIC_DRAW);

	
	glBindVertexArray(0);


	shader.load();

}

void Game::onUpdate(float deltaTime, const GameInput &input)
{

	//input
	{
		const float speed = 1 * deltaTime;
		glm::vec3 movePos = {};

		if (input.getKey(GameInput::LEFT).isHeld())
		{
			movePos.x -= speed;
		}

		if (input.getKey(GameInput::RIGHT).isHeld())
		{
			movePos.x += speed;
		}

		if (input.getKey(GameInput::FRONT).isHeld())
		{
			movePos.z -= speed;
		}

		if (input.getKey(GameInput::BACK).isHeld())
		{
			movePos.z += speed;
		}

		if (input.getKey(GameInput::DOWN).isHeld())
		{
			movePos.y -= speed;
		}

		if (input.getKey(GameInput::UP).isHeld())
		{
			movePos.y += speed;
		}
		camera.move(movePos);

		static int lastMouseX;
		static int lastMouseY;
		if(input.getKey(GameInput::RIGHT_CLICK).isHeld())
		{
			glm::vec2 delta = input.getMousePos();
			delta -= glm::vec2(lastMouseX, lastMouseY);
			camera.rotateCamera(delta * deltaTime);	   
			lastMouseX = input.getMousePosX();
			lastMouseY = input.getMousePosY();
		}else
		{
			lastMouseX = input.getMousePosX();
			lastMouseY = input.getMousePosY();
		}
	}

	glBindVertexArray(frontFaceVAO);
	shader.bind();
	shader.setPlayerPos(camera.getPosition());
	shader.setProjectionMatrix(camera.getProjectionMatrix());
	shader.setModelViewMatrix(camera.getViewMatrix());
	texture.bind(0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

}

void Game::updateWindowMetrics(int screenW, int screenH)
{
	this->screenW = screenW;
	this->screenH = screenH;

	renderer2d.updateWindowMetrics(screenW, screenH);
	glViewport(0, 0, screenW, screenH);

	camera.updateAspectRatio(screenW, screenH);

}
