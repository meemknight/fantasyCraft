#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include "gameLayer.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "platformInput.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include "imfilebrowser.h"
#include <gl2d/gl2d.h>
#include <notepad.h>
#include <glad/glad.h>
#include "shader.h"
#include "camera.h"
#include "glm/vec2.hpp"
#include <texture.h>
#include "renderer.h"
#include "skyBox.h"


gl2d::Texture t;

//game data
gl2d::Renderer2D renderer2d;
gl2d::Font font;

Camera *camera = 0;
bool isCreativeCamera = 0;

ChunksRenderer renderer;

ChunkManager chunkManager;

gl2d::Texture arrowTexture;

SkyBox skyBox;

void updateWindowMetrics(int screenW, int screenH)
{
	screenW = screenW;
	screenH = screenH;

	renderer2d.updateWindowMetrics(screenW, screenH);
	glViewport(0, 0, screenW, screenH);

	camera->updateAspectRatio(screenW, screenH);
}


bool initGame(gl2d::FrameBuffer fbo)
{
	renderer.init();

	camera = new CreativeModeCamera();
	isCreativeCamera = true;

	updateWindowMetrics(platform::getFrameBufferSizeX(), platform::getFrameBufferSizeY());

	//create 2D renderer
	renderer2d.create();

	font.createFromFile(RESOURCES_PATH "roboto_black.ttf");

	camera->getPosition() = {0,70,0};


	chunkManager.setGridSize(10, glm::vec2{camera->getPosition().x, camera->getPosition().z});

	//std::cout << chunkManager.bottomCorner.x << " " << chunkManager.bottomCorner.y << "\n";
	//std::cout << chunkManager.topCorner.x << " " << chunkManager.topCorner.y << "\n";

	arrowTexture.loadFromFile(RESOURCES_PATH  "arrow.png");

	skyBox.create();

	const char *faces[6] =
	{
		RESOURCES_PATH "right.jpg",
		RESOURCES_PATH "left.jpg",
		RESOURCES_PATH "top.jpg",
		RESOURCES_PATH "bottom.jpg",
		RESOURCES_PATH "front.jpg",
		RESOURCES_PATH "back.jpg"
	};

	//skyBox.loadTextures(faces);
	skyBox.loadTexturesFromCrossTexture(RESOURCES_PATH "skyBox.png");

	return true;
}



bool gameLogic(float deltaTime, gl2d::FrameBuffer fbo)
{
#pragma region init stuff
	int w = 0; int h = 0;
	w = platform::getFrameBufferSizeX(); //window w
	h = platform::getFrameBufferSizeY(); //window h
	
	glViewport(0, 0, w, h);
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo.fbo);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear screen


#pragma endregion
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_MULTISAMPLE);

	//input
	{
		if (platform::isKeyPressedOn(platform::Button::C))
		{
			if (isCreativeCamera)
			{
				Camera *c = new FlyCamera();
				*c = *camera;
				delete camera;
				camera = c;
			}
			else
			{
				Camera *c = new CreativeModeCamera();
				*c = *camera;
				delete camera;
				camera = c;
			}

			isCreativeCamera = !isCreativeCamera;
		}

		//if (platform::isKeyReleased(platform::Button::P))
		//{
		//	renderer.getAo() = !renderer.getAo();
		//}

		const float speed = 16 * deltaTime;
		glm::vec3 movePos = {};


		if (platform::isKeyHeld(platform::Button::Left))
		{
			movePos.x -= speed;
		}

		if (platform::isKeyHeld(platform::Button::Right))
		{
			movePos.x += speed;
		}

		if (platform::isKeyHeld(platform::Button::Up))
		{
			movePos.z -= speed;
		}

		if (platform::isKeyHeld(platform::Button::Down))
		{
			movePos.z += speed;
		}

		if (platform::isKeyHeld(platform::Button::Q))
		{
			movePos.y -= speed;
		}

		if (platform::isKeyHeld(platform::Button::E))
		{
			movePos.y += speed;
		}
		camera->move(movePos);

		static int lastMouseX;
		static int lastMouseY;

		glm::vec2 delta = platform::getRelMousePosition();
		delta -= glm::vec2(lastMouseX, lastMouseY);
		camera->rotateCamera(delta * deltaTime);
		lastMouseX = platform::getRelMousePosition().x;
		lastMouseY = platform::getRelMousePosition().y;

		//platform::setRelMousePosition(platform::getFrameBufferSizeX() / 2, platform::getFrameBufferSizeY() / 2);

	}

	//std::cout << "pos: " << camera.getPosition().x << " " << camera.getPosition().z << " -- chunk"
	//	<< chunkManager.getPlayerInChunk({camera.getPosition().x, camera.getPosition().z}).x
	//	<< " " << chunkManager.getPlayerInChunk({ camera.getPosition().x, camera.getPosition().z }).y << "\n";


	chunkManager.setPlayerPos(glm::vec2{camera->getPosition().x, camera->getPosition().z});


	glm::ivec3 rayEnd = {};
	glm::ivec3 blockPlace = {};
	if (chunkManager.rayCast(rayEnd, blockPlace, camera->getPosition() +
		glm::vec3{0.5, 0.5, 0.5},
		camera->getViewDirection()))
	{
		auto b = chunkManager.getBlockRaw(rayEnd);
		if (b)
		{
			//std::cout << *b << "\n";
		}

		if (platform::isRMousePressed())
		{
			chunkManager.placeBlock(blockPlace, stone, camera->getPosition());
		}

		if (platform::isLMousePressed())
		{
			chunkManager.placeBlock(rayEnd, 0, camera->getPosition());
		}

	}


	renderer.render(*camera, chunkManager, skyBox);

	//2d ui stuff

	renderer2d.renderRectangle({platform::getFrameBufferSizeX() / 2 - 15,
		platform::getFrameBufferSizeX() / 2 - 15, 30, 30},
		arrowTexture);

	renderer2d.flush();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
#pragma endregion

}

//This function might not be be called if the program is forced closed
void closeGame()
{


}
