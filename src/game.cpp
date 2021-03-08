#include "game.h"
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "chunks.h"

void Game::onCreate(int screenW, int screenH)
{

	updateWindowMetrics(screenW, screenH);

	//create 2D renderer
	renderer2d.create();

	font.createFromFile(RESOURCES_PATH "roboto_black.ttf");

	camera.getPosition() = { 0,70,0 };


	chunkManager.setGridSize(16, glm::vec2{camera.getPosition().x, camera.getPosition().z});

	std::cout << chunkManager.bottomCorner.x << " " << chunkManager.bottomCorner.y << "\n";
	std::cout << chunkManager.topCorner.x << " " << chunkManager.topCorner.y << "\n";

	arrowTexture.loadFromFile(RESOURCES_PATH  "arrow.png");

}

void Game::onUpdate(float deltaTime, const GameInput &input)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_MULTISAMPLE);
	//glEnable(GL_MULTISAMPLE);
	
	//input
	{
		const float speed = 100 * deltaTime;
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
		
		glm::vec2 delta = input.getMousePos();
		delta -= glm::vec2(lastMouseX, lastMouseY);
		camera.rotateCamera(delta * deltaTime);	   
		lastMouseX = input.getMousePosX();
		lastMouseY = input.getMousePosY();
		
	}
	
	//std::cout << "pos: "  <<  camera.getPosition().x << " " << camera.getPosition().z << "\n";
	chunkManager.setPlayerPos(glm::vec2{ camera.getPosition().x, camera.getPosition().z });

	Block b(BLOCKS::grass);

	renderer.render(camera, chunkManager);
	

	//2d ui stuff

	renderer2d.renderRectangle({ screenW / 2 - 15, screenH / 2 - 15, 30, 30 },
		{}, {},
		arrowTexture);

	renderer2d.flush();

}

void Game::updateWindowMetrics(int screenW, int screenH)
{
	this->screenW = screenW;
	this->screenH = screenH;

	renderer2d.updateWindowMetrics(screenW, screenH);
	glViewport(0, 0, screenW, screenH);

	camera.updateAspectRatio(screenW, screenH);

}
