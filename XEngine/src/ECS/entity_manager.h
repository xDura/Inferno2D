#pragma once
#include "../pool.h"
#include <unordered_map>
#include "../utils.h"
#include "../math.h"
#include "../2d_animation.h"
#include "stdint.h"

class SpriteSheet;
//64 bit bitset used as a mask
//to identify components and component groups
enum COMPONENT_ID : int64_t
{
	COMPONENT = 1 << 0,
	TRANSFORM = 1 << 1,
	SPRITE_RENDERER = 1 << 2,
	SPRITE_ANIMATOR = 1 << 3,
	BOX_COLLIDER = 1 << 4,
	CIRCLE_COLLIDER = 1 << 5,
	PLAYER_CONTROLLER = 1 << 6
};

static COMPONENT_ID component_id(int64_t values) 
{
	return (COMPONENT_ID)values;
}

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

enum SPRITE_RENDERER_FLAGS
{
	INVERT_X = 1 << 0,
	INVERT_Y = 1 << 1
};

enum RENDERER_LAYERS 
{
	PLAYER = 0,
	BACKGROUND = 1,
};

class SpriteRenderer : public Component
{
public:
	SpriteSheet* spriteSheet;
	int spriteIndex;
	RENDERER_LAYERS layer;
	SPRITE_RENDERER_FLAGS flags;
	virtual COMPONENT_ID GetComponentID() { return COMPONENT_ID::SPRITE_RENDERER; }
};

class SpriteAnimator : public Component
{
public:
	SpriteAnimation animation;
	virtual COMPONENT_ID GetComponentID() { return COMPONENT_ID::SPRITE_ANIMATOR; }
};

enum COLLIDER_TYPE
{
	BOX,
	CIRCLE
};

class Collider : Component
{
public:
	virtual COLLIDER_TYPE getType() = 0;
};

class BoxCollider : Collider
{
public:
	Vector2 size;
	virtual COLLIDER_TYPE getType() { return COLLIDER_TYPE::BOX; }
	virtual COMPONENT_ID GetComponentID() { return COMPONENT_ID::BOX_COLLIDER; };
};

class CircleCollider : Collider
{
public:
	float radius;
	virtual COLLIDER_TYPE getType() { return COLLIDER_TYPE::CIRCLE; }
	virtual COMPONENT_ID GetComponentID() { return COMPONENT_ID::CIRCLE_COLLIDER; };
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