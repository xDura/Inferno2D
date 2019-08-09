#pragma once
#include "game.h"
#include "math.h"
#include "mathTests.h"
#include "shader.h"
#include "camera.h"
#include "imguiLayer.h"
#include "mesh_loaders.h"
#include "pool.h"
#include "ECS/entity_manager.h"
#include "ECS/system_manager.h"
#include "sprite_sheet.h"

Mesh debugLines;
Camera* camera;
Texture* tex;
Texture* tileTex;
Shader* lineShader;
Shader* tiledShader;
Shader* simpleShader;
ImguiLayer* imguiLayer;

Mat44 model;
Mesh quadMesh;

//static defs
float Game::timeScale = 1.0f;
bool Game::needsShutDown = false;
int Game::tileSizeX = 24;
int Game::tileSizeY = 1;
int Game::tileIndex = 1;
SDL_Window* Game::window = NULL;
SDL_GLContext* Game::glContext = NULL;
SpriteAnimation Game::animation;
SpriteAnimator2 Game::animator;

//Test dinos
SpriteAnimation idleAnimation;
SpriteAnimation walkAnimation;
bool invertX = false;
bool lookingRight = true;

std::vector<int> ints;

SpriteSheet environtmentSpriteSheet;
EntityManager entityManager;

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

	/*unsigned int memsize = (unsigned int)10000;
	void* m = Memory::AllocateGameMemory(memsize);*/

	LOG("Sizeof Shader: %d", sizeof(Shader));

	simpleShader = new Shader();
	simpleShader->Load("data/Shaders/simpleVert.vs", "data/Shaders/simpleFrag.ps");
	tiledShader = new Shader();
	tiledShader->Load("data/Shaders/tileVert.vs", "data/Shaders/tileFrag.ps");

	tex = new Texture();
	tex->load("data/Sprites/DinoSprites_doux.png");
	tileTex = new Texture();
	tileTex->load("data/Sprites/tiles.png");

	environtmentSpriteSheet.Setup(5, 8, tileTex);
	environtmentSpriteSheet.height = 5;
	environtmentSpriteSheet.width = 8;

	entityManager.InitPools();
	COMPONENT_ID transformAndSprite = component_id(COMPONENT_ID::SPRITE_RENDERER | COMPONENT_ID::TRANSFORM);
	for (unsigned int i = 0; i < 10; i++)
	{
		Entity* entity = entityManager.CreateEntity(transformAndSprite);
		Transform* t = (Transform*)entity->GetComponent(COMPONENT_ID::TRANSFORM);
		SpriteRenderer* r = (SpriteRenderer*)entity->GetComponent(COMPONENT_ID::SPRITE_RENDERER);
		r->layer = (RENDERER_LAYERS)1;
		t->transform.translateLocal((float)i * 2.0f, 0.0f, (float)r->layer);
		r->spriteIndex = 25;
		r->spriteSheet = &environtmentSpriteSheet;
	}

	animator.currentAnimation = &idleAnimation;
	animator.normalizedTime = 0.0f;
	animator.currentFrameIndex = 0;

	idleAnimation.startIndex = 0;
	idleAnimation.endIndex = 4;
	idleAnimation.totalFrames = 5;
	idleAnimation.loop = true;
	idleAnimation.framesPerSecond = 1.2f;
	idleAnimation.totalSeconds = 5 * (1.0f / 1.2f);

	walkAnimation.startIndex = 4;
	walkAnimation.endIndex = 9;
	walkAnimation.totalFrames = 6;
	walkAnimation.loop = true;
	walkAnimation.framesPerSecond = 1.3f;
	walkAnimation.totalSeconds = 6 * (1.0f / 1.3f);

	//we want it to be edgy
	glDisable(GL_MULTISAMPLE);

	quadMesh = Mesh();
	Vector3 size = Vector3(1.0f, 1.0f, 1.0f);
	generateQuad(&quadMesh);

	model = Mat44();
	model.translateLocal(0.0f, 1.7f, -1.0f);

	Vector3 camPos = Vector3(0.0f, 1.0f, 10.0f);
	Vector3 camTarget = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 camUp = Vector3(0.0f, 1.0f, 0.0f);

	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	//camera = new Camera(camPos, camTarget, camUp, -10.0f, 10.0f, -7.0f, 7.0f, 0.1f, 10000.f);
	camera = new Camera(camPos, camTarget, camUp, 60.0f, width / (float)height, 0.1f, 10000.f);
}

void Game::Update(float deltaTime)
{
	deltaTime *= timeScale;
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


	if (Input::state[SDL_SCANCODE_LEFT])
	{
		lookingRight = false;
		Vector3 translation = Vector3(-5.0f, 0.0f, 0.0f);
		translation = translation * deltaTime;
		model.translateLocal(translation);
		if (animator.currentAnimation != &walkAnimation)
			animator.normalizedTime = 0.0f;
		animator.currentAnimation = &walkAnimation;
	}
	else if (Input::state[SDL_SCANCODE_RIGHT])
	{
		lookingRight = true;
		Vector3 translation = Vector3(5.0f, 0.0f, 0.0f);
		translation = translation * deltaTime;
		model.translateLocal(translation);
		if (animator.currentAnimation != &walkAnimation)
			animator.normalizedTime = 0.0f;
		animator.currentAnimation = &walkAnimation;
	}
	else
	{
		if (animator.currentAnimation != &idleAnimation)
			animator.normalizedTime = 0.0f;
		animator.currentAnimation = &idleAnimation;
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
				break;
			case SDL_WINDOWEVENT: 
				switch (event.window.event)
				{
					case SDL_WINDOWEVENT_RESIZED:
						glViewport(0, 0, event.window.data1, event.window.data2);
						camera->SetPerspective(60.0f, (float)event.window.data1 / (float)event.window.data2, 0.1f, 10000.f);
					break;
				}
				break;
			case SDL_QUIT:
				Game::needsShutDown = true;
				break;
		}
		imguiLayer->OnEvent(event);
	}

	imguiLayer->OnPreRender();
	animator.currentAnimation->totalFrames = animator.currentAnimation->endIndex - animator.currentAnimation->startIndex;
	animator.currentAnimation->totalSeconds = animator.currentAnimation->totalFrames * (1.0f / animator.currentAnimation->framesPerSecond);
	animator.Update(deltaTime);

	//******** MOVE THIS TO ONE OTHER PLACE : RENDERER**************
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SpriteRendererSystem::RenderSprites(&entityManager, tiledShader, camera, &quadMesh);

	Vector2 tileSize;
	tiledShader->enable();
	tiledShader->SetTexture(tex->tex_id);
	tiledShader->SetMat44("M", model * camera->viewProjectionMat);
	tileSize = Vector2((float)tileSizeX, (float)tileSizeY);
	tiledShader->SetVector2("tileSize", tileSize);
	tiledShader->SetInt("tileIndex", animator.currentFrameIndex);
	tiledShader->SetBool("invertX", !lookingRight);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBindVertexArray(quadMesh.VAO);
	glDrawArrays(GL_TRIANGLES, 0, quadMesh.length);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	tiledShader->disable();

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
