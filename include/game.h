#include "gl2d/gl2d.h"
#include <glad/glad.h>
#include "shader.h"

class GameInput;

class Button
{
public:

	bool isPressed() { return pressed; };
	bool isHeld() { return held; }
	bool isReleased() { return released; }

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

	
private:
	
	friend GameInput;

	bool held = 0;
	bool pressed = 0;
	bool released = 0;

	void resetInput()
	{
		released = 0;
		pressed = 0;
	}
};


class GameInput
{
public:
	enum keys 
	{
		FRONT, 
		BACK,
		LEFT,
		RIGHT,
		LEFT_CLICK,
		RIGHT_CLICK,
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



private:

	Button buttons[BUTTONS_COUNT];

};


class Game
{
public:
	void onCreate(int screenW, int screenH);
	
	void onUpdate(float deltaTime, const GameInput &input);

	void onDestroy() {};

	void updateWindowMetrics(int screenW, int screenH);

private:

	//game data
	gl2d::Renderer2D renderer2d;
	gl2d::Font font;
	gl2d::Texture texture;
	int screenW, screenH;

	float posX = 0;
	float posY = 0;

	//front
	float facePositions[12] = 
	{
		0.5, 0.5, 0,
		-0.5, 0.5, 0,
		-0.5, -0.5, 0,
		0.5, -0.5, 0
	};

	//front
	unsigned int faceIndeces[6]
	{
		0,1,2,
		2,3,0
	};

	GLuint frontFaceVAO;
	GLuint frontFaceBuffer;
	GLuint frontFaceIndexBuffer;

	Shader shader;
};