#include "..\include\game.h"
#include <iostream>

void Game::onCreate(int screenW, int screenH)
{

	updateWindowMetrics(screenW, screenH);

	//create 2D renderer
	renderer2d.create();

	font.createFromFile(RESOURCES_PATH "roboto_black.ttf");
	texture.loadFromFile(RESOURCES_PATH "test.jpg");



}

void Game::onUpdate(float deltaTime, const GameInput &input)
{
	renderer2d.clearScreen();


	const float speed = 100 * deltaTime;

	if(input.getKey(GameInput::RIGHT_CLICK).isHeld())
	{
		posX -= speed;
	}

	if (input.getKey(GameInput::RIGHT).isHeld())
	{
		posX += speed;
	}
	

	renderer2d.renderRectangle({ posX, posY, 100, 100 }, { 0,0 }, 0, texture);
	glm::vec4 colors[4] = { Colors_Orange,Colors_Orange ,Colors_Orange ,Colors_Orange };
	renderer2d.renderRectangle({ 10,10, 100, 100 }, colors, {}, 30);

	renderer2d.renderText({ 100,100 }, "Hello", font, Colors_White);



	renderer2d.flush();
}

void Game::updateWindowMetrics(int screenW, int screenH)
{
	this->screenW = screenW;
	this->screenH = screenH;

	renderer2d.updateWindowMetrics(screenW, screenH);

}
