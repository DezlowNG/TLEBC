/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#pragma once

#ifndef GLTEXTURE_H
#define GL_TEXTURE_H

#include <OpenGL/GLCore.h>

#include <stb/stb_image.h>

namespace rb
{
	class Texture
	{
	public:
		Texture() {}
		~Texture() {}

		void Load(std::string path);
		inline unsigned int GetID() const { return ID; }

		int width, height;
	private:
		unsigned int ID = 0;
	};
}

#endif