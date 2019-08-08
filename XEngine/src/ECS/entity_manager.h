#pragma once
#include "entity.h"
#include "component.h"
#include "../pool.h"
#include <unordered_map>
#include "../utils.h"
#include "../math.h"
#include "../2d_animation.h"

class Texture;

//used as a 32 bit bitset as a mask
//to identify components and component groups
typedef enum COMPONENT_ID : unsigned long
{
	COMPONENT = 1 << 0,
	TRANSFORM = 1 << 1,
	SPRITE_RENDERER = 1 << 2,
	SPRITE_ANIMATOR = 1 << 3,
};

typedef long int ENTITY_ID;

class Component
{
public:
	virtual COMPONENT_ID GetComponentID() { return COMPONENT_ID::COMPONENT; };
	virtual void OnInitialize() {};
	virtual void OnDestroy() {};
};

class Transform : public Component 
{
public:
	Mat44 transform;
	virtual COMPONENT_ID GetComponentID() { return COMPONENT_ID::TRANSFORM; }
};

class SpriteRenderer : public Component
{
public:
	Texture* texture;
	Vector2 tileSize;
	int index;
	virtual COMPONENT_ID GetComponentID() { return COMPONENT_ID::SPRITE_RENDERER; }
};

class SpriteAnimator : public Component
{
public:
	SpriteAnimation animation;
	virtual COMPONENT_ID GetComponentID() { return COMPONENT_ID::SPRITE_ANIMATOR; }
};

class Entity
{
public:
	ENTITY_ID id;
	COMPONENT_ID currentComponents;
	std::vector<Component*> components;
	Component* GetComponent(COMPONENT_ID component_id) 
	{
		for (size_t i = 0; i < components.size(); i++)
		{
			if (components[i]->GetComponentID() == component_id)
				return components[i];
		}
		return NULL;
	}
};

class EntityManager 
{
public:
	ENTITY_ID lastEntityId;
	Pool<Entity> entityPool;

	Pool<Transform> transforms;
	Pool<SpriteRenderer> sprites;
	Pool<SpriteAnimator> animators;

	std::unordered_map<ENTITY_ID, Entity*> entities;
	void InitPools();
	Entity* CreateEntity();
	Entity* CreateEntity(COMPONENT_ID components);
	void DestroyEntity(Entity* entity);
	void DestroyComponent(Component* component);
	//TODO:method to clear everything
};