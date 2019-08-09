#pragma once
#include "system_manager.h"
#include "../shader.h"
#include "../camera.h"
#include "../sprite_sheet.h"
#include "../mesh.h"

void SpriteRendererSystem::RenderSprites(EntityManager* entityManager, Shader* shader, Camera* camera, Mesh* quadMesh)
{
	Vector2 tileSize;
	shader->enable();
	for (auto it = entityManager->entities.begin(); it != entityManager->entities.end(); ++it)
	{
		Entity* currentEntity = (Entity*)it->second;
		if ((currentEntity->currentComponents & COMPONENT_ID::TRANSFORM) == 0) continue;
		if ((currentEntity->currentComponents & COMPONENT_ID::SPRITE_RENDERER) == 0) continue;

		Transform* t = (Transform*)currentEntity->GetComponent(COMPONENT_ID::TRANSFORM);
		SpriteRenderer* s = (SpriteRenderer*)currentEntity->GetComponent(COMPONENT_ID::SPRITE_RENDERER);
		shader->SetTexture(s->spriteSheet->texture->tex_id);
		shader->SetMat44("M", t->transform * camera->viewProjectionMat);
		tileSize = Vector2((float)s->spriteSheet->width, (float)s->spriteSheet->height);
		shader->SetVector2("tileSize", tileSize);
		shader->SetInt("tileIndex", s->spriteIndex);
		shader->SetBool("invertX", (s->flags & SPRITE_RENDERER_FLAGS::INVERT_X) != 0);

		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		glEnable(GL_CULL_FACE);
		glBindVertexArray(quadMesh->VAO);
		glDrawArrays(GL_TRIANGLES, 0, quadMesh->length);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	}
	shader->disable();
}
