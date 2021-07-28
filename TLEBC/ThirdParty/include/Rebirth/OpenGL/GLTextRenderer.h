/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#pragma once

#ifndef GLTEXTRENDERER_H
#define GLTEXTRENDERER_H

#include <map>

#include <OpenGL/GLCore.h>
#include <OpenGL/GLShader.h>

#include <freetype/ft2build.h>
#include FT_FREETYPE_H

static struct Character {
	unsigned int TextureID;
	glm::ivec2   Size;
	glm::ivec2   Bearing;
	unsigned int Advance;
};

namespace rb
{
	class TextRenderer
	{
	public:
		void Init(unsigned int* VAO, unsigned int* VBO, std::string fontPath, unsigned int size = 28);
		void Render(std::string text, unsigned int* VAO, unsigned int* VBO, int width, int height, float x, float y, float scale, glm::vec3 color);

		void setShader(Shader* shader) { mShader = shader; }

		float xpostemp;
		float ypostemp;
	private:
		std::map<char, Character> mCharacters;
		Shader* mShader;

		bool mRenderBold = false;
	};
}

#endif