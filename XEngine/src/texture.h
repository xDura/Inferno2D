#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "External/stb_image.h"
#include "opengl_defines.h"
#include "platform.h"

class Texture
{
public:
	unsigned int tex_id;
	int width;
	int height;
	int numChannels;

	bool load(std::string path, bool flipVertical = true)
	{
		glGenTextures(1, &tex_id);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_set_flip_vertically_on_load(flipVertical);
		// load and generate the texture

		std::string fullpath = Path::GetPath(path);
		LOG("loading texture from: %s", fullpath.c_str());
		unsigned char *data = stbi_load(fullpath.c_str(), &width, &height, &numChannels, 0);
		if (data)
		{
			//@TODO: modify the glTexImage2D acording to the numChannels
			if(numChannels > 3)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			glGenerateMipmap(GL_TEXTURE_2D);

			LOG("load succesfull");
			//TEXTURES
			//@TODO: study if we will need this data
			//inside this class
			stbi_image_free(data);

			return true;
		}
		else
		{
			LOGERROR("Failed to load texture at: %s ", fullpath);
			return false;
		}
	}
};
