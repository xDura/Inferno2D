#pragma once
#include "texture.h"

class SpriteSheet
{
public:
	unsigned int width;
	unsigned int height;
	Texture* texture;
	char* name;

	void Setup(int a_width, int a_height, Texture* a_tex)
	{
		width = a_width;
		height = a_height;
		texture = a_tex;
	}

	void Delete()
	{
		free(name);
	}
};