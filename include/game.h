#pragma once

#include "gl2d/gl2d.h"
#include <glad/glad.h>
#include "shader.h"
#include "camera.h"
#include "glm/vec2.hpp"
#include <texture.h>
#include "renderer.h"
#include "skyBox.h"


class GameInput
{
public:

	class Button
	{
	public:

		bool isPressed() { return pressed; };
		bool isHeld() { return held; }
		bool isReleased() { return released; }



	private:

		friend GameInput;

		bool held = 0;
		bool pressed = 0;
		bool released = 0;

		void setPress()
		{
			pressed = true;
			held = true;
		}

		void setRelease()
		{
			released = true;
			held = false;
		}


		void resetInput()
		{
			released = 0;
			pressed = 0;
		}
	};

	enum keys 
	{
		FRONT, 
		BACK,
		LEFT,
		RIGHT,
		LEFT_CLICK,
		RIGHT_CLICK,
		DOWN,
		UP,
		C,
		BUTTONS_COUNT
	};

	Button getKey(int k) const
	{
		if(k < 0 || k >= BUTTONS_COUNT)
		{
			return Button{};
		}

		return buttons[k];
	}

	void setKeyPress(int k) 
	{
		if (k < 0 || k >= BUTTONS_COUNT)
		{
			return;
		}

		buttons[k].setPress();
	}

	void setKeyRelease(int k)
	{
		if (k < 0 || k >= BUTTONS_COUNT)
		{
			return;
		}

		buttons[k].setRelease();
	}


	void resetInputsThisFrame()
	{
		for(int i=0; i< BUTTONS_COUNT; i++)
		{
			buttons[i].resetInput();
		}
	}

	void resetInputsToZero()
	{
		for (int i = 0; i < BUTTONS_COUNT; i++)
		{
			buttons[i].setRelease();
			buttons[i].resetInput();
		}
	}

	int getMousePosX() const
	{
		return mousePosX;
	}

	int getMousePosY() const
	{
		return mousePosY;
	}

	glm::ivec2 getMousePos() const
	{
		return glm::ivec2(mousePosX, mousePosY);
	}

	void setMousePosition(int x, int y)
	{
		mousePosX = x;
		mousePosY = y;
	}

private:

	Button buttons[BUTTONS_COUNT];
	int mousePosX;
	int mousePosY;

};


class Game
{
public:
	
	Game(int screenW, int screenH) { onCreate(screenW, screenH); }
	~Game() { onDestroy(); }

	void onUpdate(float deltaTime, const GameInput &input);


	void updateWindowMetrics(int screenW, int screenH);


private:
	void onCreate(int screenW, int screenH);

	void onDestroy() {};

	//game data
	gl2d::Renderer2D renderer2d;
	gl2d::Font font;
	int screenW, screenH;

	Camera *camera = 0;
	bool isCreativeCamera = 0;

	ChunksRenderer renderer;

	ChunkManager chunkManager;

	gl2d::Texture arrowTexture;

	SkyBox skyBox;

};