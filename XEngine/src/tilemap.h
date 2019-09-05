#pragma once
#include <vector>
#include "sprite_sheet.h"

class Tilemap
{
public:
	//TODO: add support for animated tiles inside tilemaps
	//using AnimatedSprite
	SpriteSheet* spriteSheet;
	int width;
	int height;
	int tileHeight;
	int tileWidth;
	std::vector<int> tileValues;
	char* path;

	std::string ValuesToString();
	void GetValues(const std::string& s);

	void SaveXML(const char* a_path);
	void LoadXML(const char* a_path);
	void Delete();
};