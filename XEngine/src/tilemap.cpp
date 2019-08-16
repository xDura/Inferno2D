#pragma once
#include "tilemap.h"
#include "platform.h"
#include "debug.h"
#include "External/tinyxml2.h"

std::string Tilemap::ValuesToString()
{
	std::stringstream ss;
	for (size_t i = 0; i < tileHeight; i++)
	{
		for (size_t j = 0; j < tileWidth; j++)
		{
			ss << values[i*j + j];
			if (j > (tileWidth - 1)) ss << ",";
		}
		ss << std::endl;
	}
	std::cout << ss.str();
	return ss.str();
}

void Tilemap::GetValues(const std::string & s)
{
}

void Tilemap::SaveXML(const char* a_path)
{
	ASSERT(spriteSheet != NULL);
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* versionNode = doc.NewElement("Version");
	tinyxml2::XMLText* versionText = doc.NewText("0.0.1");
	versionNode->InsertEndChild(versionText);
	doc.InsertEndChild(versionNode);

	tinyxml2::XMLElement* spriteSheetNode = doc.NewElement("TileMap");
	spriteSheetNode->SetAttribute("width", width);
	spriteSheetNode->SetAttribute("height", height);
	spriteSheetNode->SetAttribute("tileWidth", tileWidth);
	spriteSheetNode->SetAttribute("tileHeight", tileHeight);
	//TODO: values int* (matrix of values) serialized as text
	//spriteSheetNode->SetText();
	spriteSheetNode->SetAttribute("SpriteSheet", spriteSheet->path);
	doc.InsertEndChild(spriteSheetNode);

	std::string fullpath = Path::GetPath(a_path);
	FILE* file;
	fopen_s(&file, fullpath.c_str(), "w+");
	ASSERT(file != NULL);

	doc.SaveFile(file);
	fclose(file);
}

void Tilemap::LoadXML(const char* a_path)
{

}
