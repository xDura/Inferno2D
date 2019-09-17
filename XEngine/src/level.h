#ifndef LEVEL_H
#define LEVEL_H
#include "ECS/entity_manager.h"
#include "tilemap.h"
#include "sprite_sheet.h"
#include <vector>
#include "math.h"

class Level
{
public:
	std::vector<Tilemap*> tilemaps;
	std::vector<int> tilemap_layers;
	Vector2 position;

	char* path;
	std::vector<Entity*> level_entities;

	void SaveXML(const char* a_path) const;
	void LoadXML(const char* a_path);
	void ReloadXML();
	void Delete();
};
#endif //LEVEL_H