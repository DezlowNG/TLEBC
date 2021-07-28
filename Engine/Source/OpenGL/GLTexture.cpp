/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#include "GLTexture.h"

unsigned int loadTexture(std::string path, int* width, int* height)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int nrChannels;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* data = stbi_load(path.c_str(), &*width, &*height, &nrChannels, 0);
	if (data)
	{
		switch (nrChannels)
		{
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		default:
			break;
		}
		glGenerateMipmap(texture);
	}
	else
	{
		spdlog::get("log")->error("Failed to load texture in " + path + " path!");
		return 0;
	}
	stbi_image_free(data);

	return texture;
}

namespace rb
{
	void Texture::Load(std::string path)
	{
		if (ID > 0)
		{
			glDeleteTextures(1, &ID);
		}
		ID = loadTexture(path, &width, &height);
	}
}