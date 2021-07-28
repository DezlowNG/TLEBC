/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#pragma once

#ifndef TEXT_H
#define TEXT_H

#include <OpenGL/GLShader.h>
#include <OpenGL/GLTextRenderer.h>

namespace rb
{
	class Text
	{
	public:
		Text() {}
		~Text() {}

		void Init(std::string fontPath, int width, int height, int size = 28);
		void Render(std::string text, int width = -1, int height = -1, float x = 25.0f, float y = 25.0f, float scale = 1.0f, glm::vec3 color = glm::vec3(0.7f, 1.0f, 0.0f));
	private:
		unsigned int VAO, VBO, EBO;
		Shader mTextShader;
		TextRenderer mTextRenderer;
	};
}

#endif