#pragma once
#include "tilemap.h"
#include "platform.h"
#include "debug.h"
#include "External/tinyxml2.h"
#include "asset_manager.h"
#include <iostream>
#include <sstream>

std::string Tilemap::ValuesToString()
{
	std::stringstream ss;
	ss << std::endl;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			unsigned int desiredIndex = (i * width) + j;
			ss << tileValues[desiredIndex];
			if (i < width) ss << " ";
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

	size_t newSize = size_t(height) * size_t(width);
	tileValues.resize(newSize);
	const char* tilemapValues = tilemapElem->GetText();

	std::istringstream ss(tilemapValues);
	std::string aux_line;
	std::getline(ss, aux_line);
	for (unsigned int i = 0; i < width; i++)
	{
		std::getline(ss, aux_line);
		std::istringstream lineStream(aux_line);
		for (unsigned int j = 0; j < height; j++)
		{
			int value;
			lineStream >> value;
			unsigned int desiredIndex = (i * width) + j;
			tileValues[desiredIndex] = value;
		}
	}
	
	spriteSheet = AssetManager::GetSpriteSheet(spritesheet_path);
}

void Tilemap::Delete()
{
	spriteSheet = NULL;
	free(path);
}
