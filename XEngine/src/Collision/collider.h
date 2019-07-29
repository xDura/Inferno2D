#pragma once
#include "../math.h"

enum COLLIDER_TYPE
{
	BOX,
	CIRCLE
};

class Collider
{
public:
	virtual COLLIDER_TYPE getType() = 0;
};

class BoxCollider : Collider
{
public:
	Vector2 size;
	virtual COLLIDER_TYPE getType()
	{
		return COLLIDER_TYPE::BOX;
	}
};

class CircleCollider : Collider
{
public:
	float radius;
	virtual COLLIDER_TYPE getType()
	{
		return COLLIDER_TYPE::CIRCLE;
	}
};

class CollisionData
{
public:
	Vector2 position;
	Vector2 normal;
	Collider* other;
};

class Collision
{
	static bool collide(Collider* col_1, Collider* col_2, CollisionData* data)
	{

	}

	static bool collide(CircleCollider* col_1, CircleCollider* col_2, CollisionData* data)
	{
		
	}
};
