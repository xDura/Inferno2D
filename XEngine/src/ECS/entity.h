#pragma once
#include "component.h"
#include <vector>

typedef long int ENTITY_ID;
class Entity 
{
public:
	ENTITY_ID id;
	std::vector<Component*> components;
};