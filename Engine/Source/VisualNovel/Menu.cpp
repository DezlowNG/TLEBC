/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#include "Menu.h"

const char* vvvTextShaderCode = {
	"#version 330 core\n"
	"layout(location = 0) in vec4 vertex;\n"
	"out vec2 TexCoords;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
	"	TexCoords = vertex.zw;\n"
	"}\n"
};

const char* fffTextShaderCode = { "#version 330 core\n"
	"in vec2 TexCoords;\n"
	"out vec4 color;\n"
	"uniform sampler2D text;\n"
	"uniform vec3 textColor;\n"
	"void main()\n"
	"{\n"
	"	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
	"	color = vec4(textColor, 1.0) * sampled;\n"
	"}\n"
};

const char* vImageShaderCode = { "#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 2) in vec3 aColor;\n"
		"layout(location = 2) in vec2 aTexCoord;\n"
		"\n"
		"out vec2 TexCoord;\n"
		"out vec3 ourColor;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos, 1.0);\n"
		"	TexCoord = aTexCoord;\n"
		"	ourColor = aColor;\n"
		"}\n"
};

const char* fImageShaderCode = { "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 TexCoord;\n"
		"uniform sampler2D ourTexture;\n"
		"void main()\n"
		"{\n"
		"	vec4 texColor = texture(ourTexture, TexCoord);\n"
		"	if(texColor.a < 0.1)\n"
		"		discard;\n"
		"	FragColor = texColor;\n"
		"}\n"
};

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <OpenGL/GLUtils.h>



extern int WNDwidth, WNDheight;

namespace rb
{
	void Menu::Init(std::string fontPath, int width, int height)
	{
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WNDwidth), 0.0f, static_cast<float>(WNDheight));

		mTextShader.load(vvvTextShaderCode, fffTextShaderCode);

		mTextShader.use();
		glUniformMatrix4fv(glGetUniformLocation(mTextShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		this->fontPath = fontPath;

		float vertices[] = {
			   -1.0f, -1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
				1.0f, -1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
				1.0f,  1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
			   -1.0f,  1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f
		};

		unsigned int indices[] = {
					0, 1, 2,
					2, 3, 0
		};

		glGenVertexArrays(1, &mVAO);
		glGenBuffers(1, &mVBO);
		glGenBuffers(1, &mEBO);

		glBindVertexArray(mVAO);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	int Menu::CreateTextButton()
	{
		TextRenderer t;
		t.setShader(&mTextShader);
		t.Init(&mTextVAO, &mTextVBO, fontPath, 35);

		mTextButtonList.push_back(t);

		return mTextButtonList.size() - 1;
	}

	int Menu::CreateImage(std::string path)
	{
		Shader s;
		s.load(vImageShaderCode, fImageShaderCode);
		mImageShaderList.push_back(s);
		Texture t;
		t.Load(path);
		mImageTextureList.push_back(t);

		return mImageTextureList.size() - 1;
	}

	bool Menu::RenderTextButton(int id, std::string text, double x, double y, GLFWwindow* window)
	{
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WNDwidth), 0.0f, static_cast<float>(WNDheight));

		mTextShader.use();
		glUniformMatrix4fv(glGetUniformLocation(mTextShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		static int oldState = GLFW_RELEASE;
		int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (mPosMouseX >= x && mPosMouseX <= mTextButtonList[id].xpostemp && mPosMouseY >= mTextButtonList[id].ypostemp - 8 && mPosMouseY <= mTextButtonList[id].ypostemp + 20)
		{
			if (newState == GLFW_RELEASE && oldState == GLFW_PRESS)
			{
				oldState = newState;
				return true;
			}
			else
			{
				mTextButtonList[id].Render(text, &mTextVAO, &mTextVBO, -1, -1, x, y, 1.0f, glm::vec3(1.0f, 0.5f, 1.0f));
				oldState = newState;
				return false;
			}
		}
		else
		{
			mTextButtonList[id].Render(text, &mTextVAO, &mTextVBO, -1, -1, x, y, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
			return false;
		}
	}

	void Menu::RenderImage(int id)
	{
		mImageShaderList[id].use();
		/*glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)WNDwidth / (float)WNDheight, 0.1f, 100.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

		glUniformMatrix4fv(glGetUniformLocation(mImageShaderList[id].ID, "proj"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(mImageShaderList[id].ID, "view"), 1, GL_FALSE, glm::value_ptr(view));*/

		glBindVertexArray(mVAO);

		glBindTexture(GL_TEXTURE_2D, mImageTextureList[id].GetID());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	void Menu::SetRenderImagePos(double x, double y)
	{
		float vertices[] = {
				  -1.0f + x, -1.0f + y, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
				   1.0f + x, -1.0f + y, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
				   1.0f + x,  1.0f + y, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
				  -1.0f + x,  1.0f + y, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f
		};

		unsigned int indices[] = {
					0, 1, 2,
					2, 3, 0
		};

		glGenVertexArrays(1, &mVAO);
		glGenBuffers(1, &mVBO);
		glGenBuffers(1, &mEBO);

		glBindVertexArray(mVAO);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	void Menu::ChangeImage(int id, std::string path)
	{
		mImageTextureList[id].Load(path);
	}

	void Menu::SetMousePos(double x, double y)
	{
		mPosMouseX = x;
		mPosMouseY = WNDheight - y;
	}
}