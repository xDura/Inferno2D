#include "level.h"
#include "platform.h"
#include "External/tinyxml2.h"
#include "asset_manager.h"

void Level::SaveXML(const char * a_path) const
{
}

void Level::LoadXML(const char * a_path)
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
		return;
	}

	path = Copy(a_path);

	tinyxml2::XMLNode* versionNode = doc.FirstChild();
	tinyxml2::XMLNode* tilemapsNode = versionNode->NextSibling();
	int numTilemaps = 0;
	tilemapsNode->ToElement()->QueryIntAttribute("numTilemaps", &numTilemaps);
	tilemapsNode->ToElement()->QueryFloatAttribute("x", &position.x);
	tilemapsNode->ToElement()->QueryFloatAttribute("y", &position.y);
	tilemaps.resize(numTilemaps);
	tilemap_layers.resize(numTilemaps);

	tinyxml2::XMLNode* currentTilemap = tilemapsNode->FirstChild();
	for (int i = 0; i < numTilemaps; i++)
	{
		const char* path;
		int layer = 0;
		currentTilemap->ToElement()->QueryStringAttribute("path", &path);
		currentTilemap->ToElement()->QueryIntAttribute("layer", &layer);
		tilemaps[i] = AssetManager::GetTilemap(path);
		tilemap_layers[i] = layer;
		currentTilemap = currentTilemap->NextSibling();
	}

	doc.Clear();
	fclose(file);
}

void Level::ReloadXML()
{
}

void Level::Delete()
{
}
