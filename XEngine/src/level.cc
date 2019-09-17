#include "level.h"
#include "platform.h"
#include "External/tinyxml2.h"
#include "asset_manager.h"

void Level::SaveXML(const char * a_path) const
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* versionNode = doc.NewElement("Version");
	tinyxml2::XMLText* versionText = doc.NewText("0.0.1");
	versionNode->InsertEndChild(versionText);
	doc.InsertEndChild(versionNode);

	tinyxml2::XMLElement* levelNode = doc.NewElement("level");
	levelNode->SetAttribute("numTilemaps", (int)tilemaps.size());
	levelNode->SetAttribute("x", position.x);
	levelNode->SetAttribute("y", position.y);
	doc.InsertEndChild(levelNode);

	for (unsigned int i = 0; i < tilemaps.size(); i++)
	{
		tinyxml2::XMLElement* currentTilemapNode = doc.NewElement("tilemap");
		currentTilemapNode->SetAttribute("layer", tilemap_layers[i]);
		currentTilemapNode->SetAttribute("path", tilemaps[i]->path);
		levelNode->InsertEndChild(currentTilemapNode);
	}

	std::string fullpath = Path::GetPath(a_path);
	FILE* file;
	fopen_s(&file, fullpath.c_str(), "w+");
	ASSERT(file != NULL);

	doc.SaveFile(file);
	doc.Clear();
	fclose(file);
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
	if (path == NULL)
	{
		LOGERROR("trying to reload an uninitialized level");
		return;
	}
	const char* pathCopy = Copy(path);
	Delete();
	LoadXML(pathCopy);
	free((char*)pathCopy);
}

void Level::Delete()
{
	tilemaps.clear();
	tilemap_layers.clear();
	free(path);
	path = NULL;

	//TODO: check out if we have to always do the entity destruction here
	level_entities.clear();
}
