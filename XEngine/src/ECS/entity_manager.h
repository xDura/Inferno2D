#pragma once
#include "entity.h"
#include "component.h"
#include "../pool.h"
#include <unordered_map>

class EntityManager 
{
public:
	ENTITY_ID lastEntityId;
	Pool<Entity> entityPool;
	std::unordered_map<ENTITY_ID, Entity*> entities;
	void InitComponentPools();
	void CreateEntity();
};