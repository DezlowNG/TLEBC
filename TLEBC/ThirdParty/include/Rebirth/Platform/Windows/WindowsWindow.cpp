/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#include "WindowsWindow.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image.h>

namespace rb
{
	WindowsWindow::WindowsWindow(WindowProps* props)
	{
		Init(props);
	}

	void WindowsWindow::Init(WindowProps* props)
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		std::string title = props->Title + " (Powered by Rebirth Engine)";

		m_Window = glfwCreateWindow(props->Width, props->Height, title.c_str(), nullptr, nullptr);

		if (m_Window == nullptr)
			throw std::runtime_error("Failed to create GLFW window!");

		glfwMakeContextCurrent(m_Window);
		
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw std::runtime_error("Failed to init GLAD!");

		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
			});

		glfwSwapInterval(1);

		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) {
			if (focused)
			{
				glfwSwapInterval(1);
			}
			else
			{
				glfwSwapInterval(-5);
			}
			});

		GLFWimage images[1];
		images[0].pixels = stbi_load("game/images/misc/icon.png", &images[0].width, &images[0].height, 0, 4);
		glfwSetWindowIcon(m_Window, 1, images);
		stbi_image_free(images[0].pixels);
	}
}