/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#include "Text.h"

const char* vvTextShaderCode = { "#version 330 core\n"
	"layout(location = 0) in vec4 vertex;\n"
	"out vec2 TexCoords;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
	"	TexCoords = vertex.zw;\n"
	"}\n"
};

const char* ffTextShaderCode = { "#version 330 core\n"
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

extern int WNDwidth, WNDheight;

namespace rb
{
	void Text::Init(std::string fontPath, int width, int height, int size)
	{
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));

		mTextShader.load(vvTextShaderCode, ffTextShaderCode);

		mTextShader.use();
		glUniformMatrix4fv(glGetUniformLocation(mTextShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		mTextRenderer.setShader(&mTextShader);
		mTextRenderer.Init(&VAO, &VBO, fontPath, size);
	}
	void Text::Render(std::string text, int width, int height, float x, float y, float scale, glm::vec3 color)
	{
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1600), 0.0f, static_cast<float>(900));

		mTextShader.use();
		glUniformMatrix4fv(glGetUniformLocation(mTextShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		mTextRenderer.Render(text, &VAO, &VBO, width, height, x, y, scale, color);
	}
}