#pragma once
#include "mesh.h"
#include "opengl_defines.h"

void Mesh::InitGL()
{
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	// position attribute
	glGenBuffersARB(1, &verts_VBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, verts_VBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, verts.size() * sizeof(Vector3), &verts[0], GL_STATIC_DRAW_ARB);
	glVertexAttribPointerARB(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
	glEnableVertexAttribArrayARB(0);

	// texture coord attribute
	glGenBuffersARB(1, &uvs_VBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, uvs_VBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, uvs.size() * sizeof(Vector2), &uvs[0], GL_STATIC_DRAW_ARB);
	glVertexAttribPointerARB(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void*)0);
	glEnableVertexAttribArrayARB(1);

	// colors attribute
	if (colors.size() > 0)
	{
		glGenBuffersARB(1, &colors_VBO);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, colors_VBO);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, colors.size() * sizeof(Vector3), &colors[0], GL_STATIC_DRAW_ARB);
		glVertexAttribPointerARB(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
		glEnableVertexAttribArrayARB(4);
	}

	glBindVertexArray(0);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	length = (int)verts.size();
}
