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
	char* path;

	bool Load(const char* a_path, bool flipVertical = true);
	void Destroy();
};
