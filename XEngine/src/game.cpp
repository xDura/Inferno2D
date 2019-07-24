#pragma once
#include "game.h"
#include "math.h"
#include "mathTests.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "imguiLayer.h"
#include "mesh_loaders.h"

Mesh debugLines;
Camera* camera;
Texture* tex;
Shader* lineShader;
Shader* skinnedMeshShader;
Shader* simpleShader;
ImguiLayer* imguiLayer;

Mat44 model;

Mesh quadMesh;

//static defs
bool Game::needsShutDown = false;
SDL_Window* Game::window = NULL;
SDL_GLContext* Game::glContext = NULL;

void Game::Init(SDL_Window* a_window, SDL_GLContext* a_context)
{
	window = a_window;
	glContext = a_context;
	imguiLayer = new ImguiLayer();
	imguiLayer->Init(window, glContext);
}

void Game::StartUp()
{
	Input::Init(window);

	lineShader = new Shader(basic_line_shader_vertex, basic_line_shader_fragment);
	simpleShader = new Shader(basic_vertex_shader_texture, basic_fragment_shader_texture);
	skinnedMeshShader = new Shader(skinned_mesh_vertex_shader_color, skinned_mesh_fragment_shader_color);

	tex = new Texture();
	tex->load("data/Sprites/tileset.png");

	//we want it to be edgy
	glDisable(GL_MULTISAMPLE);

	quadMesh = Mesh();
	Vector3 size = Vector3(1.0f, 1.0f, 1.0f);
	generateQuad(&quadMesh, size);

	//debugLines.InitLines();
	//debugLines.ReserveLines();
	model = Mat44();
	model.setScale(Vector3(2.0f, 1.0f, 1.0f));

	Vector3 camPos = Vector3(0.0f, 0.0f, -1.0f);
	Vector3 camTarget = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 camUp = Vector3(0.0f, 1.0f, 0.0f);

	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	/*camera = new Camera(camPos, camTarget, camUp, 3.0f, 3.0f, 3.0f, 3.0f, 0.1f, 10000.f);*/
	camera = new Camera(camPos, camTarget, camUp, 60.0f, width / (float)height, 0.1f, 10000.f);
}

void Game::Update(float deltaTime)
{
	Input::Update();

	////******** MOVE THIS TO ONE OTHER PLACE : UPDATEINPUT **************
	Vector3 cameraDir;
	if (Input::state[SDL_SCANCODE_W])
		cameraDir.z = 1.0f;
	else if (Input::state[SDL_SCANCODE_S])
		cameraDir.z = -1.0f;

	if (Input::state[SDL_SCANCODE_D])
		cameraDir.x = -1.0f;
	else if (Input::state[SDL_SCANCODE_A])
		cameraDir.x = 1.0f;

	if (Input::state[SDL_SCANCODE_Q])
		cameraDir.y = 1.0f;
	else if (Input::state[SDL_SCANCODE_E])
		cameraDir.y = -1.0f;

	cameraDir = cameraDir * (float)deltaTime;
	if (Input::state[SDL_SCANCODE_LSHIFT])
		cameraDir *= 80.f;

	if ((Input::mouseState & SDL_BUTTON_MIDDLE))
	{
		camera->RotateLocal(0, Input::mouse_dx * (float)deltaTime * 0.01f, 0.0f);
		camera->RotateLocal(Input::mouse_dy * (float)deltaTime * 0.01f, camera->getLocalVector(Vector3(1.0f, 0.0f, 0.0f)));
		Input::CenterMouse();
	}

	camera->MoveLocal(cameraDir);
	camera->UpdateViewProjection();

	//************************************************************

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYUP:
			if (event.key.keysym.scancode == SDL_SCANCODE_0)
			{}
			else if (event.key.keysym.scancode == SDL_SCANCODE_1)
			{}

			if (event.key.keysym.scancode == SDL_SCANCODE_KP_PLUS)
			{}
			else if (event.key.keysym.scancode == SDL_SCANCODE_KP_MINUS)
			{}
		case SDL_WINDOWEVENT_RESIZED:
			break;
		case SDL_QUIT:
			Game::needsShutDown = true;
			break;
		}
		imguiLayer->OnEvent(event);
	}

	imguiLayer->OnPreRender();

	//******** MOVE THIS TO ONE OTHER PLACE : RENDERER**************
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	simpleShader->enable();
	lineShader->SetTexture(tex->tex_id);
	simpleShader->SetMat44("M", model * camera->viewProjectionMat);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBindVertexArray(quadMesh.VAO);
	glDrawArrays(GL_TRIANGLES, 0, quadMesh.length);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	simpleShader->disable();

	//for (unsigned int i = 0; i < meshes.size(); i++)
	//{
	//	skinnedMeshShader->enable();
	//	skinnedMeshShader->SetTexture(tex->tex_id);
	//	skinnedMeshShader->SetMat44("M", models[i] * camera->viewProjectionMat);
	//	skinnedMeshShader->SetMat44Array("boneMats", meshes[i].boneMatrices[0], (int)meshes[i].boneMatrices.size());
	//	//skinnedMeshShader.SetFloat("desiredBoneID", (float)desiredBoneId);


	//	glEnable(GL_DEPTH_TEST);
	//	glEnable(GL_CULL_FACE);
	//	glBindVertexArray(meshes[i].VAO);
	//	glDrawArrays(GL_TRIANGLES, 0, meshes[i].length);
	//	glBindVertexArray(0);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glDisable(GL_CULL_FACE);
	//	glDisable(GL_DEPTH_TEST);

	//	skinnedMeshShader->disable();
	//}

	//DRAWING DEBUG LINES
	////TODO: Move this to some debug draw module
	//debugLines.ReserveLines();
	//lineShader.enable();
	//////TODO: push backs are super slow, just do a big ass resize or something
	//lineShader.SetMat44("MVP", models[0] * camera.viewProjectionMat);

	//////debug stuff to visualize the skeleton
	//Mesh::DebugSkeleton(&meshes[0], &debugLines);
	//////Vector3 pos = xbot.currentPoseBindMatrices[desiredBoneId].getPosition();
	//////debugLines.AddWhireSphereLines(pos, y, 0.3f);

	//debugLines.RenderAsLines();
	//debugLines.ClearLines();

	//lineShader.disable();
	//******

	imguiLayer->OnPostRender();

	SDL_GL_SwapWindow(window);

	//********************************************************
}

void Game::FixedUpdate(float deltaTime)
{
}

void Game::ShutDown()
{
	imguiLayer->ShutDown();
	free(imguiLayer);
}
