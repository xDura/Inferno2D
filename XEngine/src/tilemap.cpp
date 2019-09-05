#pragma once
#include "tilemap.h"
#include "platform.h"
#include "debug.h"
#include "External/tinyxml2.h"
#include "asset_manager.h"

std::string Tilemap::ValuesToString()
{
	std::stringstream ss;
	ss << std::endl;
	for (int i = 0; i < tileHeight; i++)
	{
		for (int j = 0; j < tileWidth; j++)
		{
			size_t desiredIndex = size_t(i * j + j);
			ss << tileValues[desiredIndex];
			if (j < tileWidth) ss << " ";
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
	spriteSheetNode->SetAttribute("SpriteSheet", spriteSheet->path);
	//TODO: investigate other way to serialize the int array
	spriteSheetNode->SetText(ValuesToString().c_str());
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
	std::string fullpath = Path::GetPath(a_path);
	FILE* file;
	fopen_s(&file, fullpath.c_str(), "rb");
	ASSERT(file != NULL);

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile(file);
	if (err != 0)
	{
		LOG("Error reading Tilemap from XML at %s", a_path);
		ASSERT(false);
	}

	tinyxml2::XMLNode* versionNode = doc.FirstChild();
	//TODO: check versions

	tinyxml2::XMLNode* tilemapNode = versionNode->NextSibling();
	tinyxml2::XMLElement* tilemapElem = tilemapNode->ToElement();
	tilemapElem->QueryIntAttribute("width", &width);
	tilemapElem->QueryIntAttribute("height", &height);
	tilemapElem->QueryIntAttribute("tileWidth", &tileWidth);
	tilemapElem->QueryIntAttribute("tileHeight", &tileHeight);
	const char* spritesheet_path;
	tilemapElem->QueryStringAttribute("SpriteSheet", &spritesheet_path);

	size_t newSize = size_t(tileWidth) * size_t(tileHeight);
	tileValues.resize(newSize);
	const char* tilemapValues = tilemapElem->GetText();

	std::istringstream ss(tilemapValues);
	char endl = '0';
	for (unsigned int i = 0; i < tileHeight; i++)
	{
		for (unsigned int j = 0; j < tileWidth; j++)
		{
			size_t desiredIndex = size_t(i * j + j);
			ss >> tileValues[desiredIndex];
		}
		ss >> endl;
	}
	
	spriteSheet = AssetManager::GetSpriteSheet(spritesheet_path);
}

void Tilemap::Delete()
{
	spriteSheet = NULL;
	free(path);
}
