#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <stb_truetype/stb_truetype.h>
#include "gl2d/gl2d.h"
#include <iostream>
#include <ctime>
#include "platformTools.h"
#include "config.h"
#include "platformInput.h"
#include "otherPlatformFunctions.h"
#include "gameLayer.h"
#include <fstream>
#include <chrono>
#include <notepad.h>
#include <openglError.h>

#define REMOVE_IMGUI 0

#if REMOVE_IMGUI == 0
	#include "imgui.h"
	#include "backends/imgui_impl_glfw.h"
	#include "backends/imgui_impl_opengl3.h"
	#include "imguiThemes.h"
#endif

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif

bool windowFocus = 1;

void windowSizeCallback(GLFWwindow *window, int x, int y)
{
	platform::internal::resetInputsToZero();
}


#pragma region platform functions

GLFWwindow *wind = 0;

bool makeTheWindowVisible = 1;

namespace platform
{

	bool writeEntireFile(const char *name, void *buffer, size_t size)
	{
		std::ofstream f(name, std::ios::binary);

		if(!f.is_open())
		{
			return 0;
		}

		f.write((char*)buffer, size);

		f.close();

		return 1;
	}


	bool readEntireFile(const char *name, void *buffer, size_t size)
	{
		std::ifstream f(name, std::ios::binary);

		if (!f.is_open())
		{
			return 0;
		}

		f.read((char *)buffer, size);

		f.close();

		return 1;
	}

};
#pragma endregion


float superSampleTresshold = 0.5;
int currentSampleMethod = 0;
bool applyCurves = 1;

//float curvesA = 0.118;
//float curvesB = 0.565;

float curvesA = 0.075;
float curvesB = 0.315;

int findX = 0;
int findY = 0;

int main()
{

#ifdef _WIN32
#ifdef _MSC_VER 
#if INTERNAL_BUILD
	AllocConsole();
	(void)freopen("conin$", "r", stdin);
	(void)freopen("conout$", "w", stdout);
	(void)freopen("conout$", "w", stderr);
	std::cout.sync_with_stdio();
#endif
#endif
#endif

#pragma region init notepad

	permaAssertComment(openTheNotepadInstance(), "err initializing the notepad");

#pragma endregion

#pragma region window and opengl

	permaAssertComment(glfwInit(), "err initializing glfw");
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_VISIBLE, makeTheWindowVisible);
	glfwWindowHint(GLFW_RESIZABLE, false);

	auto notepadSize = platform::getFrameBufferSize();

	int w = notepadSize.x;
	int h = notepadSize.y;
	wind = glfwCreateWindow(w, h, "geam", nullptr, nullptr);
	glfwMakeContextCurrent(wind);
	glfwSwapInterval(2);

	glfwSetWindowSizeCallback(wind, windowSizeCallback);

	//permaAssertComment(gladLoadGL(), "err initializing glad");
	permaAssertComment(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "err initializing glad");

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

#pragma endregion

#pragma region gl2d
	gl2d::init();
#pragma endregion

	gl2d::Renderer2D screenRenderer;
	if(makeTheWindowVisible)
	{
		screenRenderer.create(0, 20);
	}
	screenRenderer.updateWindowMetrics(platform::getFrameBufferSize().x, platform::getFrameBufferSize().y);

	gl2d::FrameBuffer windowFbo;
	windowFbo.create(platform::getFrameBufferSize().x, platform::getFrameBufferSize().y);


#pragma region imgui
	#if REMOVE_IMGUI == 0
		ImGui::CreateContext();
		//ImGui::StyleColorsDark();
		imguiThemes::embraceTheDarkness();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;
	
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			//style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 0.f;
			style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;
		}
	
		ImGui_ImplGlfw_InitForOpenGL(wind, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	#endif
#pragma endregion


#pragma region initGame
	if (!initGame(windowFbo))
	{
		return 0;
	}
#pragma endregion


	//long lastTime = clock();
	
	auto stop = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(wind))
	{
		//Sleep(2);
		//UpdateMusicStream(m);
		//PlayMusicStream(m);

	#pragma region deltaTime

		//long newTime = clock();
		//float deltaTime = (float)(newTime - lastTime) / CLOCKS_PER_SEC;
		//lastTime = clock();
		auto start = std::chrono::high_resolution_clock::now();

		float deltaTime = (std::chrono::duration_cast<std::chrono::nanoseconds>(start - stop)).count() / 1000000000.0;
		stop = std::chrono::high_resolution_clock::now();

		float augmentedDeltaTime = deltaTime;
		if (augmentedDeltaTime > 1.f / 10) { augmentedDeltaTime = 1.f / 10; }
	
	#pragma endregion

	#pragma region imgui
		#if REMOVE_IMGUI == 0
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		#endif
	#pragma endregion

		#pragma region imgui

			ImGui::Begin("controlls");

			ImGui::Combo("Sample method", &currentSampleMethod, "Nearest\0Linear\0Super\0Combination\0FindStuff\0");

			ImGui::Checkbox("Curves", &applyCurves);

			if (applyCurves)
			{
				ImGui::SliderFloat("A", &curvesA, 0, 1);
				ImGui::SliderFloat("B", &curvesB, 0, 1);
			}

			if (currentSampleMethod == 0)
			{

			}else if (currentSampleMethod == 1)
			{

			}
			else
			if (currentSampleMethod == 2)
			{
				ImGui::SliderFloat("Super sample tresshold", &superSampleTresshold, 0, 1);
			}
			else if (currentSampleMethod == 4)
			{
				ImGui::DragInt("X", &findX, 1, 0, 210);
				ImGui::DragInt("Y", &findY, 1, 0, 210);
			}

			ImGui::End();


		#pragma endregion

	#pragma region game logic

		if (!gameLogic(augmentedDeltaTime, windowFbo))
		{
			closeGame();
			return 0;
		}

		if (makeTheWindowVisible)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			glDisable(GL_DEPTH_TEST);

			screenRenderer.renderRectangle({0, 0, platform::getFrameBufferSize()}, windowFbo.texture);
			screenRenderer.flush();

			//glBegin(GL_TRIANGLES);
			//
			//glVertex2f(0, 1);
			//glVertex2f(1, -1);
			//glVertex2f(-1, -1);
			//
			//glEnd();

		}

	#pragma endregion

	#pragma region render to notepad
		{
			static std::vector<unsigned char> immageData;
			glm::ivec2 immageSize = windowFbo.texture.GetSize();
			immageData.reserve(4 * immageSize.x * immageSize.y);

			assert(immageSize == platform::getFrameBufferSize());

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, windowFbo.texture.id);

			glGetTexImage(GL_TEXTURE_2D,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				immageData.data());

			auto computeCurve = [](float in)
			{
				if (!applyCurves) { return in; };

				if (in <= curvesA) { return 0.f; }
				if (in >= curvesB) { return 1.f; }

				return (in - curvesA) / (curvesB - curvesA);
			};

			auto sampleImmage = [&](float x, float y) -> float
			{
				if (x < 0 || x>1 || y < 0 || y>1) { return 0.f; }

				glm::ivec2 pos = glm::ivec2(glm::vec2(x, y) * glm::vec2(immageSize - glm::ivec2(1, 1)));

				glm::vec4 data;

				data.r = immageData[(pos.x + pos.y * immageSize.x) * 4] / 255.f;
				data.g = immageData[(pos.x + pos.y * immageSize.x) * 4 + 1] / 255.f;
				data.b = immageData[(pos.x + pos.y * immageSize.x) * 4 + 2] / 255.f;

				return computeCurve(glm::dot(glm::vec3(data), glm::vec3(0.2126, 0.7152, 0.0722)));
			};

			struct Letter
			{
				char a = ' ';
				char b = 0;
			};
			
			Letter gradient[] = {{' '}, { 0x80,0x28 }, {0x88,0x28},
				{0x98,0x28}, {0x29,0x28}, {0xf1,0x28}, {145, 37}, {146, 37},
				{147, 37}, {136, 37}};

			glm::vec2 blockSizePixeld = glm::vec2(immageSize) / glm::vec2(getNotepadBufferSize());
			glm::vec2 blockSizeUV = glm::vec2(1, 1) / glm::vec2(getNotepadBufferSize());

		

			auto linearSample = [&](int x, int y)
			{

				int sampleCount = 0;
				float averageLuminosity = 0;

				for (int j = 0; j < blockSizePixeld.y; j++)
					for (int i = 0; i < blockSizePixeld.x; i++)
					{
						glm::vec2 uv = glm::vec2(x, y) / glm::vec2(getNotepadBufferSize());
						uv += blockSizeUV * glm::vec2(i / (blockSizePixeld.x - 1.f), j / (blockSizePixeld.y - 1.f));

						auto s = sampleImmage(uv.x, 1.f - uv.y);

						sampleCount++;
						averageLuminosity += s;
					}
				averageLuminosity /= sampleCount;

				int gradientCount = sizeof(gradient) / sizeof(Letter);
				int index = (1 - averageLuminosity) * (gradientCount - 1);

				writeInBuffer(x, y, gradient[index].a, gradient[index].b);
				//writeInBuffer(x, y, 0xfa, 0x28);
				//writeInBuffer(x, y, x+27, y+43);
			};

			//linear sample
			if (currentSampleMethod == 1)
			{
				for (int y = 0; y < getNotepadBufferSize().y; y++)
					for (int x = 0; x < getNotepadBufferSize().x; x++)
					{
						linearSample(x, y);
					}
			}
			
			auto nearestPixel = [&](int x, int y)
			{
				glm::vec2 uv = glm::vec2(x, y) / glm::vec2(getNotepadBufferSize());

				auto s = sampleImmage(uv.x, 1.f - uv.y);

				int gradientCount = sizeof(gradient) / sizeof(Letter);
				int index = (1 - s) * (gradientCount - 1);

				writeInBuffer(x, y, gradient[index].a, gradient[index].b);
				//writeInBuffer(x, y, 0xfa, 0x28);
				//writeInBuffer(x, y, x+27, y+43);
			};
			//nerest pixel
			if (currentSampleMethod == 0)
			{
				for (int y = 0; y < getNotepadBufferSize().y; y++)
					for (int x = 0; x < getNotepadBufferSize().x; x++)
					{
						nearestPixel(x, y);
					}
			}

			auto superSample = [&](int x, int y)
			{

				unsigned char brailBlock = {};

				auto getBrailBlock = [&](int x, int y) -> bool
				{
					assert(x >= 0 && x <= 1 && y >= 0 && y <= 3);

					if (x)
					{
						return brailBlock & (0b1'0000 << y);
					}
					else
					{
						return brailBlock & (0b1 << y);
					}

				};

				auto setBrailBlock = [&](int x, int y, bool v) -> void
				{
					assert(x >= 0 && x <= 1 && y >= 0 && y <= 3);

					if (v)
					{
						if (x)
						{
							brailBlock |= (0b1'0000 << y);
						}
						else
						{
							brailBlock |= (0b1 << y);
						}
					}
					else
					{
						if (x)
						{
							brailBlock &= !(unsigned char)(0b1'0000 << y);
						}
						else
						{
							brailBlock &= !(unsigned char)(0b1 << y);
						}
					}
				};

				for (int j = 0; j < 4; j++)
					for (int i = 0; i < 2; i++)
					{
						glm::vec2 uv = glm::vec2(x, y) / glm::vec2(getNotepadBufferSize());
						uv += blockSizeUV * glm::vec2(i / (1.f), j / (3.f));

						//sample in the middle of the 'pixel'
						//uv += blockSizeUV / (glm::vec2(2, 4) * 2.f); //not quite working

						auto s = sampleImmage(uv.x, 1.f - uv.y);

						if (s > superSampleTresshold)
						{
							setBrailBlock(i, j, false);
						}
						else
						{
							setBrailBlock(i, j, true); //set means black
						}
					}

				char remappedBrail = 0;

				if (getBrailBlock(0, 0))
				{
					remappedBrail |= 0b0000'0001;
				}

				if (getBrailBlock(0, 1))
				{
					remappedBrail |= 0b0000'0010;
				}

				if (getBrailBlock(0, 2))
				{
					remappedBrail |= 0b0000'0100;
				}

				if (getBrailBlock(1, 0))
				{
					remappedBrail |= 0b0000'1000;
				}

				if (getBrailBlock(1, 1))
				{
					remappedBrail |= 0b0001'0000;
				}

				if (getBrailBlock(1, 2))
				{
					remappedBrail |= 0b0010'0000;
				}

				if (getBrailBlock(0, 3))
				{
					remappedBrail |= 0b0100'0000;
				}

				if (getBrailBlock(1, 3))
				{
					remappedBrail |= 0b1000'0001;
				}

				writeInBuffer(x, y, remappedBrail, 0x28);

				//int gradientCount = sizeof(gradient) / sizeof(Letter);
				//int index = (1 - averageLuminosity) * (gradientCount - 1);

				//writeInBuffer(x, y, gradient[index].a, gradient[index].b);
				//writeInBuffer(x, y, x+27, y+43);
			};
			//super sample
			if (currentSampleMethod == 2)
			{
				for (int y = 0; y < getNotepadBufferSize().y; y++)
					for (int x = 0; x < getNotepadBufferSize().x; x++)
					{
						superSample(x, y);
					}
			}

			//combination
			if (currentSampleMethod == 3)
			{
				for (int y = 0; y < getNotepadBufferSize().y; y++)
					for (int x = 0; x < getNotepadBufferSize().x; x++)
					{
						if (  ((x % 2) && (y % 2)) || (!(x % 2) && !(y % 2))  )
						{
							linearSample(x, y);
						}
						else
						{
							superSample(x, y);
						}

					}
			}

			//find stuff
			if (currentSampleMethod == 4)
			{
				for (int y = 0; y < getNotepadBufferSize().y; y++)
					for (int x = 0; x < getNotepadBufferSize().x; x++)
					{
						writeInBuffer(x, y, x+ findX, y+ findY);
					}
			}

		}
	#pragma endregion



	#pragma region reset flags

		//mouseMovedFlag = 0;
		platform::internal::updateAllButtons(deltaTime);
		platform::internal::resetTypedInput();

	#pragma endregion

	#pragma region notepad stuff

		if (!checkNotepadOpen()) 
		{
			closeGame();
			return 0;
		}

		UpdateText();

	#pragma endregion

	#pragma region window stuff

		#pragma region imgui
				#if REMOVE_IMGUI == 0
				ImGui::Render();
				int display_w, display_h;
				glfwGetFramebufferSize(wind, &display_w, &display_h);
				glViewport(0, 0, display_w, display_h);
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				// Update and Render additional Platform Windows
				// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
				//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					GLFWwindow* backup_current_context = glfwGetCurrentContext();
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
					glfwMakeContextCurrent(backup_current_context);
				}
			#endif
		#pragma endregion

		if (makeTheWindowVisible)
		{
			glfwSwapBuffers(wind);
		}

		glfwPollEvents();

	#pragma endregion
	
	#pragma region input
		{

			auto fromIndexToKey = [](int i)
			{
				if (i == VK_BACK) { return 8; }

				if (i >= 'A' && i <= 'Z')
				{
					int index = i - 'A';
					return index;
				}
				else if (i >= VK_NUMPAD0 && i <= VK_NUMPAD9)
				{
					int index = i - VK_NUMPAD0 + platform::Button::NR0;
					return index;
				}

				if (i == VK_ACCEPT)
				{
					return (int)platform::Button::Enter;
				}
				else
				if (i == VK_ESCAPE)
				{
					return (int)platform::Button::Escape;
				}
				else
				if (i == VK_UP)
				{
					return (int)platform::Button::Up;
				}
				else
				if (i == VK_DOWN)
				{
					return (int)platform::Button::Down;
				}
				else
				if (i == VK_LEFT)
				{
					return (int)platform::Button::Left;
				}
				else
				if (i == VK_RIGHT)
				{
					return (int)platform::Button::Right;
				}
				else
				if (i == VK_CONTROL)
				{
					return (int)platform::Button::LeftCtrl;
				}else
				if (i == VK_TAB)
				{
					return (int)platform::Button::Tab;
				}
				
				return -1;

			};

			for (int i = 8; i < 128; i++)
			{
				int rez = GetAsyncKeyState(i);
				int key = fromIndexToKey(i);

				if (key == platform::Button::E)
				{
					if (rez != 0)
					{
						int a = 0;
					}
				}

				if (key < 0) { continue; }

				platform::internal::setButtonState(key, rez);

				if (rez & 0x01)
				{
					platform::internal::addToTypedInput(key);
				}
			}

			if (GetAsyncKeyState(VK_RBUTTON))
			{
				int a = 0;
			}

			platform::internal::setLeftMouseState(GetAsyncKeyState(VK_LBUTTON));
			platform::internal::setRightMouseState(GetAsyncKeyState(VK_RBUTTON));
						

		}
	#pragma endregion

	


	}

	closeGame();

	return 0;
	//if you want the console to stay after closing the window
	//std::cin.clear();
	//std::cin.get();
}