#pragma once
#include "entity_manager.h"

void EntityManager::InitPools()
{
	entityPool.prewarm(100);
	transforms.prewarm(200);
	sprites.prewarm(200);
	animators.prewarm(30);
}

Entity* EntityManager::CreateEntity()
{
	Entity* entity = entityPool.spawn();
	entity->id = (lastEntityId + 1);
	return entity;
}


Entity* EntityManager::CreateEntity(COMPONENT_ID components)
{
	Entity* entity = entityPool.spawn();
	entity->id = (lastEntityId + 1);
	entities.insert(std::make_pair(entity->id, entity));

	if ((components & COMPONENT_ID::TRANSFORM) != 0) 
	{
		Transform* t = transforms.spawn();
		t->OnInitialize();
		entity->components.push_back(t);
	}
	if ((components & COMPONENT_ID::SPRITE_RENDERER) != 0)
	{
		SpriteRenderer* r = sprites.spawn();
		r->OnInitialize();
		entity->components.push_back(r);
	}
	if ((components & COMPONENT_ID::SPRITE_ANIMATOR) != 0)
	{
		SpriteAnimator* a = animators.spawn();
		a->OnInitialize();
		entity->components.push_back(a);
	}

	entity->currentComponents = components;

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	entities.erase(entity->id);
	for (size_t i = 0; i < entity->components.size(); i++)
		DestroyComponent(entity->components[i]);
	entity->components.clear();
	entity->currentComponents = (COMPONENT_ID)0;
	entityPool.despawn(entity);
}

void EntityManager::DestroyComponent(Component* component)
{
	COMPONENT_ID id = component->GetComponentID();

	switch (id)
	{
		case COMPONENT_ID::TRANSFORM: 
		{
			Transform* t = (Transform*)component;
			t->OnDestroy();
			transforms.despawn(t);
		}
		break;
		case COMPONENT_ID::SPRITE_RENDERER:
		{
			SpriteRenderer* t = (SpriteRenderer*)component;
			t->OnDestroy();
			sprites.despawn(t);
		}
		break;
		case COMPONENT_ID::SPRITE_ANIMATOR:
		{
			SpriteAnimator* t = (SpriteAnimator*)component;
			t->OnDestroy();
			animators.despawn(t);
		}
		break;
		default:
			break;
	}
}
