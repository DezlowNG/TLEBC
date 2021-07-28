/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#include "GLTextRenderer.h"

extern int WNDwidth, WNDheight;

namespace rb
{
	void TextRenderer::Init(unsigned int* VAO, unsigned int* VBO, std::string fontPath, unsigned int size)
	{
		FT_Library ft;

		if (FT_Init_FreeType(&ft))
			throw std::runtime_error("Failed to init freetype!");

		if (fontPath.empty())
			throw std::runtime_error("FreeType: font path is empty!");

		FT_Face face;

		if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
			std::runtime_error("FreeType: failed to load font in " + fontPath + " path!");
		}
		else {
			FT_Set_Pixel_Sizes(face, 0, size);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			for (unsigned int c = 0; c < 256; c++)
			{
				if ((c >= 0xC0) && (c <= 0xDF))
				{
					if (FT_Load_Char(face, 0x0410 + (c - 0xC0), FT_LOAD_RENDER))
					{
						spdlog::get("log")->warn("FreeType: failed to load glyph!");
						continue;
					}
				}
				else if ((c >= 0xE0) && (c <= 0xFF))
				{
					if (FT_Load_Char(face, 0x0430 + (c - 0xE0), FT_LOAD_RENDER))
					{
						spdlog::get("log")->warn("FreeType: failed to load glyph!");
						continue;
					}
				}
				else if (c == 0xA8)
				{
					if (FT_Load_Char(face, 0x0401, FT_LOAD_RENDER))
					{
						spdlog::get("log")->warn("FreeType: failed to load glyph!");
						continue;
					}
				}
				else if (c == 0xB8)
				{
					if (FT_Load_Char(face, 0x0451, FT_LOAD_RENDER))
					{
						spdlog::get("log")->warn("FreeType: failed to load glyph!");
						continue;
					}
				}
				else if (c == 0x85)
				{
					if (FT_Load_Char(face, 0x2026, FT_LOAD_RENDER))
					{
						spdlog::get("log")->warn("FreeType: failed to load glyph!");
						continue;
					}
				}
				else
				{
					if (FT_Load_Char(face, c, FT_LOAD_RENDER))
					{
						spdlog::get("log")->warn("FreeType: failed to load glyph!");
						continue;
					}
				}

				unsigned int texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				Character character = {
					texture,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					static_cast<unsigned int>(face->glyph->advance.x)
				};
				mCharacters.insert(std::pair<char, Character>(c, character));
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		glGenVertexArrays(1, &*VAO);
		glGenBuffers(1, &*VBO);
		glBindVertexArray(*VAO);
		glBindBuffer(GL_ARRAY_BUFFER, *VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void TextRenderer::Render(std::string text, unsigned int* VAO, unsigned int* VBO, int width, int height, float x, float y, float scale, glm::vec3 color)
	{
		mShader->use();
		glUniform3f(glGetUniformLocation(mShader->ID, "textColor"), color.x, color.y, color.z);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(*VAO);

		std::string::iterator c;
		float xpos = 0.0f, ypos = 0.0f;
		float tempx = x;
		Character ch;
		int i = 0;
		for (c = text.begin(); c != text.end(); c++)
		{
			if ((width > 0 && height > 0) && (*c == ' ' && xpos * 1.47 >= width))
			{
				y -= 30.5;
				x = tempx;
				c++;
			}

			if (*c == '\n')
			{
				y -= 30.5;
				x = tempx;
				c++;
			}

			ch = mCharacters[*c];

			xpos = x + ch.Bearing.x * scale;
			ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;

			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};

			glBindTexture(GL_TEXTURE_2D, ch.TextureID);

			glBindBuffer(GL_ARRAY_BUFFER, *VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += (ch.Advance >> 6) * scale - 2;
			i++;
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		xpostemp = xpos;
		ypostemp = ypos;
	}
}