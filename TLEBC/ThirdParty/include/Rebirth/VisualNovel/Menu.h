/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#pragma once

#ifndef MENU_H
#define MENU_H

#include <OpenGL/GLTextRenderer.h>
#include <OpenGL/GLShader.h>
#include <OpenGL/GLTexture.h>

#include <GLFW/glfw3.h>

namespace rb
{
	class Menu
	{
	public:
		Menu() {}
		~Menu() {}

		void Init(std::string fontPath, int width, int height);

		int CreateTextButton();
		int CreateImage(std::string path);

		bool RenderTextButton(int id, std::string text, double x, double y, GLFWwindow* window);
		void RenderImage(int id);
		void SetRenderImagePos(double x, double y);
		void ChangeImage(int id, std::string path);

		void SetMousePos(double x, double y);
	private:
		std::vector<TextRenderer> mTextButtonList;
		std::vector<Shader> mImageShaderList;
		std::vector<Texture> mImageTextureList;

		std::string mText;
		double mPosX, mPosY;

		double mPosMouseX, mPosMouseY;

		Shader mTextShader;

		unsigned int mVAO, mVBO, mEBO;
		unsigned int mTextVAO, mTextVBO;

		std::string fontPath;
		int width, height;
	};
}

#endif