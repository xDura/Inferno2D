#pragma once
#include "sprite_sheet.h"

class Tilemap
{
public:
	SpriteSheet* spriteSheet;
	int width;
	int height;
	int tileHeight;
	int tileWidth;
	int* values;
	char* path;

	std::string ValuesToString();
	void GetValues(const std::string& s);

	void SaveXML(const char* a_path);
	void LoadXML(const char* a_path);
	void Delete()
	{
		spriteSheet = NULL;
		free(path);
	}
};