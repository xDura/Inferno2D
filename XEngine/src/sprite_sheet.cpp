#include "sprite_sheet.h"
#include "External/tinyxml2.h"
#include "debug.h"
#include <stdio.h>
#include "platform.h"
#include "utils.h"
#include "asset_manager.h"

void SpriteSheet::Setup(int a_width, int a_height, Texture* a_tex)
{
	width = a_width;
	height = a_height;
	texture = a_tex;
}

void SpriteSheet::Delete()
{
	width = 0;
	height = 0;
	free(path);
}

void SpriteSheet::SaveXML(const char * a_path)
{
	ASSERT(texture != NULL);
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* versionNode = doc.NewElement("Version");
	tinyxml2::XMLText* versionText = doc.NewText("0.0.1");
	versionNode->InsertEndChild(versionText);
	doc.InsertEndChild(versionNode);

	tinyxml2::XMLElement* spriteSheetNode = doc.NewElement("SpriteSheet");
	spriteSheetNode->SetAttribute("width", width);
	spriteSheetNode->SetAttribute("height", height);
	spriteSheetNode->SetAttribute("texture", texture->path);
	doc.InsertEndChild(spriteSheetNode);

	std::string fullpath = Path::GetPath(a_path);
	FILE* file;
	fopen_s(&file, fullpath.c_str(), "w+");
	ASSERT(file != NULL);

	doc.SaveFile(file);
	fclose(file);
}

void SpriteSheet::LoadXML(const char * a_path)
{
	ASSERT(path == NULL);
	std::string fullpath = Path::GetPath(a_path);
	FILE* file;
	fopen_s(&file, fullpath.c_str(), "rb");
	ASSERT(file != NULL);

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile(file);
	if (err != 0)
	{
		LOG("Error reading SpriteSheet from XML at %s", a_path);
		ASSERT(false);
	}

	tinyxml2::XMLNode* versionNode = doc.FirstChild();
	//TODO: check versions

	tinyxml2::XMLNode* spriteSheetNode = versionNode->NextSibling();
	tinyxml2::XMLElement* elem = spriteSheetNode->ToElement();
	elem->QueryUnsignedAttribute("width", &width);
	elem->QueryUnsignedAttribute("height", &height);
	path = Copy(a_path);
	const char* texturePath;
	elem->QueryStringAttribute("texture", &texturePath);
	texture = AssetManager::GetTexture(texturePath);
}
