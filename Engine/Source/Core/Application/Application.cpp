/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

const char* vTextShaderCode = { "#version 330 core\n"
	"layout(location = 0) in vec4 vertex;\n"
	"out vec2 TexCoords;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
	"	TexCoords = vertex.zw;\n"
	"}\n"
};

const char* fTextShaderCode = { "#version 330 core\n"
	"in vec2 TexCoords;\n"
	"out vec4 color;\n"
	"uniform sampler2D text;\n"
	"uniform vec3 textColor;\n"
	"void main()\n"
	"{\n"
	"	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
	"	color = vec4(textColor, 1.0) * sampled;\n"
	"}\n"
};

#include <fstream>

static unsigned int it = 0;
static unsigned int it2 = 0;

static bool IsLoadSave = false;
static bool NextState = false;
static bool RenderEscMenu = false;
static bool renderButtons = true;
static bool DebugInfo = false;

extern int WNDwidth, WNDheight;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
		{
			if (RenderEscMenu)
			{
				RenderEscMenu = false;
				break;
			}
			else
			{
				RenderEscMenu = true;
				break;
			}
		}
		renderButtons = true;
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS)
		{
			NextState = true;
			break;
		}
	case GLFW_KEY_D:
		if (action == GLFW_PRESS)
		{
			if (DebugInfo)
			{
				DebugInfo = false;
				break;
			}
			else
			{
				DebugInfo = true;
				break;
			}
		}
	default:
		break;
	}
}

static bool SaveProgress(std::string path, int slot)
{
	std::fstream file(path + "save.data" + std::to_string(slot), std::ios_base::out | std::ios_base::trunc);

	if (!file.is_open())
		return false;

	file << it;

	file.close();

	return true;
}

static bool LoadProgress(std::string path, int slot)
{
	std::fstream file(path + "save.data" + std::to_string(slot), std::ios::in);

	if (!file.is_open())
		return false;

	char buff[50];
	file.read(buff, 50);
	it = std::atoi(buff);
	IsLoadSave = true;

	file.close();

	return true;
}

namespace rb
{
	void Application::Run()
	{
		Init();
		Label();

		AppInit();
		AppRender();
		AppClose();
	}

	void Application::AppInit()
	{
		mLogger.Init(settings.GameDir);
		InitWindow();
		mMainScene.Init();
		mMenu.Init(settings.FontDir, window.Width, window.Height);
		mEscSprite.Init();
		mEscSprite.Load(settings.ImagesDir + "ui/exitmenu.png");
		mDialogueBox.Init(settings.ImagesDir + "ui/textbox_blood.png", settings.FontDir, window.Width, window.Height);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		mDialogueBox.namePosX = 170.0f;
		mDialogueBox.namePosY = 227.0f;

		mDialogueBox.textPosX = 167.0f;
		mDialogueBox.textPosY = 192.0f;

		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");

		ImFontConfig font_config;
		font_config.OversampleH = 1;
		font_config.OversampleV = 1;
		font_config.PixelSnapH = 1;

		static const ImWchar ranges[] =
		{
			0x0020, 0x00FF,
			0x0400, 0x044F,
			0,
		};
		io.Fonts->AddFontFromFileTTF(settings.FontDir.c_str(), 13.5f, &font_config, ranges);
		io.IniFilename = nullptr;

		StartButton = mMenu.CreateTextButton();
		LoadSaveButton = mMenu.CreateTextButton();
		SaveButton = mMenu.CreateTextButton();
		ExitButton = mMenu.CreateTextButton();
		SaveSlotButton1 = mMenu.CreateTextButton();
		SaveSlotButton2 = mMenu.CreateTextButton();
		SaveSlotButton3 = mMenu.CreateTextButton();
		SaveSlotButton4 = mMenu.CreateTextButton();
		SaveSlotButton5 = mMenu.CreateTextButton();
		ESCImage = mMenu.CreateImage(settings.ImagesDir + "ui/exitmenu.png");

		mSound.SetVolume(0.25f);
		mMusic.SetVolume(0.25f);

		if (settings.MenuBackgroundDir == "NULL" || settings.MenuMusicDir == "NULL")
			mDrawStartMenu = false;
	}

	void Application::AppRender()
	{
		int w = 1600, h = 900;
		double xpos, ypos;

		if (mDrawStartMenu)
		{
			std::string tempstr = settings.SoundsDir + settings.MenuMusicDir;
			mMusic.play(tempstr.c_str());
			mMainScene.Load(settings.ImagesDir + settings.MenuBackgroundDir);
		}
		else
		{
			NextStatement();
		}

		while (!glfwWindowShouldClose(mWindow))
		{
			glfwGetWindowSize(mWindow, &w, &h);

			glViewport(0, 0, w, h);

			WNDwidth = w;
			WNDheight = h;

			glClear(GL_COLOR_BUFFER_BIT);
			glfwGetCursorPos(mWindow, &xpos, &ypos);

			mMusic.update();

			if (mDrawStartMenu)
			{
				RenderMenu();
			}
			else
			{
				if (NextState) NextStatement(); NextState = false;

				mMainScene.Render();

				if (!mMainScene.mDissolve && !mMainScene.mIsStart)
				{
					mSpriteIt = mSprites.begin();
					for (; mSpriteIt != mSprites.end(); mSpriteIt++)
					{
						mSpriteIt->second.Render();
					}

					mDialogueBox.Render(mRenderContent.name, mRenderContent.text);

					if (IsLoadSave)
					{
						NextStatement();
					}
				}

				mMenu.SetMousePos(xpos, ypos);

				if (RenderEscMenu)
				{
					glfwSetMouseButtonCallback(mWindow, nullptr);
					RenderEscapeMenu();
				}
				else
				{
					glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods)
						{
							if (action == GLFW_PRESS & button == GLFW_MOUSE_BUTTON_LEFT)
							{
								NextState = true;
							}
						});
				}
			}

			if (DebugInfo)
			{
				ImGuiIO& io = ImGui::GetIO(); (void)io;

				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				if (ImGui::Begin("Debug Info"))
				{
					ImGui::Text("FPS: %.f", io.Framerate);
					ImGui::Text("Iterator: %i", it);

					ImGui::End();
				}

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}

			glfwPollEvents();
			glfwSwapBuffers(mWindow);
		}
	}

	void Application::AppClose()
	{
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}

	bool drawbuttons = true;

	void Application::RenderMenu()
	{
	st: {}
		double xpos, ypos;

		glfwGetCursorPos(mWindow, &xpos, &ypos);

		mMainScene.Render();

		if (!mMainScene.mDissolve && !mStartGame)
		{
			if (drawbuttons)
			{
				if (mMenu.RenderTextButton(StartButton, "Начать", WNDwidth/2.135, WNDheight/1.835, mWindow))
				{
					mStartGame = true;
					mMusic.stop();
					mMainScene.Dissolve();
				}

				if (mMenu.RenderTextButton(LoadSaveButton, "Загрузить", WNDwidth/2.2, WNDheight/2.05, mWindow))
				{
					while (true)
					{
						glClear(GL_COLOR_BUFFER_BIT);
						glfwGetCursorPos(mWindow, &xpos, &ypos);
						mMenu.SetMousePos(xpos, ypos);

						mMainScene.Render();

						if (mMenu.RenderTextButton(SaveSlotButton1, "Слот 1", WNDwidth/2.125, WNDheight/1.59, mWindow)) {
							if (LoadProgress(settings.GameDir, 1))
							{
								mDrawStartMenu = false;
								break;
							}
						}

						if (mMenu.RenderTextButton(SaveSlotButton2, "Слот 2", WNDwidth / 2.125, WNDheight /1.7, mWindow)) {
							if (LoadProgress(settings.GameDir, 2))
							{
								mDrawStartMenu = false;
								break;
							}
						}

						if (mMenu.RenderTextButton(SaveSlotButton3, "Слот 3", WNDwidth / 2.125, WNDheight / 1.83, mWindow)) {
							if (LoadProgress(settings.GameDir, 3))
							{
								mDrawStartMenu = false;
								break;
							}
						}

						if (mMenu.RenderTextButton(SaveSlotButton4, "Слот 4", WNDwidth / 2.125, WNDheight / 1.99, mWindow)) {
							if (LoadProgress(settings.GameDir, 4))
							{
								mDrawStartMenu = false;
								break;
							}
						}

						if (mMenu.RenderTextButton(SaveSlotButton5, "Слот 5", WNDwidth / 2.125, WNDheight / 2.19, mWindow)) {
							if (LoadProgress(settings.GameDir, 5))
							{
								mDrawStartMenu = false;
								break;
							}
						}

						if (mMenu.RenderTextButton(ReturnButton, "Назад", WNDwidth/2.1, WNDheight/2.5, mWindow)) {
							goto st;
						}

						if (glfwWindowShouldClose(mWindow))
						{
							break;
						}

						glfwPollEvents();
						glfwSwapBuffers(mWindow);
					}
					mMusic.stop();
					glfwSetKeyCallback(mWindow, key_callback);
				}

				if (mMenu.RenderTextButton(ExitButton, "Выход", WNDwidth/2.115, WNDheight/2.325, mWindow))
				{
					mCloseWindow = true;
					mMusic.stop();
					mMainScene.Dissolve();
				}
			}
		}

		mMenu.SetMousePos(xpos, ypos);

		if (mCloseWindow && !mMainScene.mDissolve)
		{
			drawbuttons = false;
			if (!mMusic.isPlaying)
			{
				glfwSetWindowShouldClose(mWindow, true);
			}
		}

		if (mStartGame && !mMainScene.mDissolve)
		{
			drawbuttons = false;
			if (!mMusic.isPlaying)
			{
				mDrawStartMenu = false;
				glfwSetKeyCallback(mWindow, key_callback);

				NextStatement();
			}
		}
	}

	void Application::RenderEscapeMenu()
	{
		mMenu.SetRenderImagePos(WNDwidth/139500.0f, -0.176);
		mMenu.RenderImage(ESCImage);

		if (renderButtons)
		{
			if (mMenu.RenderTextButton(LoadSaveButton, "Сохранить", WNDwidth/2.19, WNDheight/1.73, mWindow))
			{
				renderButtons = false;
				isLoadSave = false;

				return;
			}

			if (mMenu.RenderTextButton(SaveButton, "Загрузить", WNDwidth/2.19, WNDheight / 1.94, mWindow))
			{
				renderButtons = false;
				isLoadSave = true;

				return;
			}

			if (mMenu.RenderTextButton(ReturnButton3, "Назад", WNDwidth/2.11, WNDheight / 2.21, mWindow))
			{
				RenderEscMenu = false;
			}

			if (mMenu.RenderTextButton(ExitButton, "Выйти", WNDwidth/2.11, WNDheight/2.52, mWindow))
			{
				mMainScene.Dissolve();
				mMusic.stop();

				while (mMainScene.mDissolve && mMusic.isPlaying)
				{
					glClear(GL_COLOR_BUFFER_BIT);

					mMainScene.Render();
					mMusic.update();

					glfwPollEvents();
					glfwSwapBuffers(mWindow);
				}
				glfwSetWindowShouldClose(mWindow, true);
			}
		}
		else
		{
			if (isLoadSave)
			{
				if (mMenu.RenderTextButton(SaveSlotButton1, "Слот 1", WNDwidth/ 2.12, WNDheight/1.64, mWindow)) {
					if (LoadProgress(settings.GameDir, 1))
						RenderEscMenu = false;
				}

				if (mMenu.RenderTextButton(SaveSlotButton2, "Слот 2", WNDwidth / 2.12, WNDheight / 1.78, mWindow)) {
					if (LoadProgress(settings.GameDir, 2))
						RenderEscMenu = false;
				}

				if (mMenu.RenderTextButton(SaveSlotButton3, "Слот 3", WNDwidth / 2.12, WNDheight / 1.96, mWindow)) {
					if (LoadProgress(settings.GameDir, 3))
						RenderEscMenu = false;
				}

				if (mMenu.RenderTextButton(SaveSlotButton4, "Слот 4", WNDwidth / 2.12, WNDheight / 2.17, mWindow)) {
					if (LoadProgress(settings.GameDir, 4))
						RenderEscMenu = false;
				}

				if (mMenu.RenderTextButton(SaveSlotButton5, "Слот 5", WNDwidth / 2.12, WNDheight / 2.41, mWindow)) {
					if (LoadProgress(settings.GameDir, 5))
						RenderEscMenu = false;
				}

				if (mMenu.RenderTextButton(ReturnButton, "Назад", WNDwidth/2.11, WNDheight/2.75, mWindow)) {
					renderButtons = true;
				}
			}
			else
			{
				if (mMenu.RenderTextButton(SaveSlotButton1, "Слот 1", WNDwidth / 2.12, WNDheight / 1.64, mWindow)) {
					SaveProgress(settings.GameDir, 1);
				}

				if (mMenu.RenderTextButton(SaveSlotButton2, "Слот 2", WNDwidth / 2.12, WNDheight / 1.78, mWindow)) {
					SaveProgress(settings.GameDir, 2);
				}

				if (mMenu.RenderTextButton(SaveSlotButton3, "Слот 3", WNDwidth / 2.12, WNDheight / 1.96, mWindow)) {
					SaveProgress(settings.GameDir, 3);
				}

				if (mMenu.RenderTextButton(SaveSlotButton4, "Слот 4", WNDwidth / 2.12, WNDheight / 2.17, mWindow)) {
					SaveProgress(settings.GameDir, 4);
				}

				if (mMenu.RenderTextButton(SaveSlotButton5, "Слот 5", WNDwidth / 2.12, WNDheight / 2.41, mWindow)) {
					SaveProgress(settings.GameDir, 5);
				}

				if (mMenu.RenderTextButton(ReturnButton2, "Назад", WNDwidth / 2.11, WNDheight / 2.75, mWindow)) {
					renderButtons = true;
				}
			}
		}
	}

	void Application::NextStatement()
	{
		if (IsLoadSave)
		{
			if (it2 != it)
			{
				mSprites.clear();
				int temp = it;
				bool text = true;
				std::vector<std::string> spritesToDelete;
				std::vector<std::string> spritesToDisplay;
				bool soundStop = false;
				bool ambStop = false;
				IsLoadSave = false;
				temp--;
				bool t = false;
				int tit = 0;
				while (true)
				{
					if (temp < 0)
						break;

					switch (mCommandList[temp].command)
					{
					case CommandEnum::TEXT:
						if (text)
						{
							text = false;
							it = temp;
						}
						break;
					case CommandEnum::SCENE:
						if (!t)
						{
							mMainScene.Load(settings.ImagesDir + mCommandList[temp].content);
							t = true;
						}
					case CommandEnum::SHOWSPRITE:
					{
						if (!t)
						{
							spritesToDisplay.push_back(mCommandList[temp].content);
							Sprite tempSprite;
							tempSprite.Init();
							if (!spritesToDelete.empty())
							{
								for (size_t i = 0; i < spritesToDisplay.size(); i++)
								{
									if (i < spritesToDelete.size())
									{
										if (spritesToDelete[i] != spritesToDisplay[i + 1] && spritesToDelete[i] != spritesToDisplay[i + 2])
										{
											tempSprite.Load(settings.ImagesDir + mCommandList[temp].content);
											tempSprite.SetPosition(mCommandList[temp].posX, mCommandList[temp].poxY);
											mSprites[mCommandList[temp].content] = tempSprite;
										}
									}
									else
									{
										break;
									}
								}
							}
							else
							{
								tempSprite.Load(settings.ImagesDir + mCommandList[temp].content);
								tempSprite.SetPosition(mCommandList[temp].posX, mCommandList[temp].poxY);
								mSprites[mCommandList[temp].content] = tempSprite;
							}
							break;
						}
					}
					case CommandEnum::HIDESPRITE:
						spritesToDelete.push_back(mCommandList[temp].content);
						break;
					case CommandEnum::PLAYMUSIC:
						if (!soundStop)
						{
							mMusic.SoundEngine->stopAllSounds();
							std::string t = settings.SoundsDir + mCommandList[temp].content;
							mMusic.play(t.c_str());
						}
						break;
					case CommandEnum::PLAYSOUND:
						break;
					case CommandEnum::STOPMUSIC:
						mMusic.SoundEngine->stopAllSounds();
						soundStop = true;
						break;
					case CommandEnum::CHANGEBOX:
						mDialogueBox.SetBox(settings.ImagesDir + mCommandList[temp].content);
						goto end;
						break;
					case CommandEnum::CHANGEESCMENU:
						mMenu.ChangeImage(ESCImage, settings.ImagesDir + mCommandList[temp].content);
						break;
					case CommandEnum::PLAYAMBIENCE:
						if (!ambStop)
						{
							mAmbience.SoundEngine->stopAllSounds();
							std::string t = settings.SoundsDir + mCommandList[temp].content;
							mAmbience.play(t.c_str());
						}
						break;
					case CommandEnum::STOPAMBIENCE:
						ambStop = true;
						break;
					default:
						break;
					}
					temp--;
				}
			}
			else
			{
				IsLoadSave = false;
				return;
			}
		}

	end: {}

		if (it == mCommandList.size())
			return;

	start: {}

		switch (mCommandList[it].command)
		{
		case CommandEnum::TEXT:
			if (mCommandList[it].who != "")
			{
				mRenderContent.name = mCommandList[it].who;
				mRenderContent.text = mCommandList[it].what;
			}
			else
			{
				mRenderContent.name = "";
				mRenderContent.text = mCommandList[it].what;
			}
			break;
		case CommandEnum::SCENE:
			mMainScene.Load(settings.ImagesDir + mCommandList[it].content);
			mSprites.clear();
			it++;
			it2++;
			goto start;
		case CommandEnum::SHOWSPRITE:
		{
			Sprite tempSprite;
			tempSprite.Init();
			tempSprite.Load(settings.ImagesDir + mCommandList[it].content);
			tempSprite.SetPosition(mCommandList[it].posX, mCommandList[it].poxY);
			mSprites[mCommandList[it].content] = tempSprite;
			it++;
			it2++;
			goto start;
		}
		case CommandEnum::HIDESPRITE:
			mSprites.erase(mSprites.find(mCommandList[it].content));
			it++;
			it2++;
			goto start;
		case CommandEnum::PLAYMUSIC:
		{
			std::string t = settings.SoundsDir + mCommandList[it].content;
			mMusic.play(t.c_str());
			it++;
			it2++;
			goto start;
		}
		case CommandEnum::STOPMUSIC:
			mMusic.stop();
			it++;
			it2++;
			goto start;
		case CommandEnum::PLAYSOUND:
		{
			std::string t = settings.SoundsDir + mCommandList[it].content;
			mSound.play(t.c_str());
			it++;
			it2++;
			goto start;
		}
		case CommandEnum::STOPSOUND:
			mSound.stop();
			it++;
			it2++;
			goto start;
		case CommandEnum::CHANGEBOX:
			mDialogueBox.SetBox(settings.ImagesDir + mCommandList[it].content);
			it++;
			it2++;
			goto start;
		case CommandEnum::PLAYAMBIENCE:
		{
			std::string t = settings.SoundsDir + mCommandList[it].content;
			mAmbience.play(t.c_str());
			it++;
			it2++;
			goto start;
		}
		case CommandEnum::STOPAMBIENCE:
			mAmbience.stop();
			it++;
			it2++;
			goto start;
		case CommandEnum::CHANGEESCMENU:
			mMenu.ChangeImage(ESCImage, settings.ImagesDir + mCommandList[it].content);
			it++;
			it2++;
			goto start;
		case CommandEnum::RETURNTOMENU:
			mDrawStartMenu = true;
			mStartGame = false;
			drawbuttons = true;
			it = 0;
			it2 = 0;
			mMainScene.Load(settings.ImagesDir + "ui/menu.png");
			mMusic.play("game/sounds/music/Azimuth.ogg");
			glfwSetMouseButtonCallback(mWindow, nullptr);
			goto endsw;
		default:
			break;
		}
		it++;
		it2++;
	endsw: {}
	}

	void Application::InitWindow()
	{
		WindowsWindow tempWindow(&window);
		mWindow = tempWindow.GetWindow();
	}

	void Application::AppInput()
	{
	}

	void Application::text(std::string who, std::string what)
	{
		mCommandList.push_back({ CommandEnum::TEXT, " ", who, what, 0.0f, 0.0f });
	}
	void Application::text(std::string what)
	{
		mCommandList.push_back({ CommandEnum::TEXT,  " ",  " ", what, 0.0f, 0.0f });
	}
	void Application::scene(std::string path, bool dissolve)
	{
		mCommandList.push_back({ CommandEnum::SCENE, path,  " ",  " ", 0.0f, 0.0f });
	}
	void Application::showSprite(std::string path, float x, float y)
	{
		mCommandList.push_back({ CommandEnum::SHOWSPRITE, path,  " ",  " ", x, y });
	}
	void Application::hideSprite(std::string path)
	{
		mCommandList.push_back({ CommandEnum::HIDESPRITE, path,  " ",  " ", 0.0f, 0.0f });
	}
	void Application::playMusic(std::string path)
	{
		mCommandList.push_back({ CommandEnum::PLAYMUSIC, path,  " ",  " ", 0.0f, 0.0f });
	}
	void Application::stopMusic()
	{
		mCommandList.push_back({ CommandEnum::STOPMUSIC,  " ",  " ",  " ", 0.0f, 0.0f });
	}
	void Application::playSound(std::string path)
	{
		mCommandList.push_back({ CommandEnum::PLAYSOUND, path,  " ",  " ", 0.0f, 0.0f });
	}
	void Application::stopSound()
	{
		mCommandList.push_back({ CommandEnum::STOPSOUND,  " ",  " ",  " ", 0.0f, 0.0f });
	}
	void Application::playAmbience(std::string path)
	{
		mCommandList.push_back({ CommandEnum::PLAYAMBIENCE, path,  " ",  " ", 0.0f, 0.0f });
	}
	void Application::stopAmbience()
	{
		mCommandList.push_back({ CommandEnum::STOPAMBIENCE,  " ",  " ",  " ", 0.0f, 0.0f });
	}
	void Application::changeBox(std::string path)
	{
		mCommandList.push_back({ CommandEnum::CHANGEBOX, path, " ", " ", 0.0f, 0.0f });
	}
	void Application::changeESCMenu(std::string path)
	{
		mCommandList.push_back({ CommandEnum::CHANGEESCMENU, path, " ", " ", 0.0f, 0.0f });
	}
	void Application::returnToMenu()
	{
		mCommandList.push_back({ CommandEnum::RETURNTOMENU, " ", " ", " ", 0.0f, 0.0f });
	}
}