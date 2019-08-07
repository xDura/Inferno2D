#pragma once
#include "entity_manager.h"

void EntityManager::InitComponentPools()
{
}

void EntityManager::CreateEntity()
{
	Entity* entity = entityPool.spawn();
	entity->id = (lastEntityId + 1);
}
