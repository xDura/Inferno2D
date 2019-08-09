#pragma once
#include "texture.h"

class SpriteSheet
{
public:
	const char* name;
	unsigned int width;
	unsigned int height;
	Texture* texture;

	void Setup(int a_width, int a_height, Texture* a_tex)
	{
		width = a_width;
		height = a_height;
		texture = a_tex;
	}
};