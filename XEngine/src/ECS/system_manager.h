#pragma once
#include "entity_manager.h"

class Mesh;
class Shader;
class SpriteSheet;
class Camera;
class SpriteRendererSystem
{
public:
	static void RenderSprites(EntityManager* entityManager, Shader* shader, Camera* camera, Mesh* quadMesh);
};