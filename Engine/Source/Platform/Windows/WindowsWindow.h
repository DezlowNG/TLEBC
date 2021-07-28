/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#pragma once

#ifndef WINDOWSWINDOW_H
#define WINDOWSWINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct WindowProps
{
	unsigned int Width;
	unsigned int Height;
	std::string Title;

	WindowProps(const unsigned int width = 1600,
		const unsigned int height = 900,
		const std::string& title = "Visual Novel") : Width(width), Height(height), Title(title)
	{
	}
};

namespace rb
{
	class WindowsWindow
	{
	public:
		WindowsWindow(WindowProps* props);

		GLFWwindow* GetWindow() { return m_Window; }
	private:
		void Init(WindowProps* props);

		GLFWwindow* m_Window;
		WindowProps m_Props;
	};
}

#endif