#pragma once
#include <vector>
#include "sprite_sheet.h"

class Tilemap
{
public:
	SpriteSheet* spriteSheet;
	int width;
	int height;
	int tileHeight;
	int tileWidth;
	std::vector<int> tileValues;
	char* path;

	std::string ValuesToString() const;
	void GetValues(const std::string& s);

	void SaveXML(const char* a_path) const;
	void LoadXML(const char* a_path);
	void Delete();
};