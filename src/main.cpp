#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <stb_truetype/stb_truetype.h>
#include "gl2d/gl2d.h"
#include <iostream>
#include <ctime>
#include "tools.h"
#include "game.h"
#include "log.h"

#ifdef PLATFORM_WIN32
#include <Windows.h>
#endif

#undef min
#undef max

GameInput input;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

	auto setKey = [&action](GameInput::keys k) 
	{
		if (action == GLFW_PRESS)
		{
			input.setKeyPress(k);
		}
		else
		if (action == GLFW_RELEASE)
		{
			input.setKeyRelease(k);
		}
	};

	if (key == GLFW_KEY_W || key == GLFW_KEY_UP)
	{
		setKey(GameInput::FRONT);
	}

	if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
	{
		setKey(GameInput::LEFT);
	}

	if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN)
	{
		setKey(GameInput::BACK);
	}

	if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
	{
		setKey(GameInput::RIGHT);
	}

	if (key == GLFW_KEY_Q)
	{
		setKey(GameInput::DOWN);
	}

	if (key == GLFW_KEY_E)
	{
		setKey(GameInput::UP);
	}

	if (key == GLFW_KEY_C)
	{
		setKey(GameInput::C);
	}

	if (key == GLFW_KEY_P)
	{
		setKey(GameInput::P);
	}



};

void mouseCallback(GLFWwindow *window, int key, int action, int mods)
{

	if (key == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			input.setKeyPress(GameInput::LEFT_CLICK);
		}
		else
		if (action == GLFW_RELEASE)
		{
			input.setKeyRelease(GameInput::LEFT_CLICK);
		}

	}

	if (key == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			input.setKeyPress(GameInput::RIGHT_CLICK);
		}
		else
		if (action == GLFW_RELEASE)
		{
			input.setKeyRelease(GameInput::RIGHT_CLICK);
		}

	}
}

void windowFocusCallback(GLFWwindow *window, int focused)
{
	if (focused)
	{
		
	}
	else
	{
		//if you not capture the release event when the window loses focus,
		//the buttons will stay pressed
		input.resetInputsToZero();
	}
}

void windowSizeCallback(GLFWwindow *window, int x, int y)
{
	
	input.resetInputsToZero();
}

int main()
{

#ifdef PLATFORM_WIN32
#ifdef _MSC_VER 
#if INTERNAL_BUILD
{
	AllocConsole();
	auto f1 = freopen("conin$", "r", stdin);
	auto f2 = freopen("conout$", "w", stdout);
	auto f3 = freopen("conout$", "w", stderr);
	std::cout.sync_with_stdio();
}
#endif
#endif
#endif

	permaAssertComment(glfwInit(), "err initializing glfw");


	glfwWindowHint(GLFW_SAMPLES, 4);

	int w = 500;
	int h = 500;
	GLFWwindow* wind = glfwCreateWindow(w, h, "geam", nullptr, nullptr);
	glfwMakeContextCurrent(wind);
	glfwSwapInterval(1);
	glfwSetKeyCallback(wind, keyCallback);
	glfwSetMouseButtonCallback(wind, mouseCallback);
	glfwSetWindowFocusCallback(wind, windowFocusCallback);
	glfwSetWindowSizeCallback(wind, windowSizeCallback);
	glfwSetInputMode(wind, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	permaAssertComment(gladLoadGL(), "err initializing glad");
	
	gl2d::init();
	
	try
	{
		Game game(500, 500);

	
		long lastTime = clock();

		while (!glfwWindowShouldClose(wind))
		{
			int w = 0; int h = 0;
			glfwGetWindowSize(wind, &w, &h);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		#pragma region movement
			long newTime = clock();
			float deltaTime = (float)(newTime - lastTime) / CLOCKS_PER_SEC;
			lastTime = clock();
		#pragma endregion

			double xMouse, yMouse;
			glfwGetCursorPos(wind, &xMouse, &yMouse);
			input.setMousePosition((int)xMouse, (int)yMouse);

			game.updateWindowMetrics(w, h);
			game.onUpdate(deltaTime, input);
			input.resetInputsThisFrame();


			glfwSwapBuffers(wind);
			glfwPollEvents();
		}
	}
	catch (std::string msg)
	{
		llog(ErrorLog(), msg);
		std::cin.clear();
		std::cin.get();
	}



	//if you want the console to stay after closing the window
	//std::cin.clear();
	//std::cin.get();
}