/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#pragma once

#ifndef DIALOGUEBOX_H
#define DIALOGUEBOX_H

#include <OpenGL/GLShader.h>
#include <VisualNovel/Text.h>
#include <OpenGL/GLCore.h>
#include <OpenGL/GLTexture.h>
#include <OpenGL/GLTextRenderer.h>

namespace rb
{
	class DialogueBox
	{
	public:
		DialogueBox() {}
		~DialogueBox() {}

		void Init(std::string boxPath, std::string fontPath, int width, int height);
		void Render(std::string name, std::string text);
		void SetBox(std::string boxPath);

		float posX = 25.0f, posY = 25.0f;
		float namePosX = 25.0f, namePosY = 25.0f;
		float textPosX = 90.0f, textPosY = 25.0f;
	private:
		Shader mBoxShader;
		Shader mTextShader;
		Texture mBoxTexture;

		unsigned int mVAO, mVBO, mEBO;
		Text mNameText;
		Text mText;
	};
}

#endif