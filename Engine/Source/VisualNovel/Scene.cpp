/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#include "Scene.h"

const char* vSceneShaderCode = { "#version 330 core\n"
	"layout(location = 0) in vec3 aPos;\n"
	"layout(location = 1) in vec3 aColor;\n"
	"layout(location = 2) in vec2 aTexCoord;\n"
	"out vec3 ourColor;\n"
	"out vec2 TexCoord;\n"
	"void main()\n"
	"{\n"
	 "	gl_Position = vec4(aPos, 1.0);\n"
	 "	ourColor = aColor;\n"
	 "	TexCoord = aTexCoord;\n"
	"}\n"
};

const char* fSceneShaderCode = { "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 ourColor;\n"
	"in vec2 TexCoord;\n"
	"\n"
	"uniform sampler2D ourTexture;\n"
	"uniform float alpha = 0.0;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(vec3(texture(ourTexture, TexCoord)), alpha);\n"
	"}\n"
};

namespace rb
{
	void Scene::Init()
	{
		float vertices[] = {
			   -1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
				1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
				1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
			   -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f
		};

		unsigned int indices[] = {
						0, 1, 2,
						2, 3, 0
		};

		mSceneShader.load(vSceneShaderCode, fSceneShaderCode);

		glGenVertexArrays(1, &mVAO);
		glGenBuffers(1, &mVBO);
		glGenBuffers(1, &mEBO);

		glBindVertexArray(mVAO);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	void Scene::Render()
	{
		while (mDissolve)
		{
			if (mIsStart)
			{
				mSceneTexture.Load(mTexturePath);
				mIsStart = false;
				mIsLoaded = false;
			}
			else if (mIsLoaded)
			{
				mSceneShader.use();
				glUniform1f(glGetUniformLocation(mSceneShader.ID, "alpha"), mTextureAlpha);
				mTextureAlpha -= 0.030;

				if (mTextureAlpha < 0.030)
				{
					mTextureAlpha = 0.0;
					mSceneTexture.Load(mTexturePath);
					mIsLoaded = false;
					break;
				}
				break;
			}
			else if (mD2)
			{
				mSceneShader.use();
				glUniform1f(glGetUniformLocation(mSceneShader.ID, "alpha"), mTextureAlpha);
				mTextureAlpha -= 0.015;

				if (mTextureAlpha < 0.015)
				{
					mTextureAlpha = 0.0;
					mD2 = false;
					mDissolve = false;
					break;
				}
				break;
			}

			mSceneShader.use();
			glUniform1f(glGetUniformLocation(mSceneShader.ID, "alpha"), mTextureAlpha);
			mTextureAlpha += 0.030;

			if (mTextureAlpha > 1.030)
			{
				mDissolve = false;
				mTextureAlpha = 1.0;
				break;
			}
			break;
		}
		
		mSceneShader.use();

		glBindTexture(GL_TEXTURE_2D, mSceneTexture.GetID());

		glBindVertexArray(mVAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}

	void Scene::Load(std::string imagePath)
	{
		mTexturePath = imagePath;
		mDissolve = true;
		mIsLoaded = true;
	}

	void Scene::Dissolve()
	{
		mDissolve = true;
		mD2 = true;
	}
}