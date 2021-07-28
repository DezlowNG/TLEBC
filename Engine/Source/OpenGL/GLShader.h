/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#pragma once

#ifndef GLSHADER_H
#define GLSHADER_H

#include <OpenGL/GLCore.h>

namespace rb
{
	class Shader
	{
	public:
		unsigned int ID;

		void use() { glUseProgram(ID); }
		void load(const char* vertexCode, const char* fragmentCode);
	private:
		void checkCompileErrors(unsigned int shader, std::string type);
	};
}

#endif