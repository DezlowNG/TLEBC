/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#include "GLShader.h"

namespace rb
{
	void Shader::load(const char* vertexCode, const char* fragmentCode)
	{
		unsigned int vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "Vertex");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "Fragment");

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "Program");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Shader::checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "Program")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				throw std::runtime_error(type + " shader error: " + infoLog);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				throw std::runtime_error("Program error: " + std::string(infoLog));
			}
		}
	}
}