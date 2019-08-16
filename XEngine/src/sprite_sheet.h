#pragma once
#include "texture.h"

class SpriteSheet
{
public:
	unsigned int width;
	unsigned int height;
	Texture* texture;
	char* path;

	void Setup(int a_width, int a_height, Texture* a_tex);
	void Delete();
	void SaveXML(const char* a_path);
	void LoadXML(const char* a_path);
};