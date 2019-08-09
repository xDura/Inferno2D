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
	std::string path;

	bool load(std::string a_path, bool flipVertical = true);
};
