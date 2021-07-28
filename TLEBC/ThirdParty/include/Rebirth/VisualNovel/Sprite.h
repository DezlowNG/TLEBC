/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#pragma once

#ifndef SPRITE_H
#define SPRITE_H

#include <OpenGL/GLCore.h>

#include <OpenGL/GLTexture.h>
#include <OpenGL/GLShader.h>

namespace rb
{
	class Sprite
	{
	public:
		Sprite() {}
		~Sprite() {}

		void Init();

		void Render();
		void Load(std::string imagePath);
		void SetPosition(double x, double y);
	private:
		Texture mSpriteTexture;
		Shader mSpriteShader;

		double mPosX, mPosY;
		double mVX, mVY;

		glm::mat4 view = glm::mat4(1.0f);

		unsigned int mVAO, mVBO, mEBO;
	};
}

#endif