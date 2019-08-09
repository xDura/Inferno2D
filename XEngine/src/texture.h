#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <string>

class Texture
{
public:
	unsigned int tex_id;
	int width;
	int height;
	int numChannels;

	bool load(std::string path, bool flipVertical = true);
};
