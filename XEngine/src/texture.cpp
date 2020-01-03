#include "texture.h"
#include "External/STB/stb_image.h"
#include "opengl_defines.h"
#include "platform.h"
#include "utils.h"

bool Texture::Load(const char* a_path, bool flipVertical)
{
	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	//TODO: set this flags in the file so that we can load
	//the parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(flipVertical);

	std::string fullpath = Path::GetPath(a_path);
	LOG("loading texture from: %s", fullpath.c_str());
	unsigned char *data = stbi_load(fullpath.c_str(), &width, &height, &numChannels, 0);
	if (data)
	{

		path = copySTR(a_path);
		//@TODO: modify the glTexImage2D acording to the numChannels
		if (numChannels > 3)
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
		ASSERT(false);
		return false;
	}
}

void Texture::Destroy()
{
	free(path);
	glDeleteTextures(1, &tex_id);
}
